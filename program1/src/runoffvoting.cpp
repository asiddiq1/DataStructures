//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <limits>                    //Biggest int: std::numeric_limits<int>::max()
//#include "ics46goody.hpp"
//#include "array_queue.hpp"
//#include "array_priority_queue.hpp"
//#include "array_set.hpp"
//#include "array_map.hpp"
//
//
//typedef ics::ArrayQueue<std::string>              CandidateQueue;
//typedef ics::ArraySet<std::string>                CandidateSet;
//typedef ics::ArrayMap<std::string,int>            CandidateTally;
//
//typedef ics::ArrayMap<std::string,CandidateQueue> Preferences;
//typedef ics::pair<std::string,CandidateQueue>     PreferencesEntry;
//typedef ics::ArrayPriorityQueue<PreferencesEntry> PreferencesEntryPQ; //Must supply gt at construction
//
//typedef ics::pair<std::string,int>                TallyEntry;
//typedef ics::ArrayPriorityQueue<TallyEntry>       TallyEntryPQ;
//
//
//
////Read an open file stating voter preferences (each line is (a) a voter
////  followed by (b) all the candidates the voter would vote for, in
////  preference order (from most to least preferred candidate, separated
////  by semicolons), and return a Map of preferences: a Map whose keys are
////  voter names and whose values are a queue of candidate preferences.
//
//bool sort_string(const PreferencesEntry& a, const PreferencesEntry& b)
//{
//	return a.first < b.first;
//}
//
//
//
//Preferences read_voter_preferences(std::ifstream &file) {
//	Preferences pref;
//	std::string line;
//	while (getline(file, line))
//	{
//		std::vector<std::string> words = ics::split(line, ";");
//		for (int x = 1; x < static_cast<int>(words.size()); x++)
//			{
//				pref[words.at(0)].enqueue(words.at(x));
//			}
//
//		}
//
//		return pref;
//}
//
//
////Print a label and all the entries in the preferences Map, in alphabetical
////  order according to the voter.
////Use a "->" to separate the voter name from the Queue of candidates.
//void print_voter_preferences(const Preferences& preferences) {
//
//	std::cout << "Voter Preferences" << std::endl;
//	PreferencesEntryPQ sort_q(sort_string);
//	sort_q.enqueue_all(preferences);
//	for (auto& kv: sort_q)
//	{
//		std::cout << kv.first << " -> " << kv.second << std::endl;
//	}
//
//}
//
//
////Print the message followed by all the entries in the CandidateTally, in
////  the order specified by has_higher_priority: i is printed before j, if
////  has_higher_priority(i,j) returns true: sometimes alphabetically by candidate,
////  other times by decreasing votes for the candidate.
////Use a "->" to separate the candidat name from the number of votes they
////  received.
//void print_tally(std::string message, const CandidateTally& tally, bool (*has_higher_priority)(const TallyEntry& i,const TallyEntry& j)) {
//	std::cout << message << std::endl;
//	for (const TallyEntry& kv : TallyEntryPQ(tally,has_higher_priority))
//	{
//	    std::cout << "  " << kv.first << " --> " << kv.second << std::endl;
//	}
//
//}
//
//
////Return the CandidateTally: a Map of candidates (as keys) and the number of
////  votes they received, based on the unchanging Preferences (read from the
////  file) and the candidates who are currently still in the election (which changes).
////Every possible candidate should appear as a key in the resulting tally.
////Each voter should tally one vote: for their highest-ranked candidate who is
////  still in the the election.
//CandidateTally evaluate_ballot(const Preferences& preferences, const CandidateSet& candidates) {
//
//	CandidateTally ballot_map;
//
////	for (auto& cans: candidates)
////	{
////		if (!ballot_map.has_key(cans))
////			{
////			int count = 0;
////			ballot_map[cans] = count;
////
////			}
////	}
//
//		for (auto& i: preferences)
//		{
//
//			for (auto& x: i.second){
//				if (candidates.contains(x))
//				{
//					ballot_map[x] += 1;
//					break;
//				}
//
//			}
//		}
//	return ballot_map;
//
//}
//
//
////Return the Set of candidates who are still in the election, based on the
////  tally of votes: compute the minimum number of votes and return a Set of
////  all candidates receiving more than that minimum; if all candidates
////  receive the same number of votes (that would be the minimum), the empty
////  Set is returned.
//CandidateSet remaining_candidates(const CandidateTally& tally) {
//
//	//CandidateQueue check_empty;
//	CandidateSet remaining_set;
//	int minimum = std::numeric_limits<int>::max();
//	for (auto& i: tally)
//	{
//		if (i.second < minimum)
//		{
//			minimum = i.second;
//		}
//	}
//
//	for (auto& i: tally)
//		{
//			if (i.second > minimum)
//			{
//				remaining_set.insert(i.first);
//			}
//		}
//	return remaining_set;
//
//
//}
//
//
////Prompt the user for a file, create a voter preference Map, and print it.
////Determine the Set of all the candidates in the election, from this Map.
////Repeatedly evaluate the ballot based on the candidates (still) in the
////  election, printing the vote count (tally) two ways: with the candidates
////  (a) shown alphabetically increasing and (b) shown with the vote count
////  decreasing (candidates with equal vote counts are shown alphabetically
////  increasing); from this tally, compute which candidates remain in the
////  election: all candidates receiving more than the minimum number of votes;
////  continue this process until there are less than 2 candidates.
////Print the final result: there may 1 candidate left (the winner) or 0 left
////   (no winner).
//int main() {
//  try {
//	  std::ifstream text_file;
//	  std::string file_name;
//	  ics::safe_open(text_file,"Enter the name of a file with a graph", "votepref1.txt");
//	  Preferences pref = read_voter_preferences(text_file);
//	  print_voter_preferences(pref);
//	  CandidateSet candidate_set;
//
//	  for (auto& kv: pref)
//	  {
//		  for (auto& cans: kv.second)
//		  {
//			  candidate_set.insert(cans);
//		  }
//	  }
//
//
//	  int ballot_count = 1;
//	  while (candidate_set.size() > 1)
//	  {
//		  CandidateTally tally = evaluate_ballot(pref, candidate_set);
//
//		  std::stringstream ss1;
//		  std::stringstream ss2;
//		  ss1 << ballot_count;
//		  ss2 << candidate_set;
//		  std::string message1 = "Vote count on ballot #" + ss1.str() + " with candidates in alphabetical order: "
//				  "still in election = "+ss2.str();
//
//		  print_tally(message1, tally, [](const TallyEntry& i,const TallyEntry& j){return i.second < j.second;});
//
//		  std::string message2 = "Vote count on ballot #" + ss1.str() + " with candidates in numerical order: "
//		  				  "still in election = "+ss2.str();
//
//		  print_tally(message1, tally, [](const TallyEntry& i,const TallyEntry& j){return i.second > j.second;});
//
//		  candidate_set = remaining_candidates(tally);
//		  ballot_count++;
//
//	  }
//
//	  if (candidate_set.size() == 1)
//	  {
//		  std::string winner = *candidate_set.begin();
//		  std::cout << "Winner is " << winner << std::endl;
//	  }
//	  else
//	  {
//		  std::cout << "No Winner" << std::endl;
//	  }
//
//  } catch (ics::IcsError& e) {
//    std::cout << e.what() << std::endl;
//  }
//  return 0;
//}
