#GUISE


**This software use MCMC sampling to return a set of sample induced subgraph of
size 3 to 5 using uniform distribution. If you use this code for any of your
work, please cite the following articles:**

- "Mahmudur Rahman, Mansurul Alam Bhuiyan, Mahmuda Rahman, Mohammad Al Hasan: GUISE: a uniform sampler for constructing frequency histogram of graphlets. KAIS  ’14 "
- "GUISE: Uniform Sampling of Graphlets for Large Graph Analysis, In Proc. of the 12th International Conference on Data Mining (ICDM-2012),Brussels , Belgium, December, 2012"


**This software require boost (Installation of Boost):
- in Ubuntu: sudo apt-get install libboost-all-dev

**To compile:**
- make clean
- make

**To Run:**
- ./GUISE -d <inputfile> -i <Iteration>

**example:** 
- ./GUISE -d footballUND.txt -i 20000

**Execution of GUISE will produce 3 output file in the current directory:**
- "visit_count"(contains visitation count of each graphlet)
- "graphlet_ineach_iteration"(contains graphlet visited by random walk in each iteration) 
- "graphlet_count"(count of each type of graphlet). GFD can be constructed from "graphlet_count" file.

**for any question contact:**

- Mohammad Al Hasan (Author) *Email: alhasan@cs.iupui.edu*
- Mansurul Bhuiyan *Email: mbhuiyan@iupui.edu*
- Mahmudur Rahman *Email: mmrahman@iupui.edu, prime059@gmail.com*
