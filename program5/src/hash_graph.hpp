//// Submitter: siddiqaa(Siddiq, Aisha)

#ifndef HASH_GRAPH_HPP_
#define HASH_GRAPH_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"
#include "heap_priority_queue.hpp"
#include "hash_set.hpp"
#include "hash_map.hpp"


namespace ics {


template<class T>
class HashGraph {
	//Forward declaration: used in templated typedefs below
	private:
		class LocalInfo;

	public:
		//Typedefs
    typedef std::string                NodeName;
    typedef pair<NodeName, NodeName>   Edge;
    typedef pair<NodeName, LocalInfo>  NodeLocalEntry;

	private:
    //Static methods for hashing (in the maps) and for printing in alphabetic
    //  order the nodes in a graph (see << for HashGraph<T>)
		static int hash_str(const NodeName& s) {
		  std::hash<std::string> str_hash;
		  return str_hash(s);
		}

		static int hash_pair_str(const Edge& s) {
		  std::hash<std::string> str_hash;
		  return str_hash(s.first)*str_hash(s.second);
		}

    static bool LocalInfo_gt(const NodeLocalEntry& a, const NodeLocalEntry& b)
    {return a.first < b.first;}

	public:
    //Typedefs continued (after private functions using earlier typedefs)
    typedef HashMap<NodeName, LocalInfo, hash_str>  NodeMap;
    typedef HashMap<Edge, T, hash_pair_str>         EdgeMap;
    typedef pair<NodeName, LocalInfo>               NodeMapEntry;
    typedef pair<Edge, T>                           EdgeMapEntry;

    typedef HashSet<NodeName, hash_str>             NodeSet;
    typedef HashSet<Edge, hash_pair_str>            EdgeSet;


    //Destructor/Constructors
    ~HashGraph();
		HashGraph();
		HashGraph(const HashGraph<T>& g);

    //Queries
    bool empty      ()                                     const;
    int  node_count ()                                     const;
    int  edge_count ()                                     const;
    bool has_node  (std::string node_name)                 const;
    bool has_edge  (NodeName origin, NodeName destination) const;
    T    edge_value(NodeName origin, NodeName destination) const;
    int  in_degree (NodeName node_name)                    const;
    int  out_degree(NodeName node_name)                    const;
    int  degree    (NodeName node_name)                    const;

    const NodeMap& all_nodes()                   const;
    const EdgeMap& all_edges()                   const;
    const NodeSet& out_nodes(NodeName node_name) const;
    const NodeSet& in_nodes (NodeName node_name) const;
    const EdgeSet& out_edges(NodeName node_name) const;
    const EdgeSet& in_edges (NodeName node_name) const;

    //Commands
    void add_node   (NodeName node_name);
		void add_edge   (NodeName origin, NodeName destination, T value);
		void remove_node(NodeName node_name);
		void remove_edge(NodeName origin, NodeName destination);
		void clear      ();
		void load       (std::ifstream& in_file,  std::string separator = ";");
		void store      (std::ofstream& out_file, std::string separator = ";");

		//Operators
		HashGraph<T>& operator = (const HashGraph<T>& rhs);
		bool operator == (const HashGraph<T>& rhs) const;
		bool operator != (const HashGraph<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator<<(std::ostream& outs, const HashGraph<T2>& g);


	private:
		//All methods and operators relating to LocalInfo are defined below, followed by
		//  a friend function for << onto output streams for LocalInfo
		class LocalInfo {
			public:
				LocalInfo() {}
				LocalInfo(HashGraph<T>* g) : from_graph(g) {}
				void connect(HashGraph<T>* g) {from_graph = g;}
				bool operator == (const LocalInfo& rhs) const {
				  //No need to check in_nodes and _out_nodes: redundant information there
					return this->in_edges == rhs.in_edges && this->out_edges == rhs.out_edges;
				}
				bool operator != (const LocalInfo& rhs) const {
					return !(*this == rhs);
				}

		    //LocalInfor instance variables
				//The LocalInfo class is private to code #including this file, but public
				//  instance variables allows HashGraph them directly
				//from_graph should point to the HashGraph of the LocalInfo it is in, so
				//  LocalInfo methods can access its edge_value map (see <<)
				HashGraph<T>* from_graph = nullptr;
				NodeSet       out_nodes;
				NodeSet       in_nodes;
				EdgeSet       out_edges;
				EdgeSet       in_edges;
		};


		friend std::ostream& operator<<(std::ostream& outs, const LocalInfo& li) {
			outs << "LocalInfo[" << std::endl << "         out_nodes = " << li.out_nodes << std::endl;
			outs << "         out_edges = set[";
			int printed = 0;
			for (Edge e : li.out_edges)
        outs << (printed++ == 0 ? "" : ",") << "->" << e.second << "(" << li.from_graph->edge_values[e] << ")";
			outs << "]" << std::endl;

			outs << "         in_nodes  = " << li.in_nodes << std::endl;
			outs << "         in_edges  = set[";
      printed = 0;
      for (Edge e : li.in_edges)
        outs << (printed++ == 0 ? "" : ",") << e.first << "(" << li.from_graph->edge_values[e] << ")" << "->" ;

      outs << "]]";
			return outs;
		}

		//HashGraph<T> class instance variables
		NodeMap node_values;
		EdgeMap edge_values;
};




////////////////////////////////////////////////////////////////////////////////
//
//HashGraph class and related definitions

//Destructor/Constructors

template<class T>
HashGraph<T>::~HashGraph ()
{

}


template<class T>
HashGraph<T>::HashGraph ()
{}


//Copy all nodes and edges from g
template<class T>
HashGraph<T>::HashGraph (const HashGraph& g) {

	node_values = g.node_values;
	edge_values = g.edge_values;

}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

//Returns whether a graph is empty
template<class T>
bool HashGraph<T>::empty() const {
	return (node_values.empty());

}


//Returns the number of nodes in a graph
template<class T>
int HashGraph<T>::node_count() const {
	return (node_values.size());



}


//Returns the number of edges in a graph
template<class T>
int HashGraph<T>::edge_count() const {
	return (edge_values.size());


}


//Returns whether or not node_name is in the graph
template<class T>
bool HashGraph<T>::has_node(NodeName node_name) const {
	return (node_values.has_key(node_name));


}

//Returns whether or not the edge is in the graph
template<class T>
bool HashGraph<T>::has_edge(NodeName origin, NodeName destination) const {
	return(edge_values.has_key(Edge(origin, destination)));


}


//Returns the value of the edge in the graph; if the edge is not in the graph,
//  throw a GraphError exception with appropriate descriptive text
template<class T>
T HashGraph<T>::edge_value(NodeName origin, NodeName destination) const {
	ics::pair<NodeName, NodeName> val(origin, destination);
	if (!has_edge(origin, destination))
	{
		throw GraphError("Does not contain edge");
	}
	else
	{
		return(edge_values[Edge(origin, destination)]);

	}
}


//Returns the in-degree of node_name; if that node is not in the graph,
//  throw a GraphError exception with appropriate descriptive text
template<class T>
int HashGraph<T>::in_degree(NodeName node_name) const {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		return node_values[node_name].in_edges.size();

	}




}


//Returns the out-degree of node_name; if that node is not in the graph,
//  throw a GraphError exception with appropriate descriptive text
template<class T>
int HashGraph<T>::out_degree(NodeName node_name) const {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{

		return node_values[node_name].out_edges.size();
	}

}


//Returns the degree of node_name; if that node is not in the graph,
//  throw a GraphError exception with appropriate descriptive text.
template<class T>
int HashGraph<T>::degree(NodeName node_name) const {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		int in_deg = in_degree(node_name);
		int out_deg = out_degree(node_name);
		int total = in_deg + out_deg;
		return total;

	}


}


//Returns a reference to the all_nodes map;
//  the user should not mutate its data structure: call Graph commands instead
template<class T>
auto HashGraph<T>::all_nodes () const -> const NodeMap& {
	return node_values;

}


//Returns a reference to the all_edges map;
//  the user should not mutate its data structure: call Graph commands instead
template<class T>
auto HashGraph<T>::all_edges () const -> const EdgeMap& {
	return edge_values;
}

//Returns a reference to the out_nodes set in the LocalInfo for node_name;
//  the user should not mutate its data structure: call Graph commands instead;
//  if that node is not in the graph, throw a GraphError exception with
//  appropriate  descriptive text
template<class T>
auto HashGraph<T>::out_nodes(NodeName node_name) const -> const NodeSet& {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		return node_values[node_name].out_nodes;

	}


}


//Returns a reference to the in_nodes set in the LocalInfo for node_name;
//  the user should not mutate its data structure: call Graph commands instead;
//  if that node is not in the graph, throw a GraphError exception with
//  appropriate descriptive text
template<class T>
auto HashGraph<T>::in_nodes(NodeName node_name) const -> const NodeSet& {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		return node_values[node_name].in_nodes;

	}

}


//Returns a reference to the out_edges set in the LocalInfo for node_name;
//  the user should not mutate its data structure: call Graph commands instead;
//  if that node is not in the graph, throw a GraphError exception with
//  appropriate descriptive text
template<class T>
auto HashGraph<T>::out_edges(NodeName node_name) const -> const EdgeSet& {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		return node_values[node_name].out_edges;

	}

}


//Returns a reference to the in_edges set in the LocalInfo for node_name;
//  the user should not mutate its data structure: call Graph commands instead;
//  if that node is not in the graph, throw a GraphError exception with
//  appropriate descriptive text
template<class T>
auto HashGraph<T>::in_edges(NodeName node_name) const -> const EdgeSet& {
	if (!has_node(node_name))
	{
		throw GraphError("Node not in the graph");
	}
	else
	{
		return node_values[node_name].in_edges;

	}



}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

//Add node_name to the graph if it is not already there.
//Ensure that its associated LocalInfo has a from_graph refers to this graph.
template<class T>
void HashGraph<T>::add_node (NodeName node_name) {
	if (!has_node(node_name))
	{
		node_values.put(node_name, LocalInfo(this));

	}


}


//Add an edge from origin node to destination node, with value
//Add these node names and update edge_values and the LocalInfos of each node
template<class T>
void HashGraph<T>::add_edge (NodeName origin, NodeName destination, T value) {
	edge_values.put(Edge(origin, destination), value);

	node_values[destination].in_edges.insert(Edge(origin, destination));
	node_values[origin].out_nodes.insert(destination);
	node_values[destination].in_nodes.insert(origin);
	node_values[origin].out_edges.insert(Edge(origin, destination));


}


//Remove all uses of node_name from the graph: update node_values, edge_values,
//  and all the LocalInfo in which it appears as an origin or destination node
//If the node_name is not in the graph, do nothing
//Hint: you cannot iterate over a sets that you are changing:, so you might have
// to copy a set and then iterate over it while removing values from the original set
template<class T>
void HashGraph<T>::remove_node (NodeName node_name){

	if (has_node(node_name))
	{
		auto in_set = in_nodes(node_name);
		for (auto in_node: in_set)
		{
			edge_values.erase(Edge(in_node, node_name));
			node_values[in_node].out_nodes.erase(node_name);
			node_values[in_node].out_edges.erase(Edge(in_node, node_name));
		}


		auto out_set = out_nodes(node_name);
		for (auto out_node: out_set)
		{
			edge_values.erase(Edge(node_name, out_node));
			node_values[out_node].in_nodes.erase(node_name);
			node_values[out_node].in_edges.erase(Edge(node_name, out_node));

		}

		node_values.erase(node_name);
	}

	return;


}


//Remove all uses of this edge from the graph: update edge_values and all the
//  LocalInfo in which its origin and destination node appears
//If the edge is not in the graph, do nothing
//Hint: Simpler than remove_node: write and test this one first
template<class T>
void HashGraph<T>::remove_edge (NodeName origin, NodeName destination) {

	if (has_edge(origin, destination))
	{
		node_values[origin].out_nodes.erase(destination);
		node_values[origin].out_edges.erase(Edge(origin, destination));
		node_values[destination].in_nodes.erase(origin);
		node_values[destination].in_edges.erase(Edge(origin, destination));
		edge_values.erase(Edge(origin, destination));}
}


//Clear the graph of all nodes and edges
template<class T>
void HashGraph<T>::clear() {
	edge_values.clear();
	node_values.clear();
}


//Load the nodes and edges for a graph from a text file whose form is
// (a) a node name (one per line)
// followed by
// (b) an origin node, destination node, and value (one triple per line,
//       with the values separated by separator, on any number of lines)
// Adds these nodes/edges to those currently in the graph
//Hint: use split and istringstream (the extraction dual of ostreamstring)
template<class T>
void HashGraph<T>::load (std::ifstream& in_file, std::string separator) {
	std::string line;

	while(getline(in_file, line))
	{
		if (line != "NODESABOVEEDGESBELOW")
		{
			add_node(line);

		}
		else
		{
			break;
		}
	}
	while(getline(in_file, line))
	{
		std::vector<std::string> words = ics::split(line, separator);
		std::istringstream t_value(words[2]);
		T val;
		t_value >> val;
		add_edge(words[0], words[1], val);
	}



}


//Store the nodes and edges in a graph into a text file whose form is specified
//  above for the load method; files written by store should be readable by load
//Hint: this is the easier of the two methods: write and test it first
template<class T>
void HashGraph<T>::store(std::ofstream& out_file, std::string separator) {

	for (auto nodes: all_nodes())
	{
		out_file << nodes.first << "\n";
	}
	out_file << "NODESABOVEEDGESBELOW" << std::endl;
	for (auto edges: all_edges())
	{
		out_file << edges.first.first << separator
				<< edges.first.second << separator << edges.second << "\n";

	}
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

//Copy the specified graph into this and return the newly copied graph
//Hint: each copied LocalInfo object should reset from_graph to the this new graph
template<class T>
HashGraph<T>& HashGraph<T>::operator = (const HashGraph<T>& rhs){
	if(*this == rhs)
			return *this;

		clear();
		edge_values = rhs.edge_values;
		node_values = rhs.node_values;
		for(auto np : node_values)
		{
			np.second.connect(this);
		}

		return *this;


}


//Return whether two graphs are the same nodes and same edges
//Avoid checking == on LocalInfo (edge_map has equivalent information;
//  just check that node names are the same in each
template<class T>
bool HashGraph<T>::operator == (const HashGraph<T>& rhs) const{

	if(node_values.size() != rhs.node_values.size() || edge_values != rhs.edge_values)
	{return false;
	}

	for(auto np : rhs.all_nodes())
	{
		if(!has_node(np.first))
		{
				return false;
		}
	}

	return true;


}


//Return whether two graphs are different
template<class T>
bool HashGraph<T>::operator != (const HashGraph<T>& rhs) const{
	return !(*this == rhs);}


bool sort_b(const std::string& s1, const std::string& s2)
{
	return s1 < s2;
}



template<class T>
std::ostream& operator<<(std::ostream& outs, const HashGraph<T>& g) {
	outs << "graph[";
	HeapPriorityQueue<std::string> sort_a(&sort_b);
	for(auto np : g.node_values)
	{
		sort_a.enqueue(np.first);
	}

	while(!sort_a.empty())
	{
		std::string n = sort_a.dequeue();
		outs << "\n" << n << " -> " << g.node_values[n];
	}

	outs << "]";
	return outs;
}



}

#endif /* HASH_GRAPH_HPP_ */
