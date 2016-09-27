//! \file random.cpp - implementation of random number generator functions
/*
#ifndef _RANDOM_H
#define _RANDOM_H_

#include <boost/random.hpp>
#include <ctime>
#include <cmath>

*/

#include "random.h"
using namespace std;

static boost::mt19937 generator(static_cast<unsigned> (std::time(0)));
/*! \fn double random_uni01() 
 		*  \brief A function to generate a uniform random number between 0 and 1.
 		*  \return a double
 		*/
double random_uni01() {
  boost::uniform_real<> uni_dist(0,1);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > uni(generator, uni_dist);

  return uni();
}

/*! \fn unsigned int get_a_random_number(int lowest, int highest) 
 		*  \brief A function to return a random number between lowest(including) and highest(excluding).
 		*  \param lowest an integer
		*  \param highest an integer
		*  \return an unsigned integer
 		*/
unsigned int get_a_random_number(int lowest, int highest) {
  if (highest < lowest) {
    cout << "ERROR In random_number_generator: Higher value is smaller than lower\n";
    exit(1);
  }
  unsigned int random_integer;
  int range=(highest-lowest);
  random_integer = lowest + rand()%range;
  return random_integer;
}

/*! \fn unsigned int boost_get_a_random_number(int lowest, int highest) 
 		*  \brief A function to return a random number between lowest(including) and highest(excluding) using boost.
 		*  \param lowest an integer
		*  \param highest an integer
		*	 \return an unsigned integer
 		*/

unsigned int boost_get_a_random_number(int lowest, int highest) {
  if (highest < lowest) {
    cout << "ERROR In random_number_generator: Higher value is smaller than lower\n";
    exit(1);
  }
  //boost::mt19937 rng; 
  boost::uniform_int<> range_dist(lowest,highest-1);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
           int_ran_gen(generator, range_dist);             // glues randomness with mapping
  return int_ran_gen();
}

/*! \fn unsigned int randomWithDiscreteProbability(const vector<double>& accum_prob_vec) 
 		*  \brief A function to return a random number with discrete probability; pass the cum. distribution vector.
 		*  \param accum_prob_vec a constant reference of double vector.
		*	 \return an unsigned integer
 		*/
unsigned int randomWithDiscreteProbability(const vector<double>& accum_prob_vec) {
  double x = random_uni01();
  return lower_bound(accum_prob_vec.begin(), accum_prob_vec.end(), x) - 
         accum_prob_vec.begin();
}
//#endif
