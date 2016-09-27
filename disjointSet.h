//! \file disjointSet.h - definition and implementation of disjointSet class
#ifndef DISJOINTSET_H_
#define DISJOINTSET_H_

#include <vector>
#include <iterator>

using namespace std;
class disjointSet{
public:
	/*! \fn disjointSet (vector<int> t)
 		*  \brief A constructor function for disjointSet class.
 		*  \param int type vector
 		*/
	disjointSet (vector<int> t)
	{
		for (int x = 0; x < t.size(); ++x) {
			nodes.push_back(t[x]);
		}
		
		/*cout << "in constructor: ";
		for (int x=0; x < t.size(); ++x) {
			cout << t[x] << " ";
		}
		cout << "\n";*/
		
	}
	
	/*! \fn ~disjointSet ()
 		*  \brief A destructor function for disjointSet class.
 		*/
	~disjointSet ()
	{
		nodes.clear();
		setIndex.clear();
	}
	
	/*! \fn void printDisjointSet()
 		*  \brief A function to print disjointSet object.
 		*/
	void printDisjointSet()
	{
		cout << "disjoint nodes: ";
		for (int x=0; x < nodes.size(); ++x) {
			cout << nodes[x] << " ";
		}
		cout << "\n";
		
		cout << "partitions: ";
		for (int x=0; x < setIndex.size(); ++x) {
			cout << setIndex[x] << " ";
		}
		cout << "\n";
	}
	
	vector<int> nodes; //!< A public member variable
	vector<int> setIndex; //!< A public member variable
	
};

#endif
