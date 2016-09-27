//! \file graphGraphlet.h - implementation of graph class, identify graphlets and generate neighbors list of current graphlet.

#ifndef GRAPH_H_
#define GRAPH_H_
//without tuple look up
#include <fstream>
#include <exception>
#include <iostream>
#include <iterator>
#include <set>
#include <map>
#include <string.h>
#include "StringTokenizer.h"
#include "random.h"
#include "disjointSet.h"
#include "graphlet.h" 
using namespace std;


typedef pair<int,int> EDGE;
map<string,int> discovered_graphlets;
map<string,int>::const_iterator dit;
int *fw = new int[2000000]; //!< A global int type array
int sfw;//!< A global integer
int *wx = new int[2000000];//!< A global int type array
int swx;//!< A global integer
int *xy = new int[2000000];//!< A global int type array
int sxy;//!< A global integer
int *yz =new int[2000000];//!< A global int type array
int syz;//!< A global integer
int *zf = new int[2000000];//!< A global int type array
int szf;//!< A global integer
int *yf = new int[2000000];//!< A global int type array
int syf;//!< A global integer
int *fx = new int[2000000];//!< A global int type array
int sfx;//!< A global integer

int *temp1 = new int[2000000];//!< A global int type array
int stemp1;//!< A global integer
int *temp2 = new int[2000000];//!< A global int type array
int stemp2;//!< A global integer

int *dfw = new int[2000000];//!< A global int type array
int dsfw;//!< A global integer
int *dwx = new int[2000000];//!< A global int type array
int dswx;//!< A global integer
int *dxy = new int[2000000];//!< A global int type array
int dsxy;//!< A global integer


struct prop_edge
{
	EDGE edge;
	bool validTC;
	int TC;//triangle count

	bool validDC;
	int DC;
};

struct prop_vertex
{
	int vertex;
	int adj_size;
	vector<int> adj;      //no order is maintained
};

struct cmpE{
	bool operator()(const EDGE& e1,const EDGE& e2)const{
		if(e1.first<e2.first){
			return true;
		}else if(e1.first>e2.first){
			return false;
		}else{
			if(e1.second<e2.second){
				return true;
			}
			return false;
		}
	}
};
class graph_{
public:
	typedef pair<int,int> EDGE;
private:
	map<int,double> symbolC;//!< A private member variable

	vector<prop_vertex> graphAdj;//!< A private member variable
	int vertexCount;//!< A private member variable

	map<int,int> vertexToIndex;//!< A private member variable
	bool vertexToIndexValid;//!< A private member variable

	vector<prop_edge> edge_list;//!< A private member variable
	int edgeCount;//!< A private member variable
	bool triCountDone;//!< A private member variable
	bool degreeCountDone;//!< A private member variable

	map<EDGE,int,cmpE> edgeToIndex;//!< A private member variable
	bool edgeToIndexValid;//!< A private member variable

	set<EDGE,cmpE> edgeSet;//!< A private member variable
	bool edgeSetValid;//!< A private member variable

	vector<int> edgeCW;//!< A private member variable
	bool edgeCWValid;//!< A private member variable

	EDGE current;//!< A private member variable

public:
	/*! \fn ~graph_()
 		*  \brief A destructor function of graph class.
 		*/
	~graph_(){
		for(int i=0;i<graphAdj.size();i++){
			graphAdj[i].adj.clear();
		}
		graphAdj.clear();
		vertexToIndex.clear();
		edge_list.clear();
		edgeToIndex.clear();
		edgeSet.clear();
		edgeCW.clear();
	}

	/*! \fn graph_(const char* filename)
 		*  \brief A constructor function of graph class.
 		*  \param filename char pointer
 		*/
    graph_(const char* filename) {
    	triCountDone=false;
    	degreeCountDone=false;
    	edgeCWValid=false;
    	edgeToIndexValid=false;
    	
    	current=make_edge(0,0);
    	vertexToIndexValid=false;

    	ifstream infile(filename, ios::in);
    	read_graph(infile);
    	vertexToIndexValid=true;
    	
    	initiateEdgeSet();
    }

	/*! \fn void prepare_variable()
 		*  \brief A function to build sorted adjacency list
 		*/
    void prepare_variable()
    {
    	int largest_v=0;
    	for(int i=0;i<edge_list.size();i++){
    		edge_list[i].validDC=false;
    		edge_list[i].validTC=false;
    		if(edge_list[i].edge.second>largest_v) largest_v=edge_list[i].edge.second;
    	}
    	vertexCount = largest_v+1;
    	edgeCount=edge_list.size();
    	//make adjacency list
    	int vIndex=0;
    	map<int,int>::iterator it;
    	for(int i=0;i<edge_list.size();i++)
       	{
    		it=vertexToIndex.find(edge_list[i].edge.first);
    		if(it == vertexToIndex.end()){
    			prop_vertex a;
    			graphAdj.push_back(a);
    			graphAdj[vIndex].vertex=edge_list[i].edge.first;
    			vertexToIndex.insert(pair<int,int>(edge_list[i].edge.first,vIndex));
    			vIndex++;
    		}
    		int ti=vertexToIndex.find(edge_list[i].edge.first)->second;
   			if(edge_list[i].edge.first!=graphAdj[ti].vertex) {
   				cout<<"error read_graph!!!\n";
   				exit(1);
   			}
   			graphAdj[ti].adj.push_back(edge_list[i].edge.second);

   			it=vertexToIndex.find(edge_list[i].edge.second);
   			if(it == vertexToIndex.end()){
   				prop_vertex a;
   				graphAdj.push_back(a);
   				graphAdj[vIndex].vertex=edge_list[i].edge.second;
   		    	vertexToIndex.insert(pair<int,int>(edge_list[i].edge.second,vIndex));
   		    	vIndex++;
   			}
   			ti=vertexToIndex.find(edge_list[i].edge.second)->second;
   			if(edge_list[i].edge.second!=graphAdj[ti].vertex) {
   				cout<<"error read_graph!!!\n";
   				exit(1);
   			}
   			graphAdj[ti].adj.push_back(edge_list[i].edge.first);
       	}
    	for(int i=0;i<graphAdj.size();i++)
    	{
       		sort(graphAdj[i].adj.begin(),graphAdj[i].adj.end()); //pre-sort the adjacency list...needed for intersection operation
       		graphAdj[i].adj_size=graphAdj[i].adj.size();//pre-compute the adjacency list size
    	}
    	
    }

		/*! \fn graphlet(int X,int Y,int Z)
 		*  \brief A function to read graph from input file where, v1\tv2\n is the data format.
    	 * Graph should be undirected. No duplicated edge must be present.
    	 * For best result, vertex labels should start from 0 and end to n.
    	 * All numbers between 0 to n should represent a valid node of graph.
    	 * File should end with a newline '\n'.
 		*  \param datafile an ifstream
 		*/
    void read_graph(ifstream& datafile)
    {

    	/*
    	 * v1\tv2\n
    	 * Graph should be undirected. No duplicated edge must be present.
    	 * For best result, vertex labels should start from 0 and end to n.
    	 * All numbers between 0 to n should represent a valid node of graph.
    	 * File should end with a newline '\n'
    	 */
    	std::string oneline;
    	int largest_v=0;

    	while(1)								//read all the edges in a vector
    	{
    		std::getline(datafile,oneline);
    		if (oneline.length() < 1) break;
    		StringTokenizer strtok = StringTokenizer(oneline,"\t");
    		int v1 = strtok.nextIntToken();
    		int v2 = strtok.nextIntToken();
    		if (v1>largest_v) largest_v=v1;
    		if (v2>largest_v) largest_v=v2;
    		prop_edge e;
    		e.edge= make_edge(v1,v2);
    		e.validTC=false;
    		e.validDC=false;
    		edge_list.push_back(e);
    	}
    	vertexCount = largest_v+1;
    	edgeCount = edge_list.size();
    	//make adjacency list
    	int vIndex=0;
    	map<int,int>::iterator it;
    	
    	for(int i=0;i<edge_list.size();i++)
    	{
    		it=vertexToIndex.find(edge_list[i].edge.first);
    		if(it == vertexToIndex.end()){
    			prop_vertex a;
    			graphAdj.push_back(a);
    			graphAdj[vIndex].vertex=edge_list[i].edge.first;
    			vertexToIndex.insert(pair<int,int>(edge_list[i].edge.first,vIndex));
    			vIndex++;
    		}
    		int ti=vertexToIndex.find(edge_list[i].edge.first)->second;
    		if(edge_list[i].edge.first!=graphAdj[ti].vertex) {
    			cout<<"error read_graph!!!\n";
    			exit(1);
    		}
    		graphAdj[ti].adj.push_back(edge_list[i].edge.second);

    		it=vertexToIndex.find(edge_list[i].edge.second);
    		if(it == vertexToIndex.end()){
       			prop_vertex a;
       			graphAdj.push_back(a);
       			graphAdj[vIndex].vertex=edge_list[i].edge.second;
    		    vertexToIndex.insert(pair<int,int>(edge_list[i].edge.second,vIndex));
    		    vIndex++;
    		}
    		ti=vertexToIndex.find(edge_list[i].edge.second)->second;
    		if(edge_list[i].edge.second!=graphAdj[ti].vertex) {
    			cout<<"error read_graph!!!\n";
       			exit(1);
    		}
    		graphAdj[ti].adj.push_back(edge_list[i].edge.first);

    	}
    	for(int i=0;i<graphAdj.size();i++)
    	{
    		sort(graphAdj[i].adj.begin(),graphAdj[i].adj.end()); //pre-sort the adjacency list...needed for intersection operation
    		graphAdj[i].adj_size=graphAdj[i].adj.size();//pre-compute the adjacency list size
    	}

		cout <<"Done loading input graph\n";
    }
    

    bool findEdge(int a, int b){

    	return edgeSet.find(make_edge(a,b)) != edgeSet.end();
    }

	/*! \fn bool new_findEdge(int a, int b)
 		*  \brief A function to identify where there exists any edge between a & b.
 		*  \param a an integer
		*  \param b an integer
		*  \return  boolean
 		*/
	bool new_findEdge(int a, int b)
	{
		//new way
		map<int,int>::const_iterator f;
		f = vertexToIndex.find(a);
		if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), b)!=graphAdj[f->second].adj.end())
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
 
	/*! \fn int degree_count(EDGE e)
 		*  \brief A function to count degree of an edge.
 		*  \param e an EDGE object
		*  \return  integer
 		*/
    int degree_count(EDGE e)
    {
    	map<int,int>::const_iterator f,s;
    	f=vertexToIndex.find(e.first);
       	s=vertexToIndex.find(e.second);
       	if(f == vertexToIndex.end() or s == vertexToIndex.end())
      	{
       		cout<<"the vertex does not exist\n";
      		exit(1);
      	}
       	if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), e.second)==graphAdj[f->second].adj.end()
       			or find(graphAdj[s->second].adj.begin(), graphAdj[s->second].adj.end(), e.first)==graphAdj[s->second].adj.end()){
    	   	print_edge(e);
     		cout<<"the edge does not exist\n";
     		exit(1);
    	}
       	return graphAdj[f->second].adj.size()+graphAdj[s->second].adj.size();
    }


    void precompute_all_DC()
    {
    	if(degreeCountDone==true) return;
    	for(int i=0;i<edge_list.size();i++)
    	{
       		if(edge_list[i].validDC==false){
       			edge_list[i].DC=degree_count(edge_list[i].edge);
    	   		edge_list[i].validDC=true;
     		}
    	}
    	degreeCountDone=true;
    }

/*! \fn  void initiateEdgeSet()
 		*  \brief A function to initialize EdgeSet.
 		*/
    void initiateEdgeSet()
    {
    	if(edgeSetValid==true) return;
    	for(int i=0;i<edge_list.size();i++){
      		edgeSet.insert(edge_list[i].edge);
    	}
    	edgeSetValid=true;
    }

	/*! \fn  void initiateEdgeToIndexMap()
 		*  \brief A function to initialize EdgeToIndexMap.
 		*/
    void initiateEdgeToIndexMap()
    {
    	if(edgeToIndexValid==true) return;
    	for(int i=0;i<edge_list.size();i++){
    		edgeToIndex[edge_list[i].edge]=i;
    	}
    	cout<<"in initiateEdgeToIndexMap";
    	edgeToIndexValid=true;

    	/*for(int i=0;i<edge_list.size();i++)
    	{
    		print_edge(edge_list[edgeToIndex[edge_list[i].edge]].edge);
    		print_edge(edge_list[i].edge);
    		cout<<" "<<i<<"\n";
    	}*/
    }

	/*! \fn  void print_adjacency_list()
 		*  \brief A function to print adjacency list.
 		*/
    void print_adjacency_list()
    {
      	cout<<"-------graph begin-------\n";
      	for(int i=0;i<graphAdj.size();i++)
      	{
       		cout<<"vertex: "<<graphAdj[i].vertex<<" degree: "<<graphAdj[i].adj_size<<"---- ";
			cout << graphAdj[i].adj_size <<endl;
       		for(int j=0;j<graphAdj[i].adj.size();j++)
       		{
       			cout<<graphAdj[i].adj[j]<<", ";
       		}
       		cout<<"\n";
      	}
       	cout<<"-------graph end-------\n";
    }

	/*! \fn EDGE make_edge(int a, int b)
 		*  \brief A function to build and EDGE object.
		*  \param a an integer
		*  \param b an integer
 		*  \return EDGE object
 		*/
    EDGE make_edge(int a, int b)
    {
    	if (a<b) return EDGE(a,b);
     	return EDGE(b,a);
    }

	/*! \fn  void print_edge(EDGE e)
 		*  \brief A function to print edge.
 		*/
    void print_edge(EDGE e)
    {
    	cout<<"("<<e.first<<","<<e.second<<") ";
    }

	
	/*! \fn int getEdgeCount()
 		*  \brief A function to get edge count.
 		*  \return integer
 		*/
    int getEdgeCount(){return edgeCount;}

	/*! \fn int getVertexCount()
 		*  \brief A function to get Vertex count.
 		*  \return integer
 		*/
    int getVertexCount(){return vertexToIndex.size();}

	
	
	//MMR: May 13
	/*! \fn bool getDisjointSets(disjointSet*& ds, graphlet*& t)
 		*  \brief A function to find connected component from the current graphlet after removing any of its nodes.
		*  \param ds address of disjointSet type pointer
		*  \param t address of graphlet pointer
 		*  \return boolean
 		*/
	bool getDisjointSets(disjointSet*& ds, graphlet*& t)
	{
		int noNodes=ds->nodes.size();
		
		vector<bool> edgesT;
		edgesT.resize((noNodes*(noNodes-1))/2);
		map<int,int>::const_iterator f;
		vector<int> seen;
		int count=0;
		
		//from a graphlet of size 3 (2 after deleting a node)
		if(ds->nodes.size()==2){	
			edgesT[0] = t->get_edge_status(ds->nodes[0],ds->nodes[1]);
			
			if(edgesT[0] == true)
			{
				ds->setIndex.resize(1);
				ds->setIndex[0]=0;
				return true;
			}
			else
			{
				ds->setIndex.resize(2);
				ds->setIndex[0]=0;
				ds->setIndex[1]=1;
				return false;
			}
		}
		
		//from a graphlet of size 4 (3 after deleting a node)
		if(ds->nodes.size()==3)
		{
			edgesT[0] = t->get_edge_status(ds->nodes[0],ds->nodes[1]);
			edgesT[1] = t->get_edge_status(ds->nodes[0],ds->nodes[2]);
			edgesT[2] = t->get_edge_status(ds->nodes[1],ds->nodes[2]);
			
			for(int i=0;i<edgesT.size();i++)
			{
				if(edgesT[i]==true)
				{
					count++;
				}
			}
			
			if (count == 0) {
				ds->setIndex.resize(3);
				ds->setIndex[0]=0;
				ds->setIndex[1]=1;
				ds->setIndex[2]=2;
				return false;
			}
			
			if (count == 1) {
				ds->setIndex.resize(2);
				ds->setIndex[0]=0;
				ds->setIndex[1]=2;
				
				
				vector<int> tempv;
				tempv.resize(3);
				
				if (edgesT[0]) {
					tempv[0] = ds->nodes[0];
					tempv[1] = ds->nodes[1];
					tempv[2] = ds->nodes[2];
				}else if (edgesT[1]) {
					tempv[0] = ds->nodes[0];
					tempv[1] = ds->nodes[2];
					tempv[2] = ds->nodes[1];
				}else if (edgesT[2]) {
					tempv[0] = ds->nodes[1];
					tempv[1] = ds->nodes[2];
					tempv[2] = ds->nodes[0];
				}
				
				for (int i=0;i<tempv.size(); i++) {
					ds->nodes[i]=tempv[i];
				}
				return false;
			}
			else
			{
				ds->setIndex.resize(1);
				ds->setIndex[0]=0;
				return true;
			}
		}
		
		//from a graphlet of size 5 (4 after deleting a node)
		if(ds->nodes.size()==4)
		{
			ds->setIndex.clear();
			
			vector<int> disSet;
			disSet.resize(4);
			for (int i=0; i<disSet.size(); ++i) {
				disSet[i]=i;
			}
			
			int change,changeWith;
			
			
			for (int x=0; x<ds->nodes.size(); ++x) {
				for (int y=x+1; y<ds->nodes.size(); ++y) {
					if (t->get_edge_status(ds->nodes[x],ds->nodes[y])) {
						changeWith = disSet[x];
						change= disSet[y];
						for (int i=0; i<disSet.size(); ++i) {
							if (disSet[i]==change) {
								disSet[i]=changeWith;
							}
							
						}
					}
					
				}
			}
			
			vector<int> temp;
			int tempIndex=0;
			temp.resize(4);
			bool first;
			for(int x =0; x< temp.size(); ++x)
			{
				first = true;
				for (int y=0; y<temp.size(); ++y) {
					if (disSet[y] == x) {
						temp[tempIndex] = ds->nodes[y]; 
						
						if (first) {
							first=false;
							ds->setIndex.push_back(tempIndex);
						}
						
						tempIndex++;
						
					}
				}
			}
			
			for (int x =0; x< temp.size(); ++x) {
				ds->nodes[x]=temp[x];
			}
			return true;
		}
		return false;
	}
	
	
	// MCMC based uniform sampling code start

	/*! \fn bool is_connected(vector<int> v,graphlet*& t)
 		*  \brief A function to check whether nodes in vector v are connected or not.
		*  \param v a vector<int>
		*  \param t address of graphlet pointer
 		*  \return boolean
 		*/
	bool is_connected(vector<int> v,graphlet*& t)
	{
		vector<bool>edgesT;
		edgesT.resize((v.size()*(v.size()-1))/2);
		map<int,int>::const_iterator f;
		vector<int> seen;
		int count=0;
	//	cout << "inside is connected:"<<v[0] <<"\t"<<v[1]<<endl;

		//triangle code start//
		if(v.size()==2){ //neighbors of a triangle/triple	
			edgesT[0] = t->get_edge_status(v[0],v[1]);

			if(edgesT[0] == true){return true;}
			else
			{
				return false;
			}
		}
		// triangle code end

		if(v.size()==3)
		{
			edgesT[0] = t->get_edge_status(v[0],v[1]);
			edgesT[1] = t->get_edge_status(v[0],v[2]);
			edgesT[2] = t->get_edge_status(v[1],v[2]);

			for(int i=0;i<edgesT.size();i++)
			{
				if(edgesT[i]==true){count++;}
			}
			if(count <=1){return false;}
			else{return true;}
		}

		if(v.size()==4)
		{
			edgesT[0] = t->get_edge_status(v[0],v[1]);
			edgesT[1] = t->get_edge_status(v[0],v[2]);
			edgesT[2] = t->get_edge_status(v[0],v[3]);
			edgesT[3] = t->get_edge_status(v[1],v[2]);
			edgesT[4] = t->get_edge_status(v[1],v[3]);
			edgesT[5] = t->get_edge_status(v[2],v[3]);

			for(int i=0;i<edgesT.size();i++)
			{
				if(edgesT[i]==true){count++;}
			}
			if(v.size()==4 && count <=2){return false;}

		//do it later
			if(v.size()==4 && count ==3){
		
				f=vertexToIndex.find(v[0]);
				if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[1])==graphAdj[f->second].adj.end()
   					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[2])==graphAdj[f->second].adj.end()
					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[3])==graphAdj[f->second].adj.end()){
					return false;
				}
			
				f=vertexToIndex.find(v[1]);
				if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[0])==graphAdj[f->second].adj.end()
   					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[2])==graphAdj[f->second].adj.end()
					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[3])==graphAdj[f->second].adj.end()){
					return false;
				}

				f=vertexToIndex.find(v[2]);
				if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[1])==graphAdj[f->second].adj.end()
   					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[0])==graphAdj[f->second].adj.end()
					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[3])==graphAdj[f->second].adj.end()){
					return false;
				}

				f=vertexToIndex.find(v[3]);
				if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[1])==graphAdj[f->second].adj.end()
   					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[2])==graphAdj[f->second].adj.end()
					and find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), v[0])==graphAdj[f->second].adj.end()){
					return false;
				}
		
				return true;
			}

	
			else if(v.size()==4 && count >3){return true;}
			else if(v.size()==3 && count <=1){return false;}
		}


	}

	//Added on May 6th 2013; edited May 15th

	/*! \fn int intersect_vector_to_array(vector<int> &v1, int sv1, vector<int> &v2, int sv2, int *&merge_v, vector<int> &v,int insertion_index)
 		*  \brief A function to find intersection of two vector and save the result into an array.
		*  \param v1 an address of vector<int>
		*  \param sv1 an integer
		*  \param v2 an address of  vector<int>
		*  \param sv2 an integer
		*  \param merge_v an address of integer pointer
		*  \param v an address of vector<int>
		*  \param insertion_index an integer
 		*  \return an integer
 		*/
	int intersect_vector_to_array(vector<int> &v1, int sv1, vector<int> &v2, int sv2, int *&merge_v, vector<int> &v,int insertion_index)//need to make sure nodes in v does not come in result
	{
		int t = sv1+sv2;
		int i=0;
		int j=0;
		vector<int>::iterator it;
		int k,index,k1;
		k1 = insertion_index;
		index = 0;
		if(sv1==0 || sv2==0)
		{
			return 0;
		}

		for(k=0,index=0; k<t; k++)
		{
			if(i == sv1 || j == sv2) break;
			if(v1[i]<v2[j])
			{
				i++;
			}else if (v1[i]>v2[j])
			{
				j++;
			}
			else if(v1[i]==v2[j])
			{
				it = find(v.begin(),v.end(),v1[i]);
				if(it==v.end())
				{
					merge_v[insertion_index] = v1[i];
					index++;	
					insertion_index++;				
				}
				i++;
				j++;
				k++;
			}
			
		}
		/*cout << "Intersect-";
			for(int i=k1;i<insertion_index;i++)
			{
				cout << merge_v[i] <<",";
			}
			cout <<endl;*/
		return index;
	}

	//End Added on May 6th 2013

	//triangle code start
	
	/*! \fn int merge_vector_to_array(vector<int> &v1, int sv1, vector<int> &v2, int sv2, int *&merge_v, vector<int> &v)
 		*  \brief A function to find union of two vector and save the result into an array.
		*  \param v1 an address of vector<int>
		*  \param sv1 an integer
		*  \param v2 an address of  vector<int>
		*  \param sv2 an integer
		*  \param merge_v an address of integer pointer
		*  \param v an address of vector<int>
 		*  \return an integer
 		*/
	int merge_vector_to_array(vector<int> &v1, int sv1, vector<int> &v2, int sv2, int *&merge_v, vector<int> &v)
	{
		int t = sv1+sv2;
		int i=0;
		int j=0;
		vector<int>::iterator it;
		int k,index;
		
		if(sv1==0 && sv2!=0)
		{
			for(int i=0; i< sv2;i++)
			{
				it = find(v.begin(),v.end(),v2[i]);
				if(it==v.end())
				{
					merge_v[i] = v2[i];
				}
			}
			return sv2;
		}
		if(sv2==0 && sv1!=0)
		{
			for(int i=0; i< sv1;i++)
			{
				it = find(v.begin(),v.end(),v1[i]);
				if(it==v.end())
				{
					merge_v[i] = v1[i];
				}
			}
			return sv1;
		}
		
		for(k=0,index=0; k<t; k++)
		{
			if(i == sv1 && j == sv2) break;
			if(v1[i]<v2[j])
			{
				it = find(v.begin(),v.end(),v1[i]);
				if(it==v.end())
				{
					merge_v[index] = v1[i];
				//	cout << merge_v[index] <<",";
					index++;
					
				}
				i=i+1;
				if(i== sv1)
				{
					for(;j<sv2;j++)
					{
						it = find(v.begin(),v.end(),v2[j]);
						if(it==v.end())
						{
							merge_v[index] = v2[j];
							index++;
						}
					}
				}
			}
			else if(v1[i]==v2[j])
			{
				it = find(v.begin(),v.end(),v1[i]);
				if(it==v.end())
				{
					merge_v[index] = v1[i];
				//	cout << merge_v[index] <<",";
					index++;					
				}
				i=i+1;
				j=j+1;
				if(i== sv1)
				{
					for(;j<sv2;j++)
					{
						it = find(v.begin(),v.end(),v2[j]);
						if(it==v.end())
						{
							merge_v[index] = v2[j];
							index++;
						}
					}
				}
				else if(j== sv2)
				{
					for(;i<sv1;i++)
					{
						it = find(v.begin(),v.end(),v1[i]);
						if(it==v.end())
						{
							merge_v[index] = v1[i];
							index++;
						}
					}
				}
				k=k+1;
			}
			else
			{
				it = find(v.begin(),v.end(),v2[j]);
				if(it==v.end())
				{
					merge_v[index] = v2[j];
				//	cout << merge_v[index] <<",";
					index++;			
				}
				j= j+1;
				if(j== sv2)
				{
					for(;i<sv1;i++)
					{
						it = find(v.begin(),v.end(),v1[i]);
						if(it==v.end())
						{
							merge_v[index] = v1[i];
							index++;
						}
					}
				}
			}
		}
		//cout <<"\nindex = " <<index <<endl;
		return index;
	}

	/*! \fn int intersection_array_to_array(int *&v1, int sv1, int *&v2, int sv2, int *&merge_v,int insertion_index)
 		*  \brief A function to find intersection of two array and save the result into an array.
		*  \param v1 an address of vector<int>
		*  \param sv1 an integer
		*  \param v2 an address of  vector<int>
		*  \param sv2 an integer
		*  \param merge_v an address of integer pointer
		*  \param v an address of vector<int>
 		*  \return an integer
 		*/
	int intersection_array_to_array(int *&v1, int sv1, int *&v2, int sv2, int *&merge_v,int insertion_index)
	{
		int t = sv1+sv2;
		int i=0;
		int j=0;
		vector<int>::iterator it;
		int k,index;
		
		if(sv1==0 || sv2==0)
		{
			return 0;
		}
		
		for(k=0,index=0; k<t; k++)
		{
			if(i == sv1 || j == sv2) break;
			
			if(v1[i]<v2[j])
			{
				i++;
			}else if (v1[i] > v2[j]) 
			{
				j++;
			}
			else if(v1[i]==v2[j])
			{
				
				merge_v[insertion_index] = v1[i];
				insertion_index++;
				index++;					
				
				i++;
				j++;
				k++;
			}
		}
		return index;
	}
	
	/*! \fn int merge_array_to_array(int *&v1, int sv1, int *&v2, int sv2, int *&merge_v,int insertion_index)
 		*  \brief A function to find union of two array and save the result into an array.
		*  \param v1 an address of integer pointer
		*  \param sv1 an integer
		*  \param v2 an address of integer pointer
		*  \param sv2 an integer
		*  \param merge_v an address of integer pointer
		*  \param insertion_index an integer
 		*  \return an integer
 		*/
	int merge_array_to_array(int *&v1, int sv1, int *&v2, int sv2, int *&merge_v,int insertion_index)
	{
		int t = sv1+sv2;
		int i=0;
		int j=0;
		vector<int>::iterator it;
		int k,index;
		
		if(sv1==0 && sv2!=0)
		{
			for(int i=0; i< sv2;i++)
			{
				merge_v[insertion_index] = v2[i];
				insertion_index++;
			}
			return sv2;
		}
		if(sv2==0 && sv1!=0)
		{
			for(int i=0; i< sv1;i++)
			{
				merge_v[insertion_index] = v1[i];
				insertion_index++;
			}
			return sv1;
		}
		
		for(k=0,index=0; k<t; k++)
		{
			if(i == sv1 && j == sv2) break;
			if(v1[i]<v2[j])
			{
				
					merge_v[insertion_index] = v1[i];
				//	cout << merge_v[index] <<",";
					index++;
					insertion_index++;		
				i=i+1;
				if(i== sv1)
				{
					for(;j<sv2;j++)
					{
						merge_v[insertion_index] = v2[j];
						index++;
						insertion_index++;
					}
				}
			}
			else if(v1[i]==v2[j])
			{
				
					merge_v[insertion_index] = v1[i];
					insertion_index++;
				//	cout << merge_v[index] <<",";
					index++;					
				
				i=i+1;
				j=j+1;
				if(i== sv1)
				{
					for(;j<sv2;j++)
					{
						merge_v[insertion_index] = v2[j];
						index++;
						insertion_index++;
					}
				}
				else if(j== sv2)
				{
					for(;i<sv1;i++)
					{
						merge_v[insertion_index] = v1[i];
						index++;
						insertion_index++;
					}
				}
				k=k+1;
			}
			else
			{
				
					merge_v[insertion_index] = v2[j];
				//	cout << merge_v[index] <<",";
					index++;	
					insertion_index++;		
				
				j= j+1;
				if(j== sv2)
				{
					for(;i<sv1;i++)
					{
						merge_v[insertion_index] = v1[i];
						index++;
						insertion_index++;
					}
				}
			}
		}
		//cout <<"\nindex = " <<index <<endl;
		return index;
	}

	/*! \fn void populate_neighbors(graphlet*& t,int *&neighbors_list, vector<int> &checkpoint)
 		*  \brief A function to populate neighborhood of graphlet t
		*  \param t an address of graphlet pointer
		*  \param neighbors_list an address of integer pointer
		*  \param checkpoint an address of vector<int>
 		*/
	void populate_neighbors(graphlet*& t,int *&neighbors_list, vector<int> &checkpoint)
	{	
	
		map<int,int>::const_iterator f,w,x,y,z,df,dw,dx,dy;
		vector<int> v;
		t->getVertices(v);
		vector<int> v_temp;
		v_temp.resize(0); //for some reason, this is important
		int temp[6];
		
		int insertion_index=0;
		
		int size;
		if(t->get_size()==3)
		{
			int degree=0;
			int d=0;
			insertion_index=0;
			int k=0;
			int l=0;
			int is_connect=0;
			//merging code start

			f = vertexToIndex.find(v[0]);
			w = vertexToIndex.find(v[1]);
			x = vertexToIndex.find(v[2]);		

			size = merge_vector_to_array(graphAdj[f->second].adj,graphAdj[f->second].adj.size(),graphAdj[w->second].adj,graphAdj[w->second].adj.size(),fw,v);
			sfw=size;
			
			size = merge_vector_to_array(graphAdj[w->second].adj,graphAdj[w->second].adj.size(),graphAdj[x->second].adj,graphAdj[x->second].adj.size(),wx,v);
			swx=size;
			
			size = merge_vector_to_array(graphAdj[f->second].adj,graphAdj[f->second].adj.size(),graphAdj[x->second].adj,graphAdj[x->second].adj.size(),fx,v);
			sfx=size;
			
			for(int i=0; i<t->get_size();i++)
			{		
				for(int j=0; j< t->get_size();j++)
				{
					if(j==i) continue;
					else{v_temp.push_back(v[j]);}
				}
				if(is_connected(v_temp,t))
				{
					temp[k] = v[i];
					k++;
					
					t->degree_sum[4] +=1;
					if(i==0)
					{
						
						d = merge_array_to_array(wx,swx,wx,swx,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
				
						degree += d;
						t->degree_sum[i] = d;
					}
					if(i==1)
					{
						
						d = merge_array_to_array(fx,sfx,fx,sfx,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
				
						degree += d;
						t->degree_sum[i] = d;
					}
					if(i==2)
					{
						
						d = merge_array_to_array(fw,sfw,fw,sfw,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
				
						degree += d;
						t->degree_sum[i] = d;
					}
				}
				else
				{
					disjointSet *tds = new disjointSet(v_temp);
					getDisjointSets(tds,t);
					//tds->printDisjointSet();
					
					df = vertexToIndex.find(tds->nodes[0]);
					dw = vertexToIndex.find(tds->nodes[1]);
					delete tds;
					
					d = intersect_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),neighbors_list,v,insertion_index);
					insertion_index = insertion_index+d;
					degree += d;
					t->degree_sum[i] = d;
					
				
				}
				v_temp.clear();
				
			}
			d = merge_array_to_array(fw,sfw,wx,swx,neighbors_list,insertion_index);
			insertion_index = insertion_index+d;
			t->degree_sum[3] = d;
			degree = degree+d;
			for(int i=0; i<k;i++ )
			{
					neighbors_list[insertion_index] = temp[i];
					insertion_index++;
			}
			
			t->set_degree(degree);
		}
		else if(t->get_size()==4)
		{
				int degree=0;
				int d=0;
				insertion_index=0;
				int k=0;
				int l=0;
				int is_connect=0;
				f = vertexToIndex.find(v[0]);
				w = vertexToIndex.find(v[1]);
				x = vertexToIndex.find(v[2]);
				y = vertexToIndex.find(v[3]);	
	
				size = merge_vector_to_array(graphAdj[f->second].adj,graphAdj[f->second].adj.size(),graphAdj[w->second].adj,graphAdj[w->second].adj.size(),fw,v);
				sfw=size;
			
			size = merge_vector_to_array(graphAdj[w->second].adj,graphAdj[w->second].adj.size(),graphAdj[x->second].adj,graphAdj[x->second].adj.size(),wx,v);
			swx=size;
			
			size = merge_vector_to_array(graphAdj[x->second].adj,graphAdj[x->second].adj.size(),graphAdj[y->second].adj,graphAdj[y->second].adj.size(),xy,v);
			sxy=size;
		

			size = merge_vector_to_array(graphAdj[y->second].adj,graphAdj[y->second].adj.size(),graphAdj[f->second].adj,graphAdj[f->second].adj.size(),yf,v);
			syf=size;	
			
			for(int i=0; i<t->get_size();i++)
			{		
				for(int j=0; j< t->get_size();j++)
				{
					if(j==i) continue;
					else{v_temp.push_back(v[j]);}
				}
				
				
				if(is_connected(v_temp,t))
				{
					temp[k] = v[i];
					k++;
					degree += 1;
					t->degree_sum[5] +=1;
					if(i==0)
					{
						
						d = merge_array_to_array(wx,swx,xy,sxy,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
					}

					else if(i==1)
					{
						
						d = merge_array_to_array(xy,sxy,yf,syf,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
					}


					else if(i==2)
					{
						
						d = merge_array_to_array(yf,syf,fw,sfw,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] =d;
					}

					else if(i==3)
					{
						
						d = merge_array_to_array(fw,sfw,wx,swx,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
					}
				
				}
				else
				{
					disjointSet *tds = new disjointSet(v_temp);
					getDisjointSets(tds,t);
					//tds->printDisjointSet();
					
					df = vertexToIndex.find(tds->nodes[0]);
					dw = vertexToIndex.find(tds->nodes[1]);
					dx = vertexToIndex.find(tds->nodes[2]);
					delete tds;
					
					if (tds->setIndex.size()==2) {
						if (tds->setIndex[1] == 2) {//.. .
							size = merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),dfw,v);//union
							dsfw=size;
							
							size = merge_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),temp1,v);
							stemp1=size;
							
							d = intersection_array_to_array(dfw,dsfw,temp1,stemp1,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
							
						}else if (tds->setIndex[1] == 1) {//. ..
							size = merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[df->second].adj,graphAdj[df->second].adj.size(),temp1,v);//union
							stemp1=size;
							
							size = merge_vector_to_array(graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),dwx,v);
							dswx=size;
							
							d = intersection_array_to_array(temp1,stemp1,dwx,dswx,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
							
						}
					}
					if (tds->setIndex.size()==3) {//. . .
						size=intersect_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),temp1,v, insertion_index);//need to make sure nodes in v does not come in result
						stemp1=size;
						
						size = merge_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),temp2,v);
						stemp2=size;
						
						d = intersection_array_to_array(temp1,stemp1,temp2,stemp2,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						degree += d;
						t->degree_sum[i] = d;
					}
					
					
				}
				
				v_temp.clear();
			}
			d = merge_array_to_array(wx,swx,yf,syf,neighbors_list,insertion_index);
			insertion_index = insertion_index + d;
			t->degree_sum[4] = d;
			degree += d;
			for(int i=0; i<k;i++ )
			{
				neighbors_list[insertion_index] = temp[i];
				insertion_index++;
			}
			t->set_degree(degree);		
					
		}
		else if(t->get_size()==5)
		{
				int degree=0;
				int d=0;
				insertion_index=0;
				int k=0;
				int l=0;
				int is_connect=0;
				f = vertexToIndex.find(v[0]);	
				w = vertexToIndex.find(v[1]);
				x = vertexToIndex.find(v[2]);
				y = vertexToIndex.find(v[3]);
				z = vertexToIndex.find(v[4]);	
				
			size = merge_vector_to_array(graphAdj[f->second].adj,graphAdj[f->second].adj.size(),graphAdj[w->second].adj,graphAdj[w->second].adj.size(),fw,v);
			sfw=size;
				
			size = merge_vector_to_array(graphAdj[w->second].adj,graphAdj[w->second].adj.size(),graphAdj[x->second].adj,graphAdj[x->second].adj.size(),wx,v);
			swx=size;
			
			size = merge_vector_to_array(graphAdj[x->second].adj,graphAdj[x->second].adj.size(),graphAdj[y->second].adj,graphAdj[y->second].adj.size(),xy,v);
			sxy=size;
			
			size = merge_vector_to_array(graphAdj[y->second].adj,graphAdj[y->second].adj.size(),graphAdj[z->second].adj,graphAdj[z->second].adj.size(),yz,v);
			syz=size;
			
			size = merge_vector_to_array(graphAdj[z->second].adj,graphAdj[z->second].adj.size(),graphAdj[f->second].adj,graphAdj[f->second].adj.size(),zf,v);
			szf=size;
			
			for(int i=0; i<t->get_size();i++)
			{
				for(int j=0; j< t->get_size();j++)
				{
					if(j==i) continue;
					else{v_temp.push_back(v[j]);}
				}
				
				if(is_connected(v_temp,t))
				{
					temp[k] = v[i];
					k++;
					degree += 1;
					t->degree_sum[5]+=1;
					if(i==0)
					{
						
						d = merge_array_to_array(wx, swx, yz, syz, neighbors_list, insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
						
					}

					else if(i==1)
					{
						d = merge_array_to_array(xy, sxy, zf, szf, neighbors_list , insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
						
					}


					else if(i==2)
					{
						
						d = merge_array_to_array(fw,sfw,yz,syz, neighbors_list , insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
						
					}

					else if(i==3)
					{
						
						d = merge_array_to_array(wx, swx, zf, szf, neighbors_list , insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
						
					}

					else if(i==4)
					{
						
						d = merge_array_to_array(xy, sxy, fw, sfw, neighbors_list, insertion_index);
						insertion_index = insertion_index+d;
						
						degree += d;
						t->degree_sum[i] = d;
					}	
				}
				else
				{
					disjointSet *tds = new disjointSet(v_temp);
					getDisjointSets(tds,t);
					//tds->printDisjointSet();
					
					
					df = vertexToIndex.find(tds->nodes[0]);
					dw = vertexToIndex.find(tds->nodes[1]);
					dx = vertexToIndex.find(tds->nodes[2]);
					dy = vertexToIndex.find(tds->nodes[3]);
					delete tds;
					if (tds->setIndex.size()==2) {
						if (tds->setIndex[1] == 2) {//.. ..
							size=merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),dfw,v);//need to make sure nodes in v does not come in result
							dsfw=size;
							
							size = merge_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),dxy,v);
							dsxy=size;
							
							d = intersection_array_to_array(dfw,dsfw,dxy,dsxy,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
						}
						
						if (tds->setIndex[1] == 3) {//... .
							size=merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),dfw,v);//need to make sure nodes in v does not come in result
							dsfw=size;
							
							size = merge_vector_to_array(graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),dwx,v);
							dswx=size;
							
							
							size = merge_array_to_array(dfw,dsfw,dwx,dswx,temp1,insertion_index);//union
							stemp1=size;
							
							size = merge_vector_to_array(graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),temp2,v);
							stemp2=size;
							
							
							d = intersection_array_to_array(temp1,stemp1,temp2,stemp2,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
							
						}else if (tds->setIndex[1] == 1) {//. ...
							size=merge_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),dxy,v);//need to make sure nodes in v does not come in result
							dsxy=size;
							
							size = merge_vector_to_array(graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),dwx,v);
							dswx=size;
							
							
							size = merge_array_to_array(dwx,dswx,dxy,dsxy,temp1,insertion_index);//union
							stemp1=size;
							
							size = merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[df->second].adj,graphAdj[df->second].adj.size(),temp2,v);
							stemp2=size;
							
							
							d = intersection_array_to_array(temp1,stemp1,temp2,stemp2,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
						}
						
					}
					if (tds->setIndex.size()==4) {//. . . .
						size=intersect_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),temp1,v, insertion_index);//need to make sure nodes in v does not come in result
						stemp1=size;
						
						size = intersect_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),temp2,v,insertion_index);
						stemp2=size;
						
						d = intersection_array_to_array(temp1,stemp1,temp2,stemp2,neighbors_list,insertion_index);
						insertion_index = insertion_index+d;
						degree += d;
						t->degree_sum[i] = d;
					}
					
					if (tds->setIndex.size()==3) {
						if (tds->setIndex[1]==2) {//.. . .
							size=intersect_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),temp1,v, insertion_index);//need to make sure nodes in v does not come in result
							stemp1=size;
							
							size=merge_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),dfw,v);//need to make sure nodes in v does not come in result
							dsfw=size;
							
							d = intersection_array_to_array(temp1,stemp1,dfw,dsfw,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
							
						}
						else if(tds->setIndex[2] == 2)//. . ..							
						{
							size=intersect_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),temp1,v, insertion_index);//need to make sure nodes in v does not come in result
							stemp1=size;
							
							size=merge_vector_to_array(graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),dxy,v);//need to make sure nodes in v does not come in result
							dsxy=size;
							
							d = intersection_array_to_array(temp1,stemp1,dxy,dsxy,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
						}
						else if (tds->setIndex[1]==1 && tds->setIndex[2] == 3) {//. .. .
							size=intersect_vector_to_array(graphAdj[df->second].adj,graphAdj[df->second].adj.size(),graphAdj[dy->second].adj,graphAdj[dy->second].adj.size(),temp1,v, insertion_index);//need to make sure nodes in v does not come in result
							stemp1=size;
							
							size = merge_vector_to_array(graphAdj[dw->second].adj,graphAdj[dw->second].adj.size(),graphAdj[dx->second].adj,graphAdj[dx->second].adj.size(),dwx,v);
							dswx=size;
							
							d = intersection_array_to_array(temp1,stemp1,dwx,dswx,neighbors_list,insertion_index);
							insertion_index = insertion_index+d;
							degree += d;
							t->degree_sum[i] = d;
						}
						else
						{
							cout << "error in disjoint\n";
						}
						
						
					}
				
				
				}
				
				v_temp.clear();
			}
					for(int i=0; i<k;i++ )
					{
						neighbors_list[insertion_index] = temp[i];
						insertion_index++;
					}
					t->set_degree(degree);	
				
			}	
	}

	/*! \fn void find_which_graplet(graphlet *&t, vector<bool>& edgesT)
 		*  \brief A function to identify the graphlet type
		*  \param t an address of graphlet pointer
		*  \param edgesT an address of vector<bool>
 		*/
	void find_which_graplet(graphlet *&t, vector<bool>& edgesT)
	{
		int *degree  = new int[t->get_size()];
		vector<int> v;
		t->getVertices(v);
		
		if(t->get_size()==3)
		{
			degree[0] = degree[1] = degree[2] = 0;
			if(edgesT[0]==true)
			{
				degree[0]++;
				degree[1]++;
			}
			if(edgesT[1]==true)
			{
				degree[0]++;
				degree[2]++;
			}
			if(edgesT[2]==true)
			{
				degree[1]++;
				degree[2]++;
			}
			sort(degree, degree+3);

			if(degree[0]==1 && degree[1]==1 && degree[2]==2)
			{
				t->graphletid = 1;
			}
			else if(degree[0]==2 && degree[1]==2 && degree[2]==2)
			{
				t->graphletid = 2;
			}
			
		}
		if(t->get_size()==4)
		{
			degree[0] = degree[1] = degree[2] = degree[3]=0;
			if(edgesT[0]==true)
			{
				degree[0]++;
				degree[1]++;
			}
			if(edgesT[1]==true)
			{
				degree[0]++;
				degree[2]++;
			}
			if(edgesT[2]==true)
			{
				degree[0]++;
				degree[3]++;
			}
			if(edgesT[3]==true)
			{
				degree[1]++;
				degree[2]++;
			}
			if(edgesT[4]==true)
			{
				degree[1]++;
				degree[3]++;
			}
			if(edgesT[5]==true)
			{
				degree[2]++;
				degree[3]++;
			}

	//		cout <<"degree"<< degree[0] <<", "<< degree[1]<<", "<<degree[2] <<", "<< degree[3]<<endl;
			sort(degree, degree+4);

	//		cout <<"degree"<< degree[0] <<", "<< degree[1]<<", "<<degree[2] <<", "<< degree[3]<<endl;
			if(degree[0]==1 && degree[1]==1 && degree[2]==2 && degree[3]==2)
			{
				t->graphletid = 3;
			}
			else if(degree[0]==1 && degree[1]==1 && degree[2]==1 && degree[3]==3)
			{
				t->graphletid = 4;
			}
			else if(degree[0]==2 && degree[1]==2 && degree[2]==2 && degree[3]==2)
			{
				t->graphletid = 5;
			}
			else if(degree[0]==1 && degree[1]==2 && degree[2]==2 && degree[3]==3)
			{
				t->graphletid = 6;
			}

			else if(degree[0]==2 && degree[1]==2 && degree[2]==3 && degree[3]==3)
			{
				t->graphletid = 7;
			}
			else if(degree[0]==3 && degree[1]==3 && degree[2]==3 && degree[3]==3)
			{
				t->graphletid = 8;
			}
		}
		if(t->get_size()==5)
		{
			vector<int> copy_degree;
			copy_degree.resize(t->get_size());
			degree[0] = degree[1] = degree[2] = degree[3] = degree[4] = 0;
			if(edgesT[0]==true)
			{ 
				degree[0]++;
				degree[1]++;
			}
			if(edgesT[1]==true)
			{
				degree[0]++;
				degree[2]++;
			}
			if(edgesT[2]==true)
			{
				degree[0]++;
				degree[3]++;
			}
			if(edgesT[3]==true)
			{
				degree[0]++;
				degree[4]++;
			}
			if(edgesT[4]==true)
			{
				degree[1]++;
				degree[2]++;
			}
			if(edgesT[5]==true)
			{
				degree[1]++;
				degree[3]++;
			}
			
			if(edgesT[6]==true)
			{
				degree[1]++;
				degree[4]++;
			}
			if(edgesT[7]==true)
			{
				degree[2]++;
				degree[3]++;
			}
			if(edgesT[8]==true)
			{
				degree[2]++;
				degree[4]++;
			}
			if(edgesT[9]==true)
			{
				degree[3]++;
				degree[4]++;
			}

	
			copy_degree[0] = degree[0];copy_degree[1] = degree[1];copy_degree[2] = degree[2];copy_degree[3] = degree[3];copy_degree[4] = degree[4];
			sort(degree, degree+5);
		
			if(degree[0]==1 && degree[1]==1 && degree[2]==2 && degree[3]==2 && degree[4]==2)
			{
				t->graphletid = 9;
			}
			else if(degree[0]==1 && degree[1]==1 && degree[2]==1 && degree[3]==2 && degree[4]==3)
			{
				t->graphletid = 10;
			}
			else if(degree[0]==1 && degree[1]==1 && degree[2]==1 && degree[3]==1 && degree[4]==4)
			{
				t->graphletid = 11;
			}
			else if(degree[0]==1 && degree[1]==1 && degree[2]==2 && degree[3]==3 && degree[4]==3)
			{
				t->graphletid = 12;
			}

			else if(degree[0]==1 && degree[1]==2 && degree[2]==2 && degree[3]==2 && degree[4]==3)
			{
				int index;
				for(int i=0; i<copy_degree.size();i++)
				{
					if(copy_degree[i]==1)
					{
						index = i;
						break;
					}
				}
				for(int j=0; j<copy_degree.size();j++)
				{
					if(index==j)continue;
					if(t->get_edge_status(v[index],v[j]))
					{
						index = j;
						break;
					}
				}
				if(copy_degree[index]==2)
					t->graphletid = 13;
				else
					t->graphletid = 16;
			}
			else if(degree[0]==1 && degree[1]==1 && degree[2]==2 && degree[3]==2 && degree[4]==4)
			{
				t->graphletid = 14;
			}

			if(degree[0]==2 && degree[1]==2 && degree[2]==2 && degree[3]==2 && degree[4]==2)
			{
				t->graphletid = 15;
			}
			else if(degree[0]==1 && degree[1]==2 && degree[2]==2 && degree[3]==3 && degree[4]==4)
			{
				t->graphletid = 17;
			}
			else if(degree[0]==2 && degree[1]==2 && degree[2]==2 && degree[3]==2 && degree[4]==4)
			{
				t->graphletid = 18;
			}
			else if(degree[0]==1 && degree[1]==2 && degree[2]==3 && degree[3]==3 && degree[4]==3)
			{
				t->graphletid = 19;
			}

			else if(degree[0]==2 && degree[1]==2 && degree[2]==2 && degree[3]==3 && degree[4]==3)
			{
				int index;
				vector<int> _index;
				for(int i=0; i<copy_degree.size();i++)
				{
					if(copy_degree[i]==3)
					{
						index = i;
						break;
					}
				}
				for(int j=0; j<copy_degree.size();j++)
				{
					if(index==j)continue;
					if(t->get_edge_status(v[index],v[j]))
					{
						_index.push_back(j);
						//break;
					}
				}
				if(copy_degree[_index[0]]==2 && copy_degree[_index[1]]==2 && copy_degree[_index[2]]==2)
					t->graphletid = 20;
				else
					t->graphletid = 21;
			
			}
			else if(degree[0]==2 && degree[1]==2 && degree[2]==2 && degree[3]==4 && degree[4]==4)
			{
				t->graphletid = 22;
			}

			if(degree[0]==1 && degree[1]==3 && degree[2]==3 && degree[3]==3 && degree[4]==4)
			{
				t->graphletid = 23;
			}
			else if(degree[0]==2 && degree[1]==2 && degree[2]==3 && degree[3]==3 && degree[4]==4)
			{
				t->graphletid = 24;
			}
			else if(degree[0]==2 && degree[1]==3 && degree[2]==3 && degree[3]==3 && degree[4]==3)
			{
				t->graphletid = 25;
			}
			else if(degree[0]==2 && degree[1]==3 && degree[2]==3 && degree[3]==4 && degree[4]==4)
			{
				t->graphletid = 26;
			}

			else if(degree[0]==3 && degree[1]==3 && degree[2]==3 && degree[3]==3 && degree[4]==4)
			{
				t->graphletid = 27;
			}
			else if(degree[0]==3 && degree[1]==3 && degree[2]==4 && degree[3]==4 && degree[4]==4)
			{
				t->graphletid = 28;
			}
			else if(degree[0]==4 && degree[1]==4 && degree[2]==4 && degree[3]==4 && degree[4]==4)
			{
				t->graphletid = 29;
			}
			return;
		}
	}

	/*! \fn void get_graphletid(graphlet *& t)
 		*  \brief A function to identify the graphlet type
		*  \param t an address of graphlet pointer
 		*/
	void get_graphletid(graphlet *& t)
	{
		vector<int>v;
		t->getVertices(v);
		set<graphlet*> graphlets;
		int flag=0;
		
		vector<bool>edgesT;
		edgesT.resize((v.size()*(v.size()-1))/2);
		
			if(t->get_size()==3)
			{
				int k=0;
				for(int i=0; i<v.size();i++)
				{
					for(int j=i+1; j<v.size();j++)
					{
						edgesT[k] = t->get_edge_status(v[i],v[j]);
				
						if(edgesT[k]==false)
						{
							edgesT[k] = findEdge(v[i],v[j]);	
							if(edgesT[k]==true)
							{
								t->set_edge_status(v[i],v[j]);
							}
						}
				
						k++;
					
					}

				}
			
				find_which_graplet(t,edgesT);
			}
			if(t->get_size()==4)
			{
				
				int k=0;
				for(int i=0; i<v.size();i++)
				{
					for(int j=i+1; j<v.size();j++)
					{
						edgesT[k] = t->get_edge_status(v[i],v[j]);
				
						if(edgesT[k]==false)
						{
							edgesT[k] = new_findEdge(v[i],v[j]);	
							if(edgesT[k]==true)
							{
								t->set_edge_status(v[i],v[j]);
							}
						}
				
						k++;
					}

				}
		
				find_which_graplet(t,edgesT);
			}

			if(t->get_size()==5)
			{
				
				int k=0;
				for(int i=0; i<v.size();i++)
				{
					for(int j=i+1; j<v.size();j++)
					{
						edgesT[k] = t->get_edge_status(v[i],v[j]);
				
						if(edgesT[k]==false)
						{
							edgesT[k] = new_findEdge(v[i],v[j]);	
							if(edgesT[k]==true)
							{
								t->set_edge_status(v[i],v[j]);
							}
						}
			
						k++;
					}

				}
			
				find_which_graplet(t,edgesT);
			}
	}

	/*! \fn graphlet * get_a_initial_graphlet(int size)
 		*  \brief A function to get a initial graphlet to start random walk
		*  \param size an integer
		*  \return graphlet pointer
 		*/
	graphlet * get_a_initial_graphlet(int size)
	{
		for(int i=1;i<=29;i++){
       		symbolC[i]=0;
    	}
		int rand = boost_get_a_random_number(0, edge_list.size());
		
		EDGE e = edge_list[rand].edge;
		
		vector<int> v;
    	v.resize(size);
    	vector<bool> edgesT;
       	map<int,int>::const_iterator f,s;
    	f=vertexToIndex.find(e.first);
   		s=vertexToIndex.find(e.second);
   		if(f == vertexToIndex.end() or s == vertexToIndex.end())
   		{
   			cout<<"the vertex does not exist\n";
        	exit(1);
   		}
   		v[0]=f->first;//v[0] contains the first vertex of edge
   		v[1]=s->first;//v[1] contains the second vertex of edge
   		if(find(graphAdj[f->second].adj.begin(), graphAdj[f->second].adj.end(), e.second)==graphAdj[f->second].adj.end()
   			or find(graphAdj[s->second].adj.begin(), graphAdj[s->second].adj.end(), e.first)==graphAdj[s->second].adj.end()){
    		print_edge(e);
    		cout<<"the edge does not exist\n";
    		exit(1);
   		}
		int flag=0;
		for (int i=0;i<graphAdj[f->second].adj.size();i++){
				if(flag==1){break;}
   				v[2]=graphAdj[f->second].adj[i];
   				if (v[2]==s->first || v[2] == f->first) continue;
				if(size==3){cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<":3\n";break;}
				
				
   				for(int j=i+1; j<graphAdj[f->second].adj.size();j++){
					if(flag==1){break;}
   					v[3]=graphAdj[f->second].adj[j];
   					if(v[3]==s->first || v[2] == f->first) continue;
					if(size == 4){flag=1; break;}
					cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]<<":4\n";

   					for(int k=j+1; k<graphAdj[f->second].adj.size();k++){
   						v[4]=graphAdj[f->second].adj[k];
   						if(v[4]==s->first || v[2] == f->first) continue;
						else if(size==5){cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]<<" "<<v[4]<<":11_1\n";flag=1;break;}	
   					}
   				}
			}
		
		if(size==3)
		{
			graphlet *t = new graphlet(v[0],v[1],v[2]);
			t->set_edge_status(v[0],v[1]);
			t->set_edge_status(v[0],v[2]);
			t->print_graphlet();
			get_graphletid(t);
			
			return t;
		}
		else if(size==4)

		{
			
			graphlet *t = new graphlet(v[0],v[1],v[2],v[3]);
			t->set_edge_status(v[0],v[1]);
			t->set_edge_status(v[0],v[2]);
			t->set_edge_status(v[0],v[3]);
			if(findEdge(v[1],v[2]))
				t->set_edge_status(v[1],v[2]);
			if(findEdge(v[1],v[3]))
				t->set_edge_status(v[1],v[3]);
			if(findEdge(v[2],v[3]))
				t->set_edge_status(v[2],v[3]);
			
			get_graphletid(t);
			
			return t;
		}
		else
		{
			graphlet *t = new graphlet(v[0],v[1],v[2],v[3],v[4]);
			t->set_edge_status(v[0],v[1]);
			t->set_edge_status(v[0],v[2]);
			t->set_edge_status(v[0],v[3]);
			t->set_edge_status(v[0],v[4]);
			if(new_findEdge(v[1],v[2]))
				t->set_edge_status(v[1],v[2]);
			if(new_findEdge(v[1],v[3]))
				t->set_edge_status(v[1],v[3]);
			if(new_findEdge(v[1],v[4]))
				t->set_edge_status(v[1],v[4]);
			if(new_findEdge(v[2],v[3]))
				t->set_edge_status(v[2],v[3]);
			if(new_findEdge(v[2],v[4]))
				t->set_edge_status(v[2],v[4]);
			if(new_findEdge(v[3],v[4]))
				t->set_edge_status(v[3],v[4]);
			
			get_graphletid(t);
			
			return t;
		}
		
	}
	

};
#endif
