//// Submitter: siddiqaa(Siddiq, Aisha)

#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "hash_graph.hpp"
#include "dijkstra.hpp"



std::string get_node_in_graph(const ics::DistGraph& g, std::string prompt, bool allow_QUIT) {
  std::string node;
  for(;;) {
    node = ics::prompt_string(prompt + " (must be in graph" + (allow_QUIT ? " or QUIT" : "") + ")");
    if ((allow_QUIT && node == "QUIT") || g.has_node(node))
      break;
  }
  return node;
}


int main() {
  try {
	  	ics::HashGraph<int> g;
	  	std::string fn;

	  	fn = ics::prompt_string("Enter graph file name", "flightcost.txt");
	  	std::ifstream file(fn);
	  	g.load(file, ";");
	  	std::cout << g << std::endl;

	  	ics::CostMap a_map = extended_dijkstra(g, get_node_in_graph(g, "Enter start node", false));
	  	std::cout << a_map << std::endl;

	  	std::string pause = get_node_in_graph(g, "Enter stop node", true);
	  	while(pause != "QUIT")
	  	{
	  		ics::ArrayQueue<std::string> n1 = ics::recover_path(a_map, pause);
	  		std::cout << "cost is " << a_map[pause].cost << "; path is " << n1 << std::endl;
	  		pause = get_node_in_graph(g, "Enter stop node", true);
	  	}

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
