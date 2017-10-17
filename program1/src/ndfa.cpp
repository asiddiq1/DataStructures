#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "ics46goody.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_set.hpp"
#include "array_map.hpp"


typedef ics::ArraySet<std::string>                     States;
typedef ics::ArrayQueue<std::string>                   InputsQueue;
typedef ics::ArrayMap<std::string,States>              InputStatesMap;

typedef ics::ArrayMap<std::string,InputStatesMap>       NDFA;
typedef ics::pair<std::string,InputStatesMap>           NDFAEntry;

bool gt_NDFAEntry (const NDFAEntry& a, const NDFAEntry& b)
{return a.first<b.first;}

typedef ics::ArrayPriorityQueue<NDFAEntry,gt_NDFAEntry> NDFAPQ;

typedef ics::pair<std::string,States>                   Transitions;
typedef ics::ArrayQueue<Transitions>                    TransitionsQueue;


//Read an open file describing the non-deterministic finite automaton (each
//  line starts with a state name followed by pairs of transitions from that
//  state: (input followed by a new state, all separated by semicolons), and
//  return a Map whose keys are states and whose associated values are another
//  Map with each input in that state (keys) and the resulting set of states it
//  can lead to.


const NDFA read_ndfa(std::ifstream &file) {
	std::string line;
	NDFA ndfa;
	while (std::getline(file, line))
	{

		std::vector<std::string> words = ics::split(line, ";");
		if (words.size() == 1)
		{
			ndfa[words.at(0)];
		}
		for (int x = 1; x < static_cast<int>(words.size()); x+=2)
		{
			if (!ndfa.has_key(words.at(0)))
			{
				InputStatesMap map_val;
				ndfa[words.at(0)] = map_val;

			}
			if (!ndfa[words.at(0)].has_key(words.at(x)))
			{
				States set_states;
				set_states.insert(words.at(x+1));
				ndfa[words.at(0)][words.at(x)] = set_states;
			}
			else
			{
				ndfa[words.at(0)][words.at(x)].insert(words.at(x+1));
			}
		}
	}
	return ndfa;
}


//Print a label and all the entries in the finite automaton Map, in
//  alphabetical order of the states: each line has a state, the text
//  "transitions:" and the Map of its transitions.
void print_ndfa(const NDFA& ndfa) {

	NDFAPQ ap(gt_NDFAEntry);
	ap.enqueue_all(ndfa);

	std::cout << "Non-Deterministic Finite Automaton Description" << std::endl;
	for (const auto kv: ap)
	{
		std::cout << kv.first << " transitions: " << kv.second << std::endl;
	}

}


//Return a queue of the calculated transition pairs, based on the non-deterministic
//  finite automaton, initial state, and queue of inputs; each pair in the returned
//  queue is of the form: input, set of new states.
//The first pair contains "" as the input and the initial state.
//If any input i is illegal (does not lead to any state in the non-deterministic finite
//  automaton), ignore it.
TransitionsQueue process(const NDFA& ndfa, std::string state, const InputsQueue& inputs) {
	TransitionsQueue tq;
	States state_explore;
	state_explore.insert(state);
	tq.enqueue(Transitions("", state_explore));
	States new_explore;

	for (auto input: inputs)
	{

		for (auto states: state_explore)
		{
			if (ndfa[states].has_key(input))
			{
				new_explore.insert_all(ndfa[states][input]);
			}
		}

		tq.enqueue(Transitions(input, new_explore));
		state_explore = new_explore;
		new_explore.clear();
	}
	return tq;
}


//Print a TransitionsQueue (the result of calling process) in a nice form.
//Print the Start state on the first line; then print each input and the
//  resulting new states indented on subsequent lines; on the last line, print
//  the Stop state.
void interpret(TransitionsQueue& tq) {  //or TransitionsQueue or TransitionsQueue&&
		Transitions trans_pair = tq.dequeue();
		std::cout << "Start state = " << trans_pair.second << std::endl;

		while (!tq.empty())
		{
			trans_pair = tq.dequeue();
			std::cout << "Input= " << trans_pair.first << " new_state= " << trans_pair.second << std::endl;

		}
		std::cout << "Stop state(s) = " << trans_pair.second << std::endl;
	}


//Prompt the user for a file, create a finite automaton Map, and print it.
//Prompt the user for a file containing any number of simulation descriptions
//  for the finite automaton to process, one description per line; each
//  description contains a start state followed by its inputs, all separated by
//  semicolons.
//Repeatedly read a description, print that description, put each input in a
//  Queue, process the Queue and print the results in a nice form.
int main() {
  try {

	  std::ifstream text_file;
	  ics::safe_open(text_file,"Enter the name of a file with a Non-Deterministic Finite Automaton", "ndfaendin01.txt");
	  NDFA fa = read_ndfa(text_file);
	  print_ndfa(fa);

	  std::ifstream text_file2;
	  ics::safe_open(text_file2,"Enter the name of a file with the start-state and input", "ndfainputendin01.txt");
	  std::string line;

	  while (getline(text_file2, line))
	  {
		  std::cout << "Starting new simulation with description:" << line << std::endl;
		  std::vector<std::string> words = ics::split(line, ";");
		  InputsQueue input_q;
	  			for (int x = 1; x < static_cast<int>(words.size()); x++)
	  				{
	  					input_q.enqueue(words.at(x));
	  				}
	  		std::string start_state = words.at(0);
	  		TransitionsQueue tq = process(fa, start_state, input_q);
	  		interpret(tq);
	  		}



  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
