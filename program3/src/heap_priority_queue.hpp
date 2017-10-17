// Submitter: siddiqaa(Siddiq, Aisha)

#ifndef HEAP_PRIORITY_QUEUE_HPP_
#define HEAP_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include <utility>              //For std::swap function
#include "array_stack.hpp"      //See operator <<
#include <algorithm>


namespace ics {


//Instantiate the templated class supplying tgt(a,b): true, iff a has higher priority than b.
//If tgt is defaulted to nullptr in the template, then a constructor must supply cgt.
//If both tgt and cgt are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-nullptr value supplied by tgt/cgt is stored in the instance variable gt.
template<class T, bool (*tgt)(const T& a, const T& b) = nullptr> class HeapPriorityQueue {
  public:
    //Destructor/Constructors
    ~HeapPriorityQueue();

    HeapPriorityQueue          (bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit HeapPriorityQueue (int initial_length, bool (*cgt)(const T& a, const T& b));
    HeapPriorityQueue          (const HeapPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = nullptr);
    explicit HeapPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = nullptr);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit HeapPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = nullptr);


    //Queries
    bool empty      () const;
    int  size       () const;
    T&   peek       () const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands
    int  enqueue (const T& element);
    T    dequeue ();
    void clear   ();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int enqueue_all (const Iterable& i);


    //Operators
    HeapPriorityQueue<T,tgt>& operator = (const HeapPriorityQueue<T,tgt>& rhs);
    bool operator == (const HeapPriorityQueue<T,tgt>& rhs) const;
    bool operator != (const HeapPriorityQueue<T,tgt>& rhs) const;

    template<class T2, bool (*gt2)(const T2& a, const T2& b)>
    friend std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T2,gt2>& pq);



    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of HeapPriorityQueue<T,tgt>
        ~Iterator();
        T           erase();
        std::string str  () const;
        HeapPriorityQueue<T,tgt>::Iterator& operator ++ ();
        HeapPriorityQueue<T,tgt>::Iterator  operator ++ (int);
        bool operator == (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const;
        bool operator != (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T,tgt>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }

        friend Iterator HeapPriorityQueue<T,tgt>::begin () const;
        friend Iterator HeapPriorityQueue<T,tgt>::end   () const;

      private:
        //If can_erase is false, the value has been removed from "it" (++ does nothing)
        HeapPriorityQueue<T,tgt>  it;                 //copy of HPQ (from begin), to use as iterator via dequeue
        HeapPriorityQueue<T,tgt>* ref_pq;
        int                      expected_mod_count;
        bool                     can_erase = true;

        //Called in friends begin/end
        //These constructors have different initializers (see it(...) in first one)
        Iterator(HeapPriorityQueue<T,tgt>* iterate_over, bool from_begin);    // Called by begin
        Iterator(HeapPriorityQueue<T,tgt>* iterate_over);                     // Called by end
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
    T*  pq;                              // Array represents a heap, so it uses heap ordering property
    int length    = 0;                   //Physical length of pq array: must be >= .size()
    int used      = 0;                   //Amount of array used:  invariant: 0 <= used <= length
    int mod_count = 0;                   //For sensing concurrent modification


    //Helper methods
    void ensure_length  (int new_length);
    int  left_child     (int i) const;         //Useful abstractions for heaps as arrays
    int  right_child    (int i) const;
    int  parent         (int i) const;
    bool is_root        (int i) const;
    bool in_heap        (int i) const;
    void percolate_up   (int i);
    void percolate_down (int i);
    void heapify        ();                   // Percolate down all value is array (from indexes used-1 to 0): O(N)
  };





////////////////////////////////////////////////////////////////////////////////
//
//HeapPriorityQueue class and related definitions

//Destructor/Constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::~HeapPriorityQueue() {
	delete[] pq;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt) {
  if (gt == nullptr)
    throw TemplateFunctionError("HeapPriorityQueue::default constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("HeapPriorityQueue::default constructor: both specified and different");

  pq = new T[length];



}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(int initial_length, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(initial_length) {
  if (gt == nullptr)
    throw TemplateFunctionError("HeapPriorityQueue::length constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("HeapPriorityQueue::length constructor: both specified and different");

  if (length < 0)
    length = 0;
  pq = new T[length];




}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const HeapPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(to_copy.length) {
  if (gt == nullptr)
    gt = to_copy.gt;
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("HeapPriorityQueue::copy constructor: both specified and different");

  pq = new T[length];

  if (gt == to_copy.gt) {
    used = to_copy.used;
    for (int i=0; i<to_copy.used; ++i)
      pq[i] = to_copy.pq[i];
  }else
  {
    for (int i=0; i<to_copy.used; ++i)
    	pq[i] = to_copy.pq[i];

    this->heapify();
  }




}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(il.size()) {
  if (gt == nullptr)
    throw TemplateFunctionError("HeapPriorityQueue::initializer_list constructor: neither specified");
  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
    throw TemplateFunctionError("HeapPriorityQueue::initializer_list constructor: both specified and different");

  pq = new T[length];

  int x = 0;
  for (const T& pq_elem : il)
  {
  	pq[x] = pq_elem;
  	x++;
  }

  used = il.size();
  this->heapify();





}


template<class T, bool (*tgt)(const T& a, const T& b)>
template<class Iterable>
HeapPriorityQueue<T,tgt>::HeapPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
: gt(tgt != nullptr ? tgt : cgt), length(i.size()) {
if (gt == nullptr)
  throw TemplateFunctionError("HeapPriorityQueue::Iterable constructor: neither specified");
if (tgt != nullptr && cgt != nullptr && tgt != cgt)
  throw TemplateFunctionError("HeapPriorityQueue::Iterable constructor: both specified and different");

pq = new T[length];

int x = 0;
for (const T& pq_elem : i)
{
	pq[x] = pq_elem;
	x++;
}

used = i.size();
this->heapify();







}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::empty() const {
	return (used == 0);
}


template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::size() const {
	return used;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& HeapPriorityQueue<T,tgt>::peek () const {
	  if (empty())
	    throw EmptyError("HeapPriorityQueue::peek");

	  return pq[0];


}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string HeapPriorityQueue<T,tgt>::str() const {
	  std::ostringstream answer;
	  answer << "HeapPriorityQueue[";

	  if (length != 0) {
	    answer << "0:" << pq[0];
	    for (int i = 1; i < length; ++i)
	      answer << "," << i << ":" << pq[i];
	  }

	  answer << "](length=" << length << ",used=" << used << ",mod_count=" << mod_count << ")";
	  return answer.str();


}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::enqueue(const T& element) {
	this->ensure_length(used+1);
	if (used == 0)
	{
		pq[used] = element;
		used++;
		mod_count++;
	}
	else
	{
		pq[used] = element;
		percolate_up(used); //percolate up swap value from parent
		used++;
		mod_count++;

	}

	return 1;


}


template<class T, bool (*tgt)(const T& a, const T& b)>
T HeapPriorityQueue<T,tgt>::dequeue() {
	if (this->empty())
		throw EmptyError("HeapPriorityQueue::dequeue");
	T root_val = pq[0]; //value to dequeue
	pq[0] = pq[--used];
	percolate_down(0);
	mod_count++;
	return root_val;


}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::clear() {
	used = 0;
	++mod_count;
}


template<class T, bool (*tgt)(const T& a, const T& b)>
template <class Iterable>
int HeapPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
	int count = 0;
	for (const T& v : i)
		count += enqueue(v);

	return count;



}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>& HeapPriorityQueue<T,tgt>::operator = (const HeapPriorityQueue<T,tgt>& rhs) {
	  if (this == &rhs)
	    return *this;

	  gt = rhs.gt;   // if tgt != nullptr, gts are already equal (or compiler error)
	  this->ensure_length(rhs.used);
	  used = rhs.used;
	  for (int i=0; i<used; ++i)
	  {
	    pq[i] = rhs.pq[i];
	  }

	  this->heapify();
	  ++mod_count;
	  return *this;



}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::operator == (const HeapPriorityQueue<T,tgt>& rhs) const {
	  if (this == &rhs)
	    return true;

	  if (gt != rhs.gt) //For PriorityQueues to be equal, they need the same gt function, and values
	    return false;

	  if (used != rhs.size())
	    return false;
	  HeapPriorityQueue<T,tgt>::Iterator rhs_i = rhs.begin();
	  HeapPriorityQueue<T,tgt>::Iterator rhs_pq = this->begin();
	  for (int i= 0 ; i < rhs.size(); i++, ++rhs_pq , ++rhs_i)
	    // Uses ! and ==, so != on T need not be defined
	    if (!(*rhs_pq == *rhs_i))
	      return false;

	  return true;




}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::operator != (const HeapPriorityQueue<T,tgt>& rhs) const {
	return !(*this == rhs);

}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::ostream& operator << (std::ostream& outs, const HeapPriorityQueue<T,tgt>& p) {
	ArrayStack<T> items;
	outs << "priority_queue[";
	for (auto k: p)
	{
		items.push(k);
	}

	  if (!p.empty()) {
	    outs << items.pop();
	  }
		while (!items.empty())
		{
			outs << "," << items.pop();
			}
		outs << "]:highest";

	return outs;


}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::begin () const -> HeapPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T,tgt>*>(this), 0);

}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::end () const -> HeapPriorityQueue<T,tgt>::Iterator {
	return Iterator(const_cast<HeapPriorityQueue<T,tgt>*>(this));
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::ensure_length(int new_length) {
	  if (length >= new_length)
	    return;
	  T* old_pq = pq;
	  length = std::max(new_length,2*length);
	  pq = new T[length];
	  for (int i=0; i<used; ++i)
	    pq[i] = old_pq[i];

	  delete [] old_pq;

}


template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::left_child(int i) const //returns index to left child
{
	int index_to_left = 2*i + 1;
	return index_to_left;


}

template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::right_child(int i) const
{
	int index_to_right = 2*i + 2;
	return index_to_right;

}

template<class T, bool (*tgt)(const T& a, const T& b)>
int HeapPriorityQueue<T,tgt>::parent(int i) const
{
	int index_to_parent = (i - 1) /2;
	return index_to_parent;


}

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::is_root(int i) const
{
	return (i == 0);



}

template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::in_heap(int i) const
{
	return (i < used);

}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::percolate_up(int i) {


	for (int x = i; !is_root(x); x = parent(x))
	{

		if (gt(pq[x], pq[parent(x)]))
		{
			std::swap(pq[x], pq[parent(x)]);

		}
		else
		{
			break;
		}

	}

}




template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::percolate_down(int i) {
	int value;
	while (in_heap(i))
	{
		if (!in_heap(left_child(i)) && !in_heap(right_child(i)))
		{
			break;
		}
		if (in_heap(left_child(i)) && !in_heap(right_child(i)))
		{
			value = left_child(i);
		}

		else if (gt(pq[left_child(i)], pq[right_child(i)]))
		{
			value = left_child(i);
		}
		else
		{
			value = right_child(i);

		}

		if (gt(pq[value], pq[i]))
		{
			std::swap(pq[value], pq[i]);
			i = value;

		}
		else
		{
			break;
		}

	}

}


template<class T, bool (*tgt)(const T& a, const T& b)>
void HeapPriorityQueue<T,tgt>::heapify() {
for (int i = used-1; i >= 0; --i)
  percolate_down(i);
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::Iterator(HeapPriorityQueue<T,tgt>* iterate_over, bool tgt_nullptr)
: it(*iterate_over,iterate_over->gt), ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count) {


}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::Iterator(HeapPriorityQueue<T,tgt>* iterate_over)
:it(iterate_over->gt), ref_pq(iterate_over), expected_mod_count(iterate_over->mod_count){
}


template<class T, bool (*tgt)(const T& a, const T& b)>
HeapPriorityQueue<T,tgt>::Iterator::~Iterator()
{}


template<class T, bool (*tgt)(const T& a, const T& b)>
T HeapPriorityQueue<T,tgt>::Iterator::erase() {
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::erase");
	  if (!can_erase)
	    throw CannotEraseError("HeapPriorityQueue::Iterator::erase Iterator cursor already erased");
	  if (it.empty() || it.used > ref_pq->used)
	    throw CannotEraseError("HeapPriorityQueue::Iterator::erase Iterator cursor beyond data structure");

	  can_erase = false;
	  T to_return = it.dequeue();
	  int index;
	  for (int x = 0; x < ref_pq->size(); x++)
	  {
		  if (ref_pq->pq[x] == to_return)
		  {
			  index = x;

		  }
	  }

	  ref_pq->pq[index] = ref_pq->pq[--ref_pq->used];
	  ref_pq->percolate_up(index);
	  ref_pq->percolate_down(index);
	  expected_mod_count = ref_pq->mod_count;
	  return to_return;



}


template<class T, bool (*tgt)(const T& a, const T& b)>
std::string HeapPriorityQueue<T,tgt>::Iterator::str() const {
	  std::ostringstream answer;
	  answer << ref_pq->str()  << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
	  return answer.str();

}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::Iterator::operator ++ () -> HeapPriorityQueue<T,tgt>::Iterator& {
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ++");

	  if (it.empty())
	    return *this;

	  if (can_erase)
	    it.dequeue();         //decreasing priority goes backward in array, towards 0
	  else
	    can_erase = true;  //current already indexes "one beyond" deleted value

	  return *this;


}


template<class T, bool (*tgt)(const T& a, const T& b)>
auto HeapPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> HeapPriorityQueue<T,tgt>::Iterator {
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ++(int)");

	  if (it.empty())
	    return *this;

	  Iterator to_return(*this);
	  if (can_erase)
		  it.dequeue();        //decreasing priority goes backward in array, towards 0
	  else
	    can_erase = true;  //current already indexes "one beyond" deleted value

	  return to_return;


}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::Iterator::operator == (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const {
	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	  if (rhsASI == 0)
	    throw IteratorTypeError("HeapPriorityQueue::Iterator::operator ==");
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ==");
	  if (ref_pq != rhsASI->ref_pq)
	    throw ComparingDifferentIteratorsError("HeapPriorityQueue::Iterator::operator ==");

	  return it.size() == rhsASI->it.size();


}


template<class T, bool (*tgt)(const T& a, const T& b)>
bool HeapPriorityQueue<T,tgt>::Iterator::operator != (const HeapPriorityQueue<T,tgt>::Iterator& rhs) const {
	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
	  if (rhsASI == 0)
	    throw IteratorTypeError("HeapPriorityQueue::Iterator::operator !=");
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator !=");
	  if (ref_pq != rhsASI->ref_pq)
	    throw ComparingDifferentIteratorsError("HeapPriorityQueue::Iterator::operator !=");

	  return it.size() != rhsASI->it.size();





}


template<class T, bool (*tgt)(const T& a, const T& b)>
T& HeapPriorityQueue<T,tgt>::Iterator::operator *() const {
	  if (expected_mod_count != ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator *");
	  if (!can_erase || it.empty() || it.used > ref_pq->used) {
	    std::ostringstream where;
	    where << it << " when size = " << ref_pq->size();
	    throw IteratorPositionIllegal("HeapPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
	  }

	  return it.peek();

}


template<class T, bool (*tgt)(const T& a, const T& b)>
T* HeapPriorityQueue<T,tgt>::Iterator::operator ->() const {
	  if (expected_mod_count !=  ref_pq->mod_count)
	    throw ConcurrentModificationError("HeapPriorityQueue::Iterator::operator ->");
	  if (!can_erase || it.empty()  || it.used > ref_pq->used) {
	    std::ostringstream where;
	    where << it << " when size = " << ref_pq->size();
	    throw IteratorPositionIllegal("HeapPriorityQueue::Iterator::operator -> Iterator illegal: "+where.str());
	  }

	  return &it.peek();


}

}

#endif /* HEAP_PRIORITY_QUEUE_HPP_ */
