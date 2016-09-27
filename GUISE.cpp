//! \file GUISE.cpp - main function of this project.
#include <iostream>
#include <vector>
#include "graphGraphlet.h"
#include "time.h"
#include "time_tracker.h"
#include "random_walk.h"
using namespace std;
//#define MAX 1
char* datafile;
//char* selection_method;
//char* reduction_factor;
double iteration;

void print_usage(char *prog) {
  cerr<<"Usage: "<<prog<<" -d data-file -i iteration"<<endl;
  exit(0);
}

void parse_args(int argc, char* argv[]) {
  if(argc<5) {
    print_usage(argv[0]);
  }
  for (int i=1; i < argc; ++i){
    if (strcmp(argv[i], "-d") == 0){
    	datafile=argv[++i];
    }
    else if(strcmp(argv[i], "-i") == 0){
    	iteration=atof(argv[++i]);
   }
  }
}//end parse_args()

struct eqtri
{
	bool operator()(graphlet* tr1,graphlet* tr2) const
	{
		vector<int> T1,T2;
		tr1->getVertices(T1);
		tr2->getVertices(T2);
		if(T1.size()!=3 or T2.size()!=3){
			cout<<"error comT!!!\n";
			exit(1);
		}
		if(T1[0]<T2[0]) return true;
		else if(T1[0]>T2[0]) return false;
		else{
			if(T1[1]<T2[1]) return true;
			else if(T1[1]>T2[1]) return false;
			else{
				if(T1[2]<T2[2]) return true;
				else return false;
			}
		}
		
	}
};

/*! \fn int main(int argc, char *argv[])
 		*  \brief main function to run: ./GUISE -d data-file -i iteration
		*  \param argc integer
		*  \param argv array of char pointer
		*  \return integer
 		*/
int main(int argc, char *argv[]) {
	typedef struct induction ind;
	parse_args(argc, argv);
	graph_* g=new graph_(datafile);
	double iter=iteration;
	
	random_walk * rw = new random_walk(iter,g);
	time_tracker tt;
	tt.start();
	rw->walk();
	tt.stop();
	cout <<"Total time taken = "<<tt.print()<<" seconds"<<endl;
	
	delete g;
}
