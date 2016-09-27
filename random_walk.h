#ifndef _RANDOM_WALK_H_
#define _RANDOM_WALK_H_
//! \file random_walk.h - definition and implementation of random_walk class
#include <fstream>
#include <exception>
#include <iostream>
#include <iterator>
#include <set>
#include <map>
#include <math.h>
#include <string>
#include "StringTokenizer.h"
#include "random.h"
#include "graphlet.h" 
#include "graphGraphlet.h"

using namespace std;

class random_walk
{

private:
	double maxiter; //!< A private member variable
	graph_ * g; //!< A private member variable
	vector<int> graphletcount; //!< A private member variable
	map<string,int> visit_count; //!< A private member variable
	map<string,int>::iterator vit; //!< A private member variable
	map<string,int>::iterator cit; //!< A private member variable

public:
	//map<int,vector<int> > neighbors_list;
	//map<int,vector<int> > next_neighbors_list;
	int *neighbors_list; //!< A public member variable
	int *next_neighbors_list; //!< A public member variable
	
	vector<int> checkpoint; //!< A public member variable
	vector<int> temp; //!< A public member variable
	ofstream out4; //!< A public member variable

	/*! \fn random_walk(double iter, graph_ *G)
 		*  \brief A constructor function for random walk class.
 		*  \param iter a double
		*  \param *G a graph_ type pointer
 		*/
	random_walk(double iter, graph_ *G)
	{
		maxiter = iter;
		g = G;
		neighbors_list = new int[1000000];
		next_neighbors_list = new int[1000000];
		checkpoint.resize(6);
		temp.resize(6);
        graphletcount.resize(30);
		//out4.open("reject");
		
	}

	/*! \fn void walk()
 		*  \brief A member function to do the random walk on graphlet space. Walk continues until
		* iteration count hits maximum number of iteration. From a current node this method selects
		* next node to jump based on acceptance probability calculated by Metropolis-Hastings 
		* algorithm.
 		*/
	void walk()
	{
		int *swap;
		graphlet *next = g->get_a_initial_graphlet(4);
		process_graphlet(next,next_neighbors_list);
		
		ofstream out2("visit_count"); // Will hold visit count of each graphlet
		ofstream out3 ("graphlet_ineach_iteration"); // Will contain graphlet visited in each iteration
		ofstream out4 ("graphlet_count"); // will contain total visit count of each type(1-29) of graphlet
		set<graphlet*>::iterator sit;
		double totalgraphlet=0;
		int iter=1;
		int l1=0;
		vector<int> v;
        cout << "Random walk starts for " << maxiter << " iterations\n";
		while(1)
		{			
			graphlet * current = next;
			
            graphletcount[current->graphletid]++;
			swap = neighbors_list;
			neighbors_list = next_neighbors_list;
			next_neighbors_list = swap;
			next = get_next(current);
			
			
			if(iter>maxiter)
			{
				
				for(cit = visit_count.begin(); cit != visit_count.end();cit++)
				{
					out2 << cit->first<<"\t"<<cit->second<<endl;
				}
				for(int k=1; k<30; k++)
				{
					out4 << k << "\t" << graphletcount[k] << "\n";
					totalgraphlet += graphletcount[k];
				}
				totalgraphlet += 29;
				cout << "(graphlet type,proportion(in %))\n";
				for(int k=1; k<30; k++)
				{
					cout <<"("<< k << "," << ((graphletcount[k]+1)/totalgraphlet)*100<<")";
				}
				cout << "\nGFD (";
				int k;
				for(k=1; k<29; k++)
				{
					cout << log10((graphletcount[k]+1)/totalgraphlet)<<",";
				}
				cout << log10((graphletcount[k]+1)/totalgraphlet);
				cout <<")\nRandom Walk ends\n";
				return;
			}		

			string gc = next->canonical_code();
			out3 << gc <<"\t"<<next->graphletid<<endl;	
			
			iter++;			
			//cout <<"iter:"<<iter <<endl;
			
			cit = visit_count.find(gc);
			if(cit != visit_count.end())
			{
				cit->second++;
			}
			else
			{
				visit_count.insert(make_pair(gc,1));
			}
		
			current->clear_edge_stat();
			current->degree_sum.clear();
			delete current;
			
		}
	}

	/*! \fn graphlet * get_next(graphlet *current)
 		*  \brief A member function to get next node on graphlet space to jump from current. 
		*	 Acceptance probability calculation of Metropolis-Hastings 
		* algorithm is implemented here.
		*	\param current a graphlet type pointer.
		* \return a graphlet pointer 
	*/
	graphlet * get_next(graphlet *current)
	{
			srand(time(NULL));
			int random;
			double get_uni;
			double acceptance_prob;
			set<graphlet*>::iterator sit;
			vector<int> v;
			vector<int>::iterator it;
			int i;
			double reject=0;
			while(1){
			
				random = boost_get_a_random_number(0,current->get_degree());
				
				//new way to generate random number

				/*std::random_device rd;
    			std::mt19937 gen(rd());
    			std::uniform_int_distribution<> dis(0, current->get_degree()-1);
				random = dis(gen);*/

				graphlet *g1;
				if(current->get_size()==3)
				{
					int sum=0;
					int nindex=0;
					v.clear();
					current->getVertices(v);
					
					int index = random;
					for(int i=0; i<4;i++)
					{
						
						if(current->degree_sum[i]==0)continue;
						sum = sum + current->degree_sum[i];
						
						if(sum > random)
						{
							nindex = i;
							
							break;
						}
						else{
							index = index - current->degree_sum[i];
						}
						
					}
					
					if(nindex==3)
					{
						v.clear();
						current->getVertices(v);
						g1 = new graphlet (v[0],v[1],v[2],neighbors_list[random]);
						if(current->get_edge_status(v[0],v[1]))
								g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
								g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
								g1->set_edge_status(v[1],v[2]);
						
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					else if(nindex==0)
					{
						int ver = v[0];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					else if(nindex==1)
					{
						int ver = v[1];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					else if(nindex==2)
					{
						int ver = v[2];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
				}
				else if(current->get_size()==4)
				{
					int sum=0;
					int nindex=0;
					v.clear();
					current->getVertices(v);
					
					int index = random;
					for(int i=0; i<6;i++)
					{
						
						if(current->degree_sum[i]==0)continue;
						sum = sum + current->degree_sum[i];
						
						if(sum > random)
						{
							nindex = i;
							
							break;
						}
						else{
							index = index - current->degree_sum[i];
						}
						
					}
					
					if(nindex==4)
					{
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					
					else if(nindex==5)
					{
						
						it = find(v.begin(),v.end(),neighbors_list[random]);
						v.erase(it);
						g1 = new graphlet (v[0],v[1],v[2]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					
					else if(nindex == 0)
					{
						
						int ver = v[0];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	

					else if(nindex == 1)
					{
						
						int ver = v[1];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
					else if(nindex == 2)
					{
						
						int ver = v[2];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
					else if(nindex == 3)
					{	
						
						int ver = v[3];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
						
				}
				
				else if(current->get_size()==5)
				{
					int sum=0;
					int nindex=0;
					v.clear();
					current->getVertices(v);
					
					int index = random;
					for(int i=0; i<6;i++)
					{
						
						if(current->degree_sum[i]==0)continue;
						sum = sum + current->degree_sum[i];
						
						if(sum > random)
						{
							nindex = i;
							
							break;
						}
						else{
							index = index - current->degree_sum[i];
						}
						
					}
					
					if(nindex==5)
					{
						
						
						it = find(v.begin(),v.end(),neighbors_list[random]);
						v.erase(it);
						g1 = new graphlet (v[0],v[1],v[2],v[3]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					
					else if(nindex==0)
					{
						
						int ver = v[0];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
					}
					
					else if(nindex == 1)
					{
						
						int ver = v[1];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	

					else if(nindex == 2)
					{
						
						int ver = v[2];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
					else if(nindex == 3)
					{
						
						int ver = v[3];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
					else if(nindex == 4)
					{	
						
						int ver = v[4];
						it = find(v.begin(),v.end(),ver);
						v.erase(it);
						
						g1 = new graphlet (v[0],v[1],v[2],v[3],neighbors_list[random]);
						
						if(current->get_edge_status(v[0],v[1]))
							g1->set_edge_status(v[0],v[1]);
						if(current->get_edge_status(v[0],v[2]))
							g1->set_edge_status(v[0],v[2]);
						if(current->get_edge_status(v[0],v[3]))
							g1->set_edge_status(v[0],v[3]);
						if(current->get_edge_status(v[1],v[2]))
							g1->set_edge_status(v[1],v[2]);		
						if(current->get_edge_status(v[1],v[3]))
							g1->set_edge_status(v[1],v[3]);
						if(current->get_edge_status(v[2],v[3]))
							g1->set_edge_status(v[2],v[3]);
						g->get_graphletid(g1);
						
						process_graphlet(g1,next_neighbors_list);
			
					}	
						
				}
				
				if(g1->get_degree()  < 1E-03 )
				{
					cout <<"g1=" <<g1->graphletid;
					g1->print_graphlet();
					
					g1->clear_edge_stat();
					g1->degree_sum.clear();
					delete g1;
					reject++;
					continue;
				}
				acceptance_prob = (double)((current->get_degree())/(g1->get_degree()) >= 1)? 1:
				      (double)(current->get_degree())/(g1->get_degree());
			
				get_uni = random_uni01();
				//std::uniform_real_distribution<> uni_dis(0, 1);
				//get_uni = ((double) rand() / (RAND_MAX));
		
				if(get_uni <= acceptance_prob)
				{
					return g1;
				
				}
				else
				{
					g1->clear_edge_stat();
					g1->degree_sum.clear();
					delete g1;
					
					reject++;
				}
			}

	}

	/*! \fn void process_graphlet(graphlet *& g1, int *&neighbors_list)
 		*  \brief A member function to populate neighbors list of the current graphlet
		*	\param g1 an address of graphlet type pointer.
		*	\param neighbors_list an address of an interger type pointer.
	*/
	void process_graphlet(graphlet *& g1, int *&neighbors_list)
	{
		g->populate_neighbors(g1, neighbors_list, checkpoint);
		
		return;
		
	}
	
	

};

#endif

