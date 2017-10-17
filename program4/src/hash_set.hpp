//// Submitter: siddiqaa(Siddiq, Aisha)
//
//#ifndef HASH_SET_HPP_
//#define HASH_SET_HPP_
//
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <initializer_list>
//#include "ics_exceptions.hpp"
//#include "pair.hpp"
//
//
//namespace ics {
//
//
////Instantiate the templated class supplying thash(a): produces a hash value for a.
////If thash is defaulted to nullptr in the template, then a constructor must supply chash.
////If both thash and chash are supplied, then they must be the same (by ==) function.
////If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
////The (unique) non-nullptr value supplied by thash/chash is stored in the instance variable hash.
//template<class T, int (*thash)(const T& a) = nullptr> class HashSet {
//  public:
//    //Destructor/Constructors
//    ~HashSet ();
//
//    HashSet          (double the_load_threshold = 1.0, int (*chash)(const T& a) = nullptr);
//    explicit HashSet (int initial_bins, double the_load_threshold = 1.0, int (*chash)(const T& k) = nullptr);
//    HashSet          (const HashSet<T,thash>& to_copy, double the_load_threshold = 1.0, int (*chash)(const T& a) = nullptr);
//    explicit HashSet (const std::initializer_list<T>& il, double the_load_threshold = 1.0, int (*chash)(const T& a) = nullptr);
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    explicit HashSet (const Iterable& i, double the_load_threshold = 1.0, int (*chash)(const T& a) = nullptr);
//
//
//    //Queries
//    bool empty      () const;
//    int  size       () const;
//    bool contains   (const T& element) const;
//    std::string str () const; //supplies useful debugging information; contrast to operator <<
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    bool contains_all (const Iterable& i) const;
//
//
//    //Commands
//    int  insert (const T& element);
//    int  erase  (const T& element);
//    void clear  ();
//
//    //Iterable class must support "for" loop: .begin()/.end() and prefix ++ on returned result
//
//    template <class Iterable>
//    int insert_all(const Iterable& i);
//
//    template <class Iterable>
//    int erase_all(const Iterable& i);
//
//    template<class Iterable>
//    int retain_all(const Iterable& i);
//
//
//    //Operators
//    HashSet<T,thash>& operator = (const HashSet<T,thash>& rhs);
//    bool operator == (const HashSet<T,thash>& rhs) const;
//    bool operator != (const HashSet<T,thash>& rhs) const;
//    bool operator <= (const HashSet<T,thash>& rhs) const;
//    bool operator <  (const HashSet<T,thash>& rhs) const;
//    bool operator >= (const HashSet<T,thash>& rhs) const;
//    bool operator >  (const HashSet<T,thash>& rhs) const;
//
//    template<class T2, int (hash2) (const T2& a)>
//    friend std::ostream& operator << (std::ostream& outs, const HashSet<T2,hash2>& s);
//
//
//
//  private:
//    class LN;
//
//  public:
//    class Iterator {
//      public:
//        typedef pair<int,LN*> Cursor;
//
//        //Private constructor called in begin/end, which are friends of HashSet<T,thash>
//        ~Iterator();
//        T           erase();
//        std::string str  () const;
//        HashSet<T,thash>::Iterator& operator ++ ();
//        HashSet<T,thash>::Iterator  operator ++ (int);
//        bool operator == (const HashSet<T,thash>::Iterator& rhs) const;
//        bool operator != (const HashSet<T,thash>::Iterator& rhs) const;
//        T& operator *  () const;
//        T* operator -> () const;
//        friend std::ostream& operator << (std::ostream& outs, const HashSet<T,thash>::Iterator& i) {
//          outs << i.str(); //Use the same meaning as the debugging .str() method
//          return outs;
//        }
//        friend Iterator HashSet<T,thash>::begin () const;
//        friend Iterator HashSet<T,thash>::end   () const;
//
//      private:
//        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
//        Cursor              current; //Bin Index with Pointer; stop: LN* == nullptr
//        HashSet<T,thash>*   ref_set;
//        int                 expected_mod_count;
//        bool                can_erase = true;
//
//        //Helper methods
//        void advance_cursors();
//
//        //Called in friends begin/end
//        Iterator(HashSet<T,thash>* iterate_over, bool from_begin);
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
//        LN* next   = nullptr;
//    };
//
//public:
//  int (*hash)(const T& k);   //Hashing function used (from template or constructor)
//private:
//  LN** set      = nullptr;   //Pointer to array of pointers: each bin stores a list with a trailer node
//  double load_threshold;     //used/bins <= load_threshold
//  int bins      = 1;         //# bins in array (should start > 0 so hash_compress doesn't % 0)
//  int used      = 0;         //Cache for number of key->value pairs in the hash table
//  int mod_count = 0;         //For sensing concurrent modification
//
//
//  //Helper methods
//  int   hash_compress        (const T& key)              const;  //hash function ranged to [0,bins-1]
//  LN*   find_element         (int bin, const T& element) const;  //Returns reference to element's node or nullptr
//  LN*   copy_list            (LN*   l)                   const;  //Copy the elements in a bin (order irrelevant)
//  LN**  copy_hash_table      (LN** ht, int bins)         const;  //Copy the bins/keys/values in ht tree (order in bins irrelevant)
//
//  void  ensure_load_threshold(int new_used);                     //Reallocate if load_threshold > load_threshold
//  void  delete_hash_table    (LN**& ht, int bins);               //Deallocate all LN in ht (and the ht itself; ht == nullptr)
//};
//
//
//
//
//
////HashSet class and related definitions
//
//////////////////////////////////////////////////////////////////////////////////
////
////Destructor/Constructors
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::~HashSet() {
//	delete_hash_table(set, bins);
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::HashSet(double the_load_threshold, int (*chash)(const T& element))
//: hash(thash != nullptr ? thash : chash){
//	  if (hash == nullptr)
//	    throw TemplateFunctionError("HashSet::length constructor: neither specified");
//	  if (thash != nullptr && chash != nullptr && thash != chash)
//	    throw TemplateFunctionError("HashSet::length constructor: both specified and different");
//
//	load_threshold = the_load_threshold;
//    set = new LN*[bins];
//    for (int i = 0; i < bins; i++)
//    {
//    	set[i] = new LN();
//    }
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::HashSet(int initial_bins, double the_load_threshold, int (*chash)(const T& element))
//: hash(thash != nullptr ? thash : chash){
//	  if (hash == nullptr)
//	    throw TemplateFunctionError("HashSet::length constructor: neither specified");
//	  if (thash != nullptr && chash != nullptr && thash != chash)
//	    throw TemplateFunctionError("HashSet::length constructor: both specified and different");
//
//	load_threshold = the_load_threshold;
//	bins = initial_bins;
//    set = new LN*[bins];
//    for (int i = 0; i < bins; i++)
//    {
//    	set[i] = new LN();
//    }
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::HashSet(const HashSet<T,thash>& to_copy, double the_load_threshold, int (*chash)(const T& element))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//	  hash = to_copy.hash;
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashSet::length constructor: both specified and different");
//
//  load_threshold = the_load_threshold;
//	if (hash == to_copy.hash)
//	{
//		set = copy_hash_table (to_copy.set, to_copy.bins);
//
//	}
//	else
//	{
//		set = new LN*[to_copy.bins];
//		set[0] = new LN();
//		for (auto kv: to_copy)
//		{
//			insert(kv);
//		}
//	}
//
//	used = to_copy.used;
//	bins = to_copy.bins;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::HashSet(const std::initializer_list<T>& il, double the_load_threshold, int (*chash)(const T& element))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//    throw TemplateFunctionError("HashSet::length constructor: neither specified");
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashSet::length constructor: both specified and different");
//
//  set = new LN*[bins];
//  set[0] = new LN();
//	for (auto k: il)
//		{
//			insert(k);
//		}
//}
//
//
//template<class T, int (*thash)(const T& a)>
//template<class Iterable>
//HashSet<T,thash>::HashSet(const Iterable& i, double the_load_threshold, int (*chash)(const T& a))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//    throw TemplateFunctionError("HashSet::length constructor: neither specified");
//
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashSet::length constructor: both specified and different");
//  load_threshold = the_load_threshold;
//  set = new LN*[bins];
//	set[0] = new LN();
//	for (auto k: i)
//		{
//			insert(k);
//		}
//
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Queries
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::empty() const {
//	return (used == 0);
//}
//
//
//template<class T, int (*thash)(const T& a)>
//int HashSet<T,thash>::size() const {
//	return (used);
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::contains (const T& element) const {
//	if (find_element(hash_compress(element), element) != nullptr)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//std::string HashSet<T,thash>::str() const {
//	std::ostringstream answer;
//
//	for (int x = 0; x < bins; x++)
//		{
//
//			answer << "bins[" << x << "]: ";
//			//items in the array
//			LN* items = set[x];
//
//			if (items->next == NULL)
//			{
//				answer << "TRAILER";
//			}
//			else
//			{
//			while (items->next != NULL)
//			{
//				answer << items->value;
//				items = items ->next; //go to next item
//				if (items->next != NULL)
//				{
//					answer << " -> ";
//				}
//			}
//
//			answer << "->TRAILER";
//			}
//			if (x+1 == bins)
//			{
//				answer << "#";
//			}
//			answer << "\n";}
//
//	return answer.str();
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//template <class Iterable>
//bool HashSet<T,thash>::contains_all(const Iterable& i) const {
//	  for (const T& v : i)
//	    if (!contains(v))
//	        return false;
//
//	    return true;
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Commands
//
//template<class T, int (*thash)(const T& a)>
//int HashSet<T,thash>::insert(const T& element) {
//
//	if(contains(element)){
//			return 0;
//		}
//
//	ensure_load_threshold(used + 1);
//	int hashIndex = hash_compress(element);
//	mod_count++;
//
//
//	set[hashIndex] = new LN(element, set[hashIndex]);
//	used++;
//	return 1;
//
// }
//
//
//template<class T, int (*thash)(const T& a)>
//int HashSet<T,thash>::erase(const T& element) {
//	if (contains(element))
//	{
//		int hash_index = hash_compress(element);
//		LN* item_location = set[hash_index]; //key located in this index here
//		LN* prev = nullptr;
//		LN* to_delete;
//		while (item_location->value != element)
//		{
//			prev = item_location; //keep track of current location
//			item_location = item_location->next; //move current
//		}
//
//		if (prev == nullptr)
//		{
//			prev = item_location;
//
//			item_location = item_location->next;
//			delete prev;
//			set[hash_index] = item_location;
//			used--;
//			mod_count++;
//			return 1;
//
//		}
//		else
//		{
//			to_delete = item_location;
//			prev->next = item_location->next;
//			item_location = item_location->next;
//			delete to_delete;
//			to_delete = nullptr;
//			used--;
//			mod_count++;
//			return 1;
//		}
//	}
//	else
//	{
//		 return 0;
//
//	}
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//void HashSet<T,thash>::clear() {
//	delete_hash_table(set, bins);
//	used = 0;
//	mod_count++;
//
//	set = new LN*[bins];
//	for(int i = 0; i < bins; i++){
//		set[i] = new LN();
//	}
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//template<class Iterable>
//int HashSet<T,thash>::insert_all(const Iterable& i) {
//	  int count = 0;
//	  for (const T& v : i)
//	     count += insert(v);
//
//	  return count;
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//template<class Iterable>
//int HashSet<T,thash>::erase_all(const Iterable& i) {
//	  int count = 0;
//	  for (const T& v : i)
//	     count += erase(v);
//
//	  return count;
//}
//
//
//template<class T, int (*thash)(const T& a)>
//template<class Iterable>
//int HashSet<T,thash>::retain_all(const Iterable& i) {
//	HashSet<T> s(i,load_threshold,hash);
//	int count = 0;
//	for(int x =0; x < bins; x++)
//	{
//		LN* val = set[x];
//		while(val->next != nullptr)
//		{
//			if(!s.contains(val->value))
//			{
//				T value = val->value;
//				count += erase(value);
//			}
//			val = val->next;
//
//		}
//	}
//	return count;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Operators
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>& HashSet<T,thash>::operator = (const HashSet<T,thash>& rhs) {
//	if (this == &rhs)
//		return *this;
//
//	delete_hash_table(set, bins);
//	set = copy_hash_table(rhs.set, rhs.bins);
//	used = rhs.used;
//	++mod_count;
//	return *this;
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator == (const HashSet<T,thash>& rhs) const {
//	  if (this == &rhs)
//	    return true;
//
//	  if (used != rhs.size())
//	    return false;
//
//	  for (int i=0; i < bins; ++i)
//	  {
//	    LN* set_val = set[i]; //current map so map_val is linked list
//	    while (set_val->next != nullptr) //current map of linked list is not empty
//	    {
//	    	if (!rhs.contains(set_val->value))
//	    	{
//	    		return false;
//	    	}
//	    	set_val = set_val->next;
//
//	    }
//
//	  }
//
//	  return true;
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator != (const HashSet<T,thash>& rhs) const {
//	return !(*this == rhs);
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator <= (const HashSet<T,thash>& rhs) const {
//	  if (this == &rhs)
//	    return true;
//	  if (used > rhs.size())
//	    return false;
//	  for (int i=0; i < bins; ++i)
//	  {
//	    LN* set_val = set[i]; //current map so map_val is linked list
//	    while (set_val->next != nullptr) //current map of linked list is not empty
//	    {
//	    	if (!rhs.contains(set_val->value))
//	    	{
//	    		return false;
//	    	}
//	    	set_val = set_val->next;
//
//	    }
//
//	  }
//
//	  return true;
//
//
//}
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator < (const HashSet<T,thash>& rhs) const {
//	  if (this == &rhs)
//	    return false;
//	  if (used >= rhs.size())
//	    return false;
//	  for (int i=0; i < bins; ++i)
//	  {
//	    LN* set_val = set[i]; //current map so map_val is linked list
//	    while (set_val->next != nullptr) //current map of linked list is not empty
//	    {
//	    	if (!rhs.contains(set_val->value))
//	    	{
//	    		return false;
//	    	}
//	    	set_val = set_val->next;
//
//	    }
//
//	  }
//
//	  return true;
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator >= (const HashSet<T,thash>& rhs) const {
//	return rhs <= *this;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::operator > (const HashSet<T,thash>& rhs) const {
//	return rhs < *this;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//std::ostream& operator << (std::ostream& outs, const HashSet<T,thash>& s) {
//
//			typename HashSet<T, thash>::LN* val;
//			int index = 0;
//			T val_sav;
//			outs << "set[";
//			  if (!s.empty()) {
//				  outs << s.set[0]->value;
//				  for (int x = 1; x < s.bins; x++)
//				  {
//					  while (s.set[x]->next != nullptr)
//					  {
//						  outs << "," << s.set[x]->value;
//
//					  }
//
//				  }
//
//			  }
//			  outs << "]";
//
//			  return outs;
//
//
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Iterator constructors
//
//template<class T, int (*thash)(const T& a)>
//auto HashSet<T,thash>::begin () const -> HashSet<T,thash>::Iterator {
//	return Iterator(const_cast<HashSet<T, thash>*>(this), true);
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//auto HashSet<T,thash>::end () const -> HashSet<T,thash>::Iterator {
//	return Iterator(const_cast<HashSet<T, thash>*>(this), false);
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Private helper methods
//
//template<class T, int (*thash)(const T& a)>
//int HashSet<T,thash>::hash_compress (const T& element) const {
//	int hash_slinging_slasher = hash(element);
//	return abs(hash_slinging_slasher) % bins;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//typename HashSet<T,thash>::LN* HashSet<T,thash>::find_element (int bin, const T& element) const {
//
//	for (int x = 0; x < bins; x++)
//	{
//		//items in the array
//		LN* items = set[x];
//		while (items->next != NULL)
//		{
//			if (element == items->value)
//			{
//				return items;
//			}
//			else
//			{
//					items = items->next; //go to next item
//			}
//
//		}
//	}
//		return nullptr;
//
//
//
//
//}
//
//template<class T, int (*thash)(const T& a)>
//typename HashSet<T,thash>::LN* HashSet<T,thash>::copy_list (LN* l) const {
//	LN* values_to_copy = new LN();
//	if (l == nullptr)
//		return values_to_copy;
//
//	while (l->next != nullptr)
//	{
//		values_to_copy = new LN(l->value, values_to_copy);
//		l = l ->next;
//	}
//	return values_to_copy;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//typename HashSet<T,thash>::LN** HashSet<T,thash>::copy_hash_table (LN** ht, int bins) const {
//	LN** new_set = new LN*[bins];
//
//	for (int x = 0; x < bins; x++){
//		LN* valz = ht[x];
//		new_set[x] = copy_list(valz); //copies the linkedlist values = copy_list?
//
//	}
//	return new_set;
//
//
//
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//void HashSet<T,thash>::ensure_load_threshold(int new_used) {
//	//checks if load factor is greater than threshold //new used is old used * 2 + 1
//	double load = double(new_used)/bins; //if this number > the load threshold
//
//	if (load > load_threshold)
//	{
//		int old_bins = bins; //old bins
//		bins = (bins * 2) + 1; //double the bins +1 ?
//		LN** new_map = new LN*[bins]; //pointer to new array
//		//set everything to null?
//		//put everything in old map back into new_map
//
//		for (int i = 0; i < bins; i++)
//		{
//		    new_map[i] = new LN();
//		}
//
//		for (int x = 0; x < old_bins; x++)
//		{
//			LN* old_map_pt = set[x];
//
//
//				while (old_map_pt != nullptr && old_map_pt->next != nullptr)
//				{
//					LN* prev = old_map_pt;
//					old_map_pt = old_map_pt->next;
//					LN*& current_bucket = new_map[hash_compress(prev->value)];
//					prev->next = current_bucket;
//					current_bucket = prev;
//				}
//		}
//
//		delete[] set;
//		set = new_map;
//	}
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//void HashSet<T,thash>::delete_hash_table (LN**& ht, int bins) {
//	for (int x = 0; x < bins; x++)
//	{
//		LN* items = ht[x]; //items in the array
//		while (items != NULL)
//		{
//			LN* prev_val = items->next; //points to next item
//			delete items; //delete current
//			items = prev_val; //item now in next one
//			//delete prev_val; //delete previous
//		}
//		ht[x] = nullptr;
//	}
//	delete[] ht; //delete array
//
//}
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Iterator class definitions
//
//
//template<class T, int (*thash)(const T& a)>
//void HashSet<T,thash>::Iterator::advance_cursors() {
//	if(current.first == -1){
//		current.second = nullptr;
//		can_erase = false;
//		return;
//	}
//
//	if(current.second != nullptr){
//		current.second = current.second->next;
//	}
//
//	while(current.second == nullptr || current.second->next == nullptr){
//		if(current.first == (ref_set->bins-1)){
//			current.first = -1;
//			current.second = nullptr;
//			can_erase = false;
//			return;
//		}
//		current.first++;
//		current.second = ref_set->set[current.first];
//	}
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::Iterator::Iterator(HashSet<T,thash>* iterate_over, bool begin)
//: ref_set(iterate_over), expected_mod_count(ref_set->mod_count) {
//
//	if (begin)
//	{
//		current.first = 0;
//		current.second = ref_set->set[0];
//		if(current.second->next == nullptr)
//			advance_cursors();
//
//	}
//	else
//	{
//		current.first = -1;
//		current.second = nullptr;
//		can_erase = false;
//	}
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//HashSet<T,thash>::Iterator::~Iterator()
//{}
//
//
//template<class T, int (*thash)(const T& a)>
//T HashSet<T,thash>::Iterator::erase() {
//	  if (expected_mod_count != ref_set->mod_count)
//		    throw ConcurrentModificationError("HashSet::Iterator::erase");
//		  if (!can_erase)
//		    throw CannotEraseError("HashSet::Iterator::erase Iterator cursor already erased");
//		  if (current.second == nullptr)
//		    throw CannotEraseError("HashSet::Iterator::erase Iterator cursor beyond data structure");
//
//		  can_erase = false;
//		  T to_return = current.second->value;
//		  advance_cursors();
//		  ref_set -> erase(to_return);
//		  expected_mod_count = ref_set->mod_count;
//		  return to_return;
//
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//std::string HashSet<T,thash>::Iterator::str() const {
//	std::ostringstream answer;
//	answer << ref_set->str()  << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
//	return answer.str();
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//auto  HashSet<T,thash>::Iterator::operator ++ () -> HashSet<T,thash>::Iterator& {
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator ++");
//
//	 if (current.second == nullptr)
//	    return *this;
//
//	  if (can_erase)
//	    advance_cursors();         //decreasing priority goes backward in array, towards 0
//	  else
//	    can_erase = true;  //current already indexes "one beyond" deleted value
//
//	  return *this;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//auto  HashSet<T,thash>::Iterator::operator ++ (int) -> HashSet<T,thash>::Iterator {
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator ++(int)");
//
//	  if (current.second == nullptr)
//	    return *this;
//
//	  Iterator to_return(*this);
//	  if (can_erase)
//	    advance_cursors();
//	  else
//	    can_erase = true;  //current already indexes "one beyond" deleted value
//
//	  return to_return;
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::Iterator::operator == (const HashSet<T,thash>::Iterator& rhs) const {
//	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("HashSet::Iterator::operator ==");
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator ==");
//	  if (ref_set != rhsASI->ref_set)
//	    throw ComparingDifferentIteratorsError("HashSet::Iterator::operator ==");
//
//	  return current == rhsASI->current;
//
//
//}
//
//
//template<class T, int (*thash)(const T& a)>
//bool HashSet<T,thash>::Iterator::operator != (const HashSet<T,thash>::Iterator& rhs) const {
//	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("HashSet::Iterator::operator !=");
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator !=");
//	  if (ref_set != rhsASI->ref_set)
//	    throw ComparingDifferentIteratorsError("HashSet::Iterator::operator !=");
//
//	  return current != rhsASI->current;
//
//}
//
//template<class T, int (*thash)(const T& a)>
//T& HashSet<T,thash>::Iterator::operator *() const {
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator *");
//	  if (!can_erase || current.first < 0 || current.second == nullptr) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_set->size();
//	    throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: "+where.str());
//	  }
////	  std::cout << "before current.second" << std::endl;
////	  std::cout << current.second->value << std::endl;
//	  return current.second->value;
//
//
//}
//
//template<class T, int (*thash)(const T& a)>
//T* HashSet<T,thash>::Iterator::operator ->() const {
//	  if (expected_mod_count != ref_set->mod_count)
//	    throw ConcurrentModificationError("HashSet::Iterator::operator ->");
//	  if (!can_erase || current.second == nullptr) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_set->size();
//	    throw IteratorPositionIllegal("HashSet::Iterator::operator -> Iterator illegal: "+where.str());
//	  }
//
//	  return &current.second->value;
//	}
//
//}
//
//
//
//#endif /* HASH_SET_HPP_ */
