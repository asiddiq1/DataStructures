//// Submitter: siddiqaa(Siddiq, Aisha)
//
//#ifndef HASH_MAP_HPP_
//#define HASH_MAP_HPP_
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
//template<class KEY,class T, int (*thash)(const KEY& a) = nullptr> class HashMap {
//  public:
//    typedef ics::pair<KEY,T>   Entry;
//
//    //Destructor/Constructors
//    ~HashMap ();
//
//    HashMap          (double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
//    explicit HashMap (int initial_bins, double the_load_threshold = 1.0, int (*chash)(const KEY& k) = nullptr);
//    HashMap          (const HashMap<KEY,T,thash>& to_copy, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
//    explicit HashMap (const std::initializer_list<Entry>& il, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    explicit HashMap (const Iterable& i, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = nullptr);
//
//
//    //Queries
//    bool empty      () const;
//    int  size       () const;
//    bool has_key    (const KEY& key) const;
//    bool has_value  (const T& value) const;
//    std::string str () const; //supplies useful debugging information; contrast to operator <<
//
//
//    //Commands
//    T    put   (const KEY& key, const T& value);
//    T    erase (const KEY& key);
//    void clear ();
//
//    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
//    template <class Iterable>
//    int put_all(const Iterable& i);
//
//
//    //Operators
//
//    T&       operator [] (const KEY&);
//    const T& operator [] (const KEY&) const;
//    HashMap<KEY,T,thash>& operator = (const HashMap<KEY,T,thash>& rhs);
//    bool operator == (const HashMap<KEY,T,thash>& rhs) const;
//    bool operator != (const HashMap<KEY,T,thash>& rhs) const;
//
//    template<class KEY2,class T2, int (*hash2)(const KEY2& a)>
//    friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY2,T2,hash2>& m);
//
//
//
//  private:
//    class LN;
//
//  public:
//    class Iterator {
//      public:
//         typedef pair<int,LN*> Cursor;
//
//        //Private constructor called in begin/end, which are friends of HashMap<T>
//        ~Iterator();
//        Entry       erase();
//        std::string str  () const;
//        HashMap<KEY,T,thash>::Iterator& operator ++ ();
//        HashMap<KEY,T,thash>::Iterator  operator ++ (int);
//        bool operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const;
//        bool operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const;
//        Entry& operator *  () const;
//        Entry* operator -> () const;
//        friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>::Iterator& i) {
//          outs << i.str(); //Use the same meaning as the debugging .str() method
//          return outs;
//        }
//        friend Iterator HashMap<KEY,T,thash>::begin () const;
//        friend Iterator HashMap<KEY,T,thash>::end   () const;
//
//      private:
//        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
//        Cursor               current; //Bin Index with Pointer; stop: LN* == nullptr
//        HashMap<KEY,T,thash>* ref_map;
//        int                  expected_mod_count;
//        bool                 can_erase = true;
//
//        //Helper methods
//        void advance_cursors();
//
//        //Called in friends begin/end
//        Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin);
//    };
//
//
//    Iterator begin () const;
//    Iterator end   () const;
//
//
//  private:
//    class LN {
//    public:
//      LN ()                         : next(nullptr){}
//      LN (const LN& ln)             : value(ln.value), next(ln.next){}
//      LN (Entry v, LN* n = nullptr) : value(v), next(n){}
//
//      Entry value;
//      LN*   next;
//  };
//
//  int (*hash)(const KEY& k);  //Hashing function used (from template or constructor)
//  LN** map      = nullptr;    //Pointer to array of pointers: each bin stores a list with a trailer node
//  double load_threshold;      //used/bins <= load_threshold
//  int bins      = 1;          //# bins in array (should start > 0 so hash_compress doesn't % 0)
//  int used      = 0;          //Cache for number of key->value pairs in the hash table
//  int mod_count = 0;          //For sensing concurrent modification
//
//
//  //Helper methods
//  int   hash_compress        (const KEY& key)          const;  //hash function ranged to [0,bins-1]
//  LN*   find_key             (int bin, const KEY& key) const;  //Returns reference to key's node or nullptr
//  LN*   copy_list            (LN*   l)                 const;  //Copy the keys/values in a bin (order irrelevant)
//  LN**  copy_hash_table      (LN** ht, int bins)       const;  //Copy the bins/keys/values in ht tree (order in bins irrelevant)
//
//  void  ensure_load_threshold(int new_used);                   //Reallocate if load_factor > load_threshold
//  void  delete_hash_table    (LN**& ht, int bins);             //Deallocate all LN in ht (and the ht itself; ht == nullptr)
//};
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////HashMap class and related definitions
//
////Destructor/Constructors
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::~HashMap(){
//	delete_hash_table(map, bins);
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::HashMap(double the_load_threshold, int (*chash)(const KEY& k))
//: hash(thash != nullptr ? thash : chash){
//	  if (hash == nullptr)
//	    throw TemplateFunctionError("HashMap::length constructor: neither specified");
//	  if (thash != nullptr && chash != nullptr && thash != chash)
//	    throw TemplateFunctionError("HashMap::length constructor: both specified and different");
//
//	load_threshold = the_load_threshold;
//    map = new LN*[bins];
//    for (int i = 0; i < bins; i++)
//    {
//    	map[i] = new LN();
//    }
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::HashMap(int initial_bins, double the_load_threshold, int (*chash)(const KEY& k))
//: hash(thash != nullptr ? thash : chash){
//	  if (hash == nullptr)
//	    throw TemplateFunctionError("HashMap::length constructor: neither specified");
//	  if (thash != nullptr && chash != nullptr && thash != chash)
//	    throw TemplateFunctionError("HashMap::length constructor: both specified and different");
//
//	load_threshold = the_load_threshold;
//	bins = initial_bins;
//    map = new LN*[bins];
//    for (int i = 0; i < bins; i++)
//    {
//    	map[i] = new LN();
//    }
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::HashMap(const HashMap<KEY,T,thash>& to_copy, double the_load_threshold, int (*chash)(const KEY& a))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//	  hash = to_copy.hash;
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashMap::length constructor: both specified and different");
//
//  load_threshold = the_load_threshold;
//	if (hash == to_copy.hash)
//	{
//		map = copy_hash_table (to_copy.map, to_copy.bins);
//
//	}
//	else
//	{
//		map = new LN*[to_copy.bins];
//		map[0] = new LN();
//		for (auto kv: to_copy)
//		{
//			put(kv.first, kv.second);
//		}
//	}
//
//	used = to_copy.used;
//	bins = to_copy.bins;
//
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::HashMap(const std::initializer_list<Entry>& il, double the_load_threshold, int (*chash)(const KEY& k))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//    throw TemplateFunctionError("HashMap::length constructor: neither specified");
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashMap::length constructor: both specified and different");
//
//  map = new LN*[bins];
//  map[0] = new LN();
//	for (auto kv: il)
//		{
//			put(kv.first, kv.second);
//		}
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//template <class Iterable>
//HashMap<KEY,T,thash>::HashMap(const Iterable& i, double the_load_threshold, int (*chash)(const KEY& k))
//: hash(thash != nullptr ? thash : chash){
//  if (hash == nullptr)
//    throw TemplateFunctionError("HashMap::length constructor: neither specified");
//
//  if (thash != nullptr && chash != nullptr && thash != chash)
//    throw TemplateFunctionError("HashMap::length constructor: both specified and different");
//  load_threshold = the_load_threshold;
//  map = new LN*[bins];
//	map[0] = new LN();
//	for (auto kv: i)
//		{
//			put(kv.first, kv.second);
//		}
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Queries
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::empty() const {
//	return (used == 0);
//
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//int HashMap<KEY,T,thash>::size() const {
//	return used;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::has_key (const KEY& key) const {
//	if (find_key(hash_compress(key), key) != nullptr)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::has_value (const T& value) const {
//	for (int x = 0; x < bins; x++)
//	{
//		//items in the array
//		LN* items = map[x];
//		while (items->next != NULL)
//		{
//			if (value == items->value.second)
//			{
//				return true;
//			}
//			else
//			{
//					items = items->next; //go to next item
//			}
//
//		}
//	}
//		return false;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//std::string HashMap<KEY,T,thash>::str() const {
//	std::ostringstream answer;
//
//	for (int x = 0; x < bins; x++)
//		{
//
//			answer << "bins[" << x << "]: ";
//			//items in the array
//			LN* items = map[x];
//
//			if (items->next == NULL)
//			{
//				answer << "TRAILER";
//			}
//			else
//			{
//			while (items->next != NULL)
//			{
//				answer << items->value.first << "->" << items->value.second;
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
//////////////////////////////////////////////////////////////////////////////////
////
////Commands
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//T HashMap<KEY,T,thash>::put(const KEY& key, const T& value) {
//	T returnValue;
//
//	ensure_load_threshold(used + 1);
//	int hashIndex = hash_compress(key);
//	mod_count++;
//
//	if(has_key(key)){
//		LN* nodePointer = find_key(hashIndex, key);
//		returnValue = nodePointer -> value.second;
//		nodePointer -> value.second = value;
//		return returnValue;
//	}
//
//	map[hashIndex] = new LN(Entry(key, value), map[hashIndex]);
//	used++;
//	return value;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//T HashMap<KEY,T,thash>::erase(const KEY& key) {
//	if (has_key(key))
//	{
//		int hash_index = hash_compress(key);
//		LN* item_location = map[hash_index]; //key located in this index here
//		LN* prev = nullptr;
//		LN* to_delete;
//		while (item_location->value.first != key)
//		{
//			prev = item_location; //keep track of current location
//			item_location = item_location->next; //move current
//		}
//
//		if (prev == nullptr)
//		{
//			prev = item_location;
//			T val_ret = prev->value.second;
//			item_location = item_location->next;
//			delete prev;
//			map[hash_index] = item_location;
//			used--;
//			mod_count++;
//			return val_ret;
//
//		}
//		else
//		{
//			to_delete = item_location;
//			T val_ret = to_delete->value.second;
//			prev->next = item_location->next;
//			item_location = item_location->next;
//			delete to_delete;
//			to_delete = nullptr;
//			used--;
//			mod_count++;
//			return val_ret;
//		}
//	}
//	else
//	{
//		 std::ostringstream answer;
//		 answer << "HashMap::erase: key(" << key << ") not in Map";
//		 throw KeyError(answer.str());
//
//	}
//
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//void HashMap<KEY,T,thash>::clear(){
//	delete_hash_table(map, bins);
//	used = 0;
//	mod_count++;
//
//	map = new LN*[bins];
//	for(int i = 0; i < bins; i++){
//		map[i] = new LN();
//	}
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//template<class Iterable>
//int HashMap<KEY,T,thash>::put_all(const Iterable& i) {
//		  int count = 0;
//		  for (auto m_entry : i) {
//		    ++count;
//		    put(m_entry.first, m_entry.second);
//		  }
//		    return count;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Operators
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//T& HashMap<KEY,T,thash>::operator [] (const KEY& key) {
//	if(!has_key(key))
//	{
//		used++;
//		ensure_load_threshold(used);
//		int nodePointer = hash_compress(key); //changed it to here because ensure load threshold will change the key position
//		map[nodePointer] = new LN(Entry(key, T()), map[nodePointer]);
//		mod_count++;
//	}
//
//	return find_key(hash_compress(key),key)->value.second;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//const T& HashMap<KEY,T,thash>::operator [] (const KEY& key) const{
//	LN* val = find_key(hash_compress(key), key);
//	if (val != nullptr)
//		return val->value.second;
//
//	std::ostringstream answer;
//	answer << "HashMap::operator []: key(" << key << ") not in Map";
//	throw KeyError(answer.str());
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>& HashMap<KEY,T,thash>::operator = (const HashMap<KEY,T,thash>& rhs) {
//	if (this == &rhs)
//		return *this;
//
//	delete_hash_table(map, bins);
//	map = copy_hash_table(rhs.map, rhs.bins); //might be wrong
//	used = rhs.used;
//	++mod_count;
//	return *this;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::operator == (const HashMap<KEY,T,thash>& rhs) const {
//		  if (this == &rhs)
//		    return true;
//
//		  if (used != rhs.size())
//		    return false;
//
//		  for (int i=0; i < bins; ++i)
//		  {
//		    LN* map_val = map[i]; //current map so map_val is linked list
//		    while (map_val->next != nullptr) //current map of linked list is not empty
//		    {
//		    	if (!rhs.has_key(map_val->value.first) || !(map_val->value.second == rhs[map_val->value.first]))
//		    	{
//		    		return false;
//		    	}
//		    	map_val = map_val->next;
//
//		    }
//
//		  }
//
//		  return true;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::operator != (const HashMap<KEY,T,thash>& rhs) const {
//	return !(*this == rhs);
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>& m){
//		typename HashMap<KEY, T, thash>::Iterator val = m.begin();
//		outs << "map[";
//		  if (!m.empty()) {
//			  outs << val->first << "->" << val->second;
//			  val++;
//			  while(val != m.end())
//			  {
//				  outs << "," << val->first << "->" << val->second;
//				  val++;
//			  }
//		  }
//		  outs << "]";
//
//		  return outs;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Iterator constructors
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//auto HashMap<KEY,T,thash>::begin () const -> HashMap<KEY,T,thash>::Iterator {
//	return Iterator(const_cast<HashMap<KEY,T, thash>*>(this), true);
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//auto HashMap<KEY,T,thash>::end () const -> HashMap<KEY,T,thash>::Iterator {
//	return Iterator(const_cast<HashMap<KEY,T, thash>*>(this), false);
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
////
////Private helper methods
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//int HashMap<KEY,T,thash>::hash_compress (const KEY& key) const {
//	int hash_slinging_slasher = hash(key);
//	return abs(hash_slinging_slasher) % bins;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::find_key (int bin, const KEY& key) const {
//	//find key meow
////	for (int x = 0; x < bins; x++)
////	{
//	    int x = bin;
//		//items in the array
//		LN* items = map[x];
//		while (items->next != NULL)
//		{
//			if (key == items->value.first)
//			{
//				return items;
//			}
//			else
//			{
//					items = items->next; //go to next item
//			}
//
//		}
////	}
//		return nullptr;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::copy_list (LN* l) const {
//	//copies only linked list returns a copy of that linkedlist
//
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
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//typename HashMap<KEY,T,thash>::LN** HashMap<KEY,T,thash>::copy_hash_table (LN** ht, int bins) const {
//	LN** new_map = new LN*[bins];
//
//	for (int x = 0; x < bins; x++){
//		LN* valz = ht[x];
//		new_map[x] = copy_list(valz); //copies the linkedlist values = copy_list?
//
//	}
//	return new_map;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//void HashMap<KEY,T,thash>::ensure_load_threshold(int new_used) {
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
//			LN* old_map_pt = map[x];
//
//
//				while (old_map_pt != nullptr && old_map_pt->next != nullptr)
//				{
//					LN* prev = old_map_pt;
//					old_map_pt = old_map_pt->next;
//					LN*& current_bucket = new_map[hash_compress(prev->value.first)];
//					prev->next = current_bucket;
//					current_bucket = prev;
//				}
//		}
//
//		delete[] map;
//		map = new_map;
//	}
//
//
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//void HashMap<KEY,T,thash>::delete_hash_table (LN**& ht, int bins) {
//
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
//template<class KEY,class T, int (*thash)(const KEY& a)>
//void HashMap<KEY,T,thash>::Iterator::advance_cursors(){
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
//		if(current.first == (ref_map->bins-1)){
//			current.first = -1;
//			current.second = nullptr;
//			can_erase = false;
//			return;
//		}
//		current.first++;
//		current.second = ref_map->map[current.first];
//	}
//
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::Iterator::Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin)
//: ref_map(iterate_over), expected_mod_count(ref_map->mod_count) {
//
//	if (from_begin)
//	{
//		current.first = 0;
//		current.second = ref_map->map[0];
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
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//HashMap<KEY,T,thash>::Iterator::~Iterator()
//{}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//auto HashMap<KEY,T,thash>::Iterator::erase() -> Entry {
//	  if (expected_mod_count != ref_map->mod_count)
//		    throw ConcurrentModificationError("HashMap::Iterator::erase");
//		  if (!can_erase)
//		    throw CannotEraseError("HashMap::Iterator::erase Iterator cursor already erased");
//		  if (current.second == nullptr)
//		    throw CannotEraseError("HashMap::Iterator::erase Iterator cursor beyond data structure");
//
//		  can_erase = false;
//		  Entry to_return = current.second->value;
//		  ref_map -> erase(to_return.first);
//		  advance_cursors();
//		  expected_mod_count = ref_map->mod_count;
//		  return to_return;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//std::string HashMap<KEY,T,thash>::Iterator::str() const {
//	std::ostringstream answer;
//	answer << ref_map->str()  << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
//	return answer.str();
//}
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//auto  HashMap<KEY,T,thash>::Iterator::operator ++ () -> HashMap<KEY,T,thash>::Iterator& {
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator ++");
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
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//auto  HashMap<KEY,T,thash>::Iterator::operator ++ (int) -> HashMap<KEY,T,thash>::Iterator {
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator ++(int)");
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
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::Iterator::operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const {
//	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("HashMap::Iterator::operator ==");
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator ==");
//	  if (ref_map != rhsASI->ref_map)
//	    throw ComparingDifferentIteratorsError("HashMap::Iterator::operator ==");
//
//	  return current == rhsASI->current;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//bool HashMap<KEY,T,thash>::Iterator::operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const {
//	  const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
//	  if (rhsASI == 0)
//	    throw IteratorTypeError("HashMap::Iterator::operator !=");
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator !=");
//	  if (ref_map != rhsASI->ref_map)
//	    throw ComparingDifferentIteratorsError("HashMap::Iterator::operator !=");
//
//	  return current != rhsASI->current;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//pair<KEY,T>& HashMap<KEY,T,thash>::Iterator::operator *() const {
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator *");
//	  if (!can_erase || current.first < 0 || current.second == nullptr) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_map->size();
//	    throw IteratorPositionIllegal("HashMap::Iterator::operator * Iterator illegal: "+where.str());
//	  }
////	  std::cout << "before current.second" << std::endl;
////	  std::cout << current.second->value << std::endl;
//	  return current.second->value;
//}
//
//
//template<class KEY,class T, int (*thash)(const KEY& a)>
//pair<KEY,T>* HashMap<KEY,T,thash>::Iterator::operator ->() const {
//	  if (expected_mod_count != ref_map->mod_count)
//	    throw ConcurrentModificationError("HashMap::Iterator::operator ->");
//	  if (!can_erase || current.second == nullptr) {
//	    std::ostringstream where;
//	    where << current << " when size = " << ref_map->size();
//	    throw IteratorPositionIllegal("HashMap::Iterator::operator -> Iterator illegal: "+where.str());
//	  }
//
//	  return &current.second->value;
//	}
//}
//
//#endif /* HASH_MAP_HPP_ */
