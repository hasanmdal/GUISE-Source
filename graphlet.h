//! \file graphlet.h - definition of graphlet class
#ifndef GRAPHLET_H_
#define GRAPHLET_H_
#include <sstream>
#include <iostream>
#include "graphGraphlet.h"
using namespace std;

class graphlet
{
private:
	int *vertices; //!< A private member variable
	int size; //!< A private member variable
	int degree; //!< A private member variable
	bool isprocessed; //!< A private member variable
	map<pair<int,int>,bool> edge_status; //!< A private member variable 
	map<pair<int,int>,bool>::iterator esit; //!< A private member variable
	
public:
	 
	vector<int> degree_sum; //!< A public member variable
	//set<graphlet*> neighbors;
	vector<graphlet*> _neighbors;
	int graphletid; //!< A public member variable
	graphlet(){}//!< Constructor

	/*! \fn graphlet(int X,int Y,int Z)
 		*  \brief A constructor function to create 3 node graphlets.
 		*  \param X an integer
		*  \param Y an integer
		*  \param Z an integer
 		*/
	graphlet(int X,int Y,int Z)
	{
		size=3;
		degree_sum.resize(6);
		for(int i=0;i<6;i++)degree_sum[i]=0;
		
		vertices = new int[3];
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		edge_status.insert(make_pair(make_pair(X,Y),false));
		edge_status.insert(make_pair(make_pair(Y,Z),false));
		edge_status.insert(make_pair(make_pair(Z,X),false));
		sort(vertices,vertices+3);
	}

	/*! \fn bool get_edge_status(int X, int Y)
 		*  \brief A function to know whether there is any edge between X and Y.
 		*  \param X an integer
		*  \param Y an integer
		*  \return boolean
 		*/
	bool get_edge_status(int X, int Y)
	{
		
		esit = edge_status.find(make_pair(X,Y));
		if(esit != edge_status.end())
		{
			return esit->second;
		}
		esit = edge_status.begin();
		esit = edge_status.find(make_pair(Y,X));
		if(esit !=edge_status.end())
		{
			return esit->second;
			
		}
		else
		{
			cout << "no stat found\n";
            return false;
		}
		
	}
	/*! \fn void clear_edge_stat()
 		*  \brief A function to clear edge_status vector.
 		*/
	void clear_edge_stat()
	{
		edge_status.clear();
	}
	
	/*! \fn void set_edge_status(int X, int Y)
 		*  \brief A function to set edge status to true between X and Y.
 		*  \param X an integer
		*  \param Y an integer
 		*/
	void set_edge_status(int X, int Y)
	{
		esit = edge_status.find(make_pair(X,Y));
		if(esit != edge_status.end()){
		//cout << "set to "<<X <<"and" <<Y<<endl;
			esit->second = true;
			return;
		}
		esit = edge_status.begin();
		esit = edge_status.find(make_pair(Y,X));
		if(esit != edge_status.end()){
		//cout << "set to "<<X <<"and" <<Y<<endl;
			esit->second = true;
			return;
		}
		else	{cout << "can not be set\n";}
		
	}

	/*! \fn graphlet(int X,int Y,int Z,int W)
 		*  \brief A constructor function to create 4 node graphlets.
 		*  \param X an integer
		*  \param Y an integer
		*  \param Z an integer
		*  \param W an integer
 		*/
	graphlet(int X,int Y,int Z,int W)
	{
		size=4;
		degree_sum.resize(6);
		for(int i=0;i<6;i++)degree_sum[i]=0;
		vertices = new int[4];
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		vertices[3] = W;
		edge_status.insert(make_pair(make_pair(X,Y),false));
		edge_status.insert(make_pair(make_pair(X,Z),false));
		edge_status.insert(make_pair(make_pair(X,W),false));
		edge_status.insert(make_pair(make_pair(Y,Z),false));
		edge_status.insert(make_pair(make_pair(Y,W),false));
		edge_status.insert(make_pair(make_pair(Z,W),false));
		
		sort(vertices,vertices+4);
	}

	/*! \fn graphlet(int V,int W,int X,int Y,int Z)
 		*  \brief A constructor function to create 4 node graphlets.
 		*  \param V an integer
		*  \param W an integer
		*  \param X an integer
		*  \param Y an integer
		*  \param Z an integer
 		*/
	graphlet(int V,int W,int X,int Y,int Z)
	{
		size=5;
		vertices = new int[5];
		degree_sum.resize(6);
		for(int i=0;i<6;i++)degree_sum[i]=0;
		vertices[0] = V;
		vertices[1] = W;
		vertices[2] = X;
		vertices[3] = Y;
		vertices[4] = Z;
		edge_status.insert(make_pair(make_pair(V,W),false));
		edge_status.insert(make_pair(make_pair(V,X),false));
		edge_status.insert(make_pair(make_pair(V,Y),false));
		edge_status.insert(make_pair(make_pair(V,Z),false));
		edge_status.insert(make_pair(make_pair(W,X),false));
		edge_status.insert(make_pair(make_pair(W,Y),false));
		edge_status.insert(make_pair(make_pair(W,Z),false));
		edge_status.insert(make_pair(make_pair(X,Y),false));
		edge_status.insert(make_pair(make_pair(X,Z),false));
		edge_status.insert(make_pair(make_pair(Y,Z),false));
		sort(vertices,vertices+5);
	}
	//!< Destructor
	~graphlet(){
		//_neighbors.clear();
		delete vertices;
		//cout<<"graphlet destructor\n";
	}

	/*! \fn void set_degree(int _degree)
 		*  \brief A function to set degree of current graphlet.
 		*  \param degree an integer
 		*/
	void set_degree(int _degree){
		degree = _degree;
	}

	/*! \fn int get_size()
 		*  \brief A function to know the size of the current graphlet.
		*  \return an integer
 		*/
	int get_size()
	{
		return size;
	}

	
	/*! \fn int get_size()
 		*  \brief A function to know the degree of the current graphlet.
		*  \return an integer
 		*/
	int get_degree()
	{
		return degree;
	}

	/*! \fn void getVertices(vector<int> & t)
 		*  \brief A function to get vertices of the current graphlet.
 		*  \param t address of vector<int>
 		*/
	void getVertices(vector<int> & t)
	{
		for(int i=0; i< this->size; i++)
			 t.push_back(vertices[i]);
	}

	/*! \fn void set_isprocessed(bool t)
 		*  \brief A function to set isprocessed flag of the current graphlet.
 		*  \param t a boolean
		*/
	void set_isprocessed(bool t)
	{
		isprocessed = t;
	}

	/*! \fn void set_isprocessed(bool t)
 		*  \brief A function to get isprocessed flag of the current graphlet.
 		*  \return boolean 
		*/
	bool get_isprocessed()
	{
		return isprocessed;
	}

	/*! \fn void print_edge_stat()
 		*  \brief A function print edge status of a graphlet.
		*/
	void print_edge_stat(){
		cout<<"edgestat:(";		
		for(esit=edge_status.begin(); esit!= edge_status.end(); esit++)
			 cout << (esit->first).first << ","<<(esit->first).second <<" stat = "<<esit->second<<endl;;
		cout <<")"<<endl;
	}

	/*! \fn void print_graphlet()
 		*  \brief A function print a graphlet.
		*/
	void print_graphlet(){
		cout<<"graphlet:(";		
		for(int i=0; i< this->size; i++)
			 cout << vertices[i] << ",";
		cout <<")"<<endl;
	}
	
	/*! \fn bool operator==( graphlet &g1)
 		*  \brief == operator overloading function for graphlets
		*/
	bool operator==( graphlet &g1)
	{
		if(vertices[0] == g1.vertices[0] && vertices[1] == g1.vertices[1] && vertices[2] == g1.vertices[2] 
			&& vertices[3] == g1.vertices[3] && vertices[4] == g1.vertices[4])
		{
			//this->print_graphlet();
			//g1.print_graphlet();
			return true;
		}
		else
		{
			return false;
		}
	}
	/*! \fn graphlet* clone() const
 		*  \brief clone function for graphlet object
		*  \return graphlet type pointer
		*/
	graphlet* clone() const {

   //   	cout << "In clone:\n";
		if(size==3)
		{
			graphlet* clone = new graphlet(vertices[0],vertices[1],vertices[2]);
			clone->size= size;
			clone->edge_status = edge_status;
			clone->_neighbors.assign(_neighbors.begin(), _neighbors.end());
			clone->graphletid = graphletid;
			clone->degree_sum.assign(degree_sum.begin(),degree_sum.end());
			clone->degree = degree;
			return clone;
		}
		if(size==4)
		{
			graphlet* clone = new graphlet(vertices[0],vertices[1],vertices[2],vertices[3]);
			clone->size= size;
			clone->edge_status = edge_status;
			clone->_neighbors.assign(_neighbors.begin(), _neighbors.end());
			clone->graphletid = graphletid;
			clone->degree_sum.assign(degree_sum.begin(),degree_sum.end());
			clone->degree = degree;
			return clone;
		}
		if(size==5)
		{
			graphlet* clone = new graphlet(vertices[0],vertices[1],vertices[2],vertices[3],vertices[4]);
			clone->size= size;
			clone->edge_status = edge_status;
			clone->_neighbors.assign(_neighbors.begin(), _neighbors.end());
			clone->graphletid = graphletid;
			clone->degree_sum.assign(degree_sum.begin(),degree_sum.end());
			clone->degree = degree;
			return clone;
		}
    }

	/*! \fn std::string canonical_code() const
 		*  \brief A function to get unique code for a graphlet
		*  \return string
		*/
	std::string canonical_code() const { 
      
	  	vector<int> data;
		for(int i=0; i< this->size; i++)
			 data.push_back(vertices[i]);
	  	
      stringstream ss;
      std::copy(data.begin(), data.end(), ostream_iterator<int>(ss, "_"));
      //cout << "canonical code: "<< ss.str()<<endl;
		data.clear();
      return ss.str();
    } 
	//vector<int> degree_sum (6);
};

#endif 
