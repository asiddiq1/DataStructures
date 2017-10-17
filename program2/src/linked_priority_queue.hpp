//// Submitter: siddiqaa(Siddiq, Aisha)
////   Partner  : dongj3(Dong, Justin)
////   We certify that we worked cooperatively on this programming
////     assignment, according to the rules for pair programming
//
//
//#ifndef LINKED_PRIORITY_QUEUE_HPP_
//#define LINKED_PRIORITY_QUEUE_HPP_
//
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <initializer_list>
//#include "ics_exceptions.hpp"
//#include "array_stack.hpp"      //See operator <<
//
//
//namespace ics {
//
//
////Instantiate the template such that tgt(a,b) is true, iff a has higher priority than b
////With a tgt specified in the template, the constructor cannot specify a cgt.
////If a tgt is defaulted, then the constructor must supply a cgt (they cannot both be nullptr)
//template<class T, bool (*tgt)(const T& a, const T& b) = nullptr> class LinkedPriorityQueue {
//  public:
//    //Destructor/Constructors
//    ~LinkedPriorityQueue();
//
//    LinkedPriorityQueue          (bool (*cgt)(const T& a, const T& b) = nullptr);
//    LinkedPriorityQueue          (const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = nullptr);
//    explicit LinkedPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = nullptr);
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    explicit LinkedPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = nullptr);
//
//
//    //Queries
//    bool empty      () const;
//    int  size       () const;
//    T&   peek       () const;
//    std::string str () const; //supplies useful debugging information; contrast to operator <<
//
//
//    //Commands
//    int  enqueue (const T& element);
//    T    dequeue ();
//    void clear   ();
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    int enqueue_all (const Iterable& i);
//
//
//    //Operators
//    LinkedPriorityQueue<T,tgt>& operator = (const LinkedPriorityQueue<T,tgt>& rhs);
//    bool operator == (const LinkedPriorityQueue<T,tgt>& rhs) const;
//    bool operator != (const LinkedPriorityQueue<T,tgt>& rhs) const;
//
//    template<class T2, bool (*gt2)(const T2& a, const T2& b)>
//    friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T2,gt2>& pq);
//
//
//
//  private:
//    class LN;
//
//  public:
//    class Iterator {
//      public:
//        //Private constructor called in begin/end, which are friends of LinkedPriorityQueue<T,tgt>
//        ~Iterator();
//        T           erase();
//        std::string str  () const;
//        LinkedPriorityQueue<T,tgt>::Iterator& operator ++ ();
//        LinkedPriorityQueue<T,tgt>::Iterator  operator ++ (int);
//        bool operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
//        bool operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
//        T& operator *  () const;
//        T* operator -> () const;
//        friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>::Iterator& i) {
//          outs << i.str(); //Use the same meaning as the debugging .str() method
//          return outs;
//        }
//        friend Iterator LinkedPriorityQueue<T,tgt>::begin () const;
//        friend Iterator LinkedPriorityQueue<T,tgt>::end   () const;
//
//      private:
//        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
//        LN*             prev;            //prev should be initalized to the header
//        LN*             current;         //current == prev->next
//        LinkedPriorityQueue<T,tgt>* ref_pq;
//        int             expected_mod_count;
//        bool            can_erase = true;
//
//        //Called in friends begin/end
//        Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial);
//    };
//
//
//    Iterator begin () const;
//    Iterator end   () const;
//
//
//  private:
//    class LN {
//      public:
//        LN ()                      {}
//        LN (const LN& ln)          : value(ln.value), next(ln.next){}
//        LN (T v,  LN* n = nullptr) : value(v), next(n){}
//
//        T   value;
//        LN* next = nullptr;
//    };
//
//
//    bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
//    LN* front     =  new LN();
//    int used      =  0;                  //Cache for number of values in linked list
//    int mod_count =  0;                  //For sensing concurrent modification
//
//    //Helper methods
//    void delete_list(LN*& front);        //Deallocate all LNs, and set front's argument to nullptr;
//};
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////LinkedPriorityQueue class and related definitions
//
////Destructor/Constructors
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::~LinkedPriorityQueue() {
//  delete_list(front); //Including header node
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(bool (*cgt)(const T& a, const T& b))
//: gt(tgt != nullptr ? tgt : cgt)
//{
//	if (gt == nullptr)
//	{
//		delete front;
//		throw TemplateFunctionError("LinkedPriorityQueue::default constructor: neither specified");
//	}
//	if (tgt != nullptr && cgt != nullptr && tgt != cgt)
//	{
//		delete front;
//		throw TemplateFunctionError("LinkedPriorityQueue::default constructor: both specified and different");
//	}
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
//: gt(tgt != nullptr ? tgt : cgt), used(to_copy.used) {
//  if (gt == nullptr)
//    gt = to_copy.gt;//throw TemplateFunctionError("ArrayPriorityQueue::copy constructor: neither specified");
//  if (tgt != nullptr && cgt != nullptr && tgt != cgt)
//    throw TemplateFunctionError("LinkedPriorityQueue::copy constructor: both specified and different");
//
//  if (gt == to_copy.gt)
//  {
//    used = to_copy.used;
//    LN * head = front->next;
//    if (to_copy.front->next != nullptr)
//    {
//    	head = new LN(to_copy.front->next->value);
//    	front->next = head;
//
//    for (LN * t = to_copy.front->next->next; t != nullptr; t = t->next)
//    {
//    	head->next = new LN(t->value);
//    	head = head->next;
//    }
//    }
//
//  }
//  else
//  {
//    for (LN * t = to_copy.front->next; t != nullptr; t = t->next)
//    {
//      enqueue(t->value);
//    }
//
//  }
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
//:gt(tgt != nullptr ? tgt : cgt){
//	for (const T& q_elem : il)
//		{
//			enqueue(q_elem);
//		}
//
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//template<class Iterable>
//LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
//:gt(tgt != nullptr ? tgt : cgt){
//	for (const T& q_elem : i)
//		{
//			enqueue(q_elem);
//		}
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Queries
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//bool LinkedPriorityQueue<T,tgt>::empty() const {
//	return (used == 0);
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//int LinkedPriorityQueue<T,tgt>::size() const {
//	return used;
//
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//T& LinkedPriorityQueue<T,tgt>::peek () const {
//	if (empty())
//		throw EmptyError("LinkedPriorityQueue::peek");
//	return front->next->value;
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//std::string LinkedPriorityQueue<T,tgt>::str() const {
//	std::ostringstream answer;
//	answer << "linked_priority_queue[HEADER";
//	if (front->next != nullptr) {
//		answer << "->";
//	    for (LN * t = front->next; t != nullptr; t = t->next)
//	    {
//	    	if (t->next == nullptr)
//	    	{
//	    		answer << t->value;
//	    	}
//	    	else
//	    	{
//	    		answer << t->value << "->";
//	    	}
//	    }
//
//	  }
//	  answer << "](used=" << used << ",front=" << front << ",mod_count=" << mod_count << ")";
//	  return answer.str();
//
//
//
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Commands
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//int LinkedPriorityQueue<T,tgt>::enqueue(const T& element) {
//
//	if (front->next == nullptr)
//	{
//		front->next = new LN(element);
//		used++;
//		mod_count++;
//
//	}
//	else if (gt(front->next->value, element) == false)
//	{
//		LN * front_val = new LN(element);
//		front_val->next = front->next;
//		front->next = front_val;
//		used++;
//		mod_count++;
//	}
//	else
//	{
//	for (LN * t = front->next; t != nullptr; t = t->next)
//	{
//
//		if (t->next == nullptr)
//		{
//			LN* new_val = new LN(element);
//			t->next = new_val;
//			used++;
//			mod_count++;
//			break;
//		}
//
//		else if (gt(t->next->value, element) == false)
//		{
//			LN* new_val = new LN(element);
//			new_val->next = t->next;
//			t->next = new_val;
//			used++;
//			mod_count++;
//			break;
//		}
//
//	}
//
//	}
//
//	return 1;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//T LinkedPriorityQueue<T,tgt>::dequeue() {
//
//	if (this->empty())
//	{
//	    throw EmptyError("LinkedPriorityQueue::dequeue");
//	}
//
//	LN * first = front->next;
//	front->next = front->next->next;
//	T ret_val = first->value;
//	delete first;
//	used--;
//	mod_count++;
//	return ret_val;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//void LinkedPriorityQueue<T,tgt>::clear() {
//	delete_list(front);
//	front = new LN();
//	used = 0;
//	mod_count++;
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//template <class Iterable>
//int LinkedPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
//	int count = 0;
//	for (LN * p = i.front->next; p !=nullptr; p=p->next)
//	{
//		count += enqueue(p->value);
//	}
//
//	return count;
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Operators
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>& LinkedPriorityQueue<T,tgt>::operator = (const LinkedPriorityQueue<T,tgt>& rhs) {
//	if (this == &rhs)
//	{
//		return *this;
//	}
//	gt = rhs.gt;
//
//	if (this->size() < rhs.size())
//	{
//		LN * head = front->next;
//		for (LN * p = rhs.front->next; p != nullptr; p = p->next)
//			{
//				if (head != nullptr)
//				{
//					head->value = p->value;
//					head = head -> next;
//
//				}
//				else
//				{
//
//					this -> enqueue(p->value);
//				}
//			}
//
//
//		}
//
//		else if (this->size() >= rhs.size())
//		{
//			while (this->size() != rhs.size())
//			{
//				this -> dequeue();
//
//			}
//
//			if (rhs.front->next != nullptr)
//			{
//
//				LN * head = front->next;
//
//				for (LN* p = rhs.front->next; p != nullptr; p = p->next)
//				{
//					head->value = p->value;
//					head = head->next;
//
//				}
//
//			}
//
//		}
//	  used = rhs.used;
//	  ++mod_count;
//	  return *this;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//bool LinkedPriorityQueue<T,tgt>::operator == (const LinkedPriorityQueue<T,tgt>& rhs) const {
//	if (this == &rhs)
//		return true;
//
//	if (gt != rhs.gt)
//	    return false;
//
//	int used = this->size();
//	if (used != rhs.size())
//		return false;
//	LN * head = front->next;
//	for (LN * p = rhs.front->next; p != nullptr; p = p->next)
//	{
//		if (head->value != p->value)
//			return false;
//		head = head->next;
//	}
//	return true;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//bool LinkedPriorityQueue<T,tgt>::operator != (const LinkedPriorityQueue<T,tgt>& rhs) const {
//	return !(*this == rhs);
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>& pq) {
//	typename LinkedPriorityQueue<T>::LN* t;
//	ArrayStack<T> items;
//	outs << "priority_queue[";
//	for (auto t = pq.front->next; t != nullptr; t = t->next)
//	{
//		items.push(t->value);
//	}
//	if (!items.empty())
//	{
//		outs << items.pop();
//	}
//	while (!items.empty())
//		{
//			outs << "," << items.pop();
//		}
//	outs << "]:highest";
//
//		  return outs;
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Iterator constructors
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//auto LinkedPriorityQueue<T,tgt>::begin () const -> LinkedPriorityQueue<T,tgt>::Iterator {
//	return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this),front->next);
//
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//auto LinkedPriorityQueue<T,tgt>::end () const -> LinkedPriorityQueue<T,tgt>::Iterator {
//	return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this), nullptr);
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Private helper methods
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//void LinkedPriorityQueue<T,tgt>::delete_list(LN*& front) {
//	if (front != nullptr)
//	{
//		LN * del = front;
//		for (LN * p = front->next; p != nullptr; p = p->next)
//		{
//			front = p;
//			delete del;
//			del = front;
//		}
//	delete del;
//	del = nullptr;
//	front = nullptr;
//	}
//
//
//
//
//}
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Iterator class definitions
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::Iterator::Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial)
//: current(initial), ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count), prev(iterate_over->front){
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//LinkedPriorityQueue<T,tgt>::Iterator::~Iterator()
//{}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//T LinkedPriorityQueue<T,tgt>::Iterator::erase() {
//
//	  if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("ArrayPriorityQueue::Iterator::erase");
//	  if (!can_erase)
//	    throw CannotEraseError("ArrayPriorityQueue::Iterator::erase Iterator cursor already erased");
//	  if (current == nullptr)
//	    throw CannotEraseError("ArrayPriorityQueue::Iterator::erase Iterator cursor beyond data structure");
//
//	  can_erase = false;
//	  T to_return = current->value;
//	  if (prev == nullptr)
//	  {
//		    LN * t = current;
//	  		current = current->next;
//	  		ref_pq->front->next = current;
//	  		delete t;
//	  		ref_pq->used--;
//
//
//	  }
//	  	  else
//	  	  {
//
//	  	  prev->next = current->next;
//	  	  delete current;
//	  	  current = prev->next;
//	  	  ref_pq->used--;
//
//	  	  }
//
//	  expected_mod_count = ref_pq->mod_count;
//	  return to_return;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//std::string LinkedPriorityQueue<T,tgt>::Iterator::str() const {
//	std::ostringstream answer;
//	 answer << ref_pq->str() << "/current=" << current << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
//	 return answer.str();
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ () -> LinkedPriorityQueue<T,tgt>::Iterator& {
//	if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++");
//
//	  if (current == nullptr)
//	    return *this;
//
//	  if (can_erase)
//	  {
//	    prev = current;
//	  	current = prev->next;
//	  }
//	  else
//	    can_erase = true;
//
//	  return *this;
//
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> LinkedPriorityQueue<T,tgt>::Iterator {
//	 if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++(int)");
//
//	  if (current == nullptr)
//	    return *this;
//
//	  Iterator to_return(*this);
//	  if (can_erase)
//	  {
//		  prev = current;
//		  current = prev->next;//decreasing priority goes backward in array, towards 0
//	  }
//	  else
//	    can_erase = true;  //current already indexes "one beyond" deleted value
//
//	  return to_return;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//bool LinkedPriorityQueue<T,tgt>::Iterator::operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {
//	const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator ==");
//	  if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ==");
//	  if (ref_pq != rhsASI->ref_pq)
//	    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator ==");
//
//	  return current == rhsASI->current;
//
//}
//
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//bool LinkedPriorityQueue<T,tgt>::Iterator::operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {
//	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator !=");
//	  if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator !=");
//	  if (ref_pq != rhsASI->ref_pq)
//	    throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator !=");
//
//	  return current != rhsASI->current;
//
//}
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//T& LinkedPriorityQueue<T,tgt>::Iterator::operator *() const {
//	  if (expected_mod_count != ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
//	  if (!can_erase || current == nullptr) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_pq->size();
//	    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
//	  }
//
//	  return current->value;
//
//
//
//}
//
//template<class T, bool (*tgt)(const T& a, const T& b)>
//T* LinkedPriorityQueue<T,tgt>::Iterator::operator ->() const {
//	  if (expected_mod_count !=  ref_pq->mod_count)
//	    throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ->");
//	  if (!can_erase || current < 0 || current >= ref_pq->used) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_pq->size();
//	    throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator -> Iterator illegal: "+where.str());
//	  }
//
//	  return &current->value;
//
//}
//
//
//}
//
//#endif /* LINKED_PRIORITY_QUEUE_HPP_ */
