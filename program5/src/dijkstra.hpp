//// Submitter: siddiqaa(Siddiq, Aisha)

#ifndef DIJKSTRA_HPP_
#define DIJKSTRA_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
#include "array_queue.hpp"
#include "array_stack.hpp"
#include "heap_priority_queue.hpp"
#include "hash_graph.hpp"


namespace ics {


class Info {
	public:
  Info() {}
  Info(std::string a_node) : node(a_node) {}

		bool operator == (const Info& rhs) const
		{return cost == rhs.cost && from == rhs.from;}

		bool operator !=( const Info& rhs) const
    {return !(*this == rhs);}

		friend std::ostream& operator<<(std::ostream& outs, const Info& i) {
		  outs << "Info[" << i.cost << "," << i.from << "]";
			return outs;
		}

		//Public instance variable definitions
    std::string node = "?";
    int         cost = std::numeric_limits<int>::max();
    std::string from = "?";
};


bool gt_info (const Info& a, const Info& b) {return a.cost < b.cost;}

typedef ics::HashGraph<int>                                  DistGraph;
typedef ics::HeapPriorityQueue<Info, gt_info>                CostPQ;
typedef ics::HashMap<std::string, Info, DistGraph::hash_str> CostMap;
typedef ics::pair<std::string, Info>                         CostMapEntry;


//Return the final_map as specified in the lecture-node description of
//  extended Dijkstra algorithm
CostMap extended_dijkstra(const DistGraph& g, std::string start_node) {
	CostMap answer;
	CostMap info;
	CostPQ info_pq;

	for (auto insert_vals: g.all_nodes())
	{
		info.put(insert_vals.first, Info(insert_vals.first));
	}

	info[start_node].cost = 0;

	for (auto info_contents: info)
	{
		info_pq.enqueue(info_contents.second);
	}

	while (!info.empty())
	{
		Info smallest_cost = info_pq.dequeue();
		if (smallest_cost.cost == std::numeric_limits<int>::max())
		{
			break;}
		if (answer.has_key(smallest_cost.node))
		{
			continue;
		}

		std::string m_node = smallest_cost.node;
		int m_cost = smallest_cost.cost;

		info.erase(m_node);
		answer.put(m_node, smallest_cost);

		for (auto n: g.out_nodes(m_node))
		{
			if(!answer.has_key(n))
			{
				if (info[n].cost >= std::numeric_limits<int>::max() || info[n].cost > m_cost + g.edge_value(m_node, n))
				{
					info[n].cost = m_cost + g.edge_value(m_node, n);
					info[n].from = m_node;
					info_pq.enqueue(info[n]);
				}
			}

		}

	}

	return answer;






}


//Return a queue whose front is the start node (implicit in answer_map) and whose
//  rear is the end node
ArrayQueue<std::string> recover_path(const CostMap& answer_map, std::string end_node) {
	ArrayQueue<std::string> q;
	ArrayStack<std::string> s;
	std::string n = end_node;
	while(answer_map[n].cost > 0)
	{
		s.push(answer_map[n].node);
		n = answer_map[n].from;

	}

	while(!s.empty())
	{
		auto item = s.pop();
		q.enqueue(item);
	}
	return q;



}


}

#endif /* DIJKSTRA_HPP_ */
