//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include "ics46goody.hpp"
//#include "array_queue.hpp"
//#include "array_priority_queue.hpp"
//#include "array_set.hpp"
//#include "array_map.hpp"
//
//
//typedef ics::ArrayQueue<std::string>                InputsQueue;
//typedef ics::ArrayMap<std::string,std::string>      InputStateMap;
//
//typedef ics::ArrayMap<std::string,InputStateMap>    FA;
//typedef ics::pair<std::string,InputStateMap>        FAEntry;
//
//bool gt_FAEntry (const FAEntry& a, const FAEntry& b)
//{return a.first<b.first;}
//
//typedef ics::ArrayPriorityQueue<FAEntry,gt_FAEntry> FAPQ;
//
//typedef ics::pair<std::string,std::string>          Transition;
//typedef ics::ArrayQueue<Transition>                 TransitionQueue;
//
//
////Read an open file describing the finite automaton (each line starts with
////  a state name followed by pairs of transitions from that state: (input
////  followed by new state, all separated by semicolons), and return a Map
////  whose keys are states and whose associated values are another Map with
////  each input in that state (keys) and the resulting state it leads to.
//const FA read_fa(std::ifstream &file) {
//	FA state_transition;
//	std::string line;
//	while (getline(file, line))
//	{
//		std::vector<std::string> words = ics::split(line, ";");
//		for (int x = 1; x < static_cast<int>(words.size()); x++)
//			{
//				state_transition[words.at(0)].put(words.at(x), words.at(x+1));
//				x++;
//			}
//
//
//		}
//
//		return state_transition;
//
//}
//
//
//
//
////Print a label and all the entries in the finite automaton Map, in
////  alphabetical order of the states: each line has a state, the text
////  "transitions:" and the Map of its transitions.
//void print_fa(const FA& fa) {
//	std::cout << "Finite Automaton Description" << std::endl;
//	for (auto& kv: fa)
//	{
//		std::cout << kv.first << " transitions: " << kv.second << std::endl;
//	}
//
//}
//
//
////Return a queue of the calculated transition pairs, based on the finite
////  automaton, initial state, and queue of inputs; each pair in the returned
////  queue is of the form: input, new state.
////The first pair contains "" as the input and the initial state.
////If any input i is illegal (does not lead to a state in the finite
////  automaton), then the last pair in the returned queue is i,"None".
//TransitionQueue process(const FA& fa, std::string state, const InputsQueue& inputs) {
//
//	TransitionQueue transition_pair;
//	transition_pair.enqueue(Transition("", state));
//	std::string current_state = state;
//
//	for (auto input: inputs)
//	{
//		for (auto kv: fa[current_state])
//		{
//			if (!fa[current_state].has_key(input))
//			{
//				transition_pair.enqueue(Transition(input, "None"));
//				break;
//			}
//			if (kv.first == input)
//			{
//				transition_pair.enqueue(Transition(input, kv.second));
//				current_state = kv.second;
//
//			}
//
//		}
//
//	}
//	return transition_pair;
//
//}
//
//
////Print a TransitionQueue (the result of calling the process function above)
//// in a nice form.
////Print the Start state on the first line; then print each input and the
////  resulting new state (or "illegal input: terminated", if the state is
////  "None") indented on subsequent lines; on the last line, print the Stop
////  state (which may be "None").
//void interpret(TransitionQueue& tq) {  //or TransitionQueue or TransitionQueue&&
//	Transition trans_pair = tq.dequeue();
//	std::cout << "Start state = " << trans_pair.second << std::endl;
//
//	while (!tq.empty())
//	{
//		trans_pair = tq.dequeue();
//		std::cout << "Input= " << trans_pair.first << " new_state= " << trans_pair.second << std::endl;
//
//	}
//	std::cout << "Stop state = " << trans_pair.second << std::endl;
//}
//
//
//
////Prompt the user for a file, create a finite automaton Map, and print it.
////Prompt the user for a file containing any number of simulation descriptions
////  for the finite automaton to process, one description per line; each
////  description contains a start state followed by its inputs, all separated by
////  semicolons.
////Repeatedly read a description, print that description, put each input in a
////  Queue, process the Queue and print the results in a nice form.
//int main() {
//  try {
//	  std::ifstream text_file;
//	  //	  std::string file_name;
//	  ics::safe_open(text_file,"Enter the name of a file with a Finite Automaton", "faparity.txt");
//	  FA finite_state = read_fa(text_file);
//	  print_fa(finite_state);
//
//	  std::ifstream text_file2;
//	  ics::safe_open(text_file2,"Enter the name of a file with the start-state and input", "fainputparity.txt");
//	  std::string line;
//
//	  while (getline(text_file2, line))
//		{
//		  	std::cout << "Starting new simulation with description:" << line << std::endl;
//			std::vector<std::string> words = ics::split(line, ";");
//			InputsQueue input_q;
//			for (int x = 1; x < static_cast<int>(words.size()); x++)
//				{
//					input_q.enqueue(words.at(x));
//				}
//		std::string start_state = words.at(0);
//		TransitionQueue tq = process(finite_state, start_state, input_q);
//		interpret(tq);
//		}
//
//  } catch (ics::IcsError& e) {
//    std::cout << e.what() << std::endl;
//  }
//
//  return 0;
//}
