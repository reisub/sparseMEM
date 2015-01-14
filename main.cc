#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <cmath>

#include "fasta_parser.h"
#include "sa_is.h"
#include "search.h"

int main(int argc, char *argv[]) {

  if(argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <file containing reference string> <file containing query string> <index level of sparseSA, K-SA> <size of minimal match>" << std::endl;
    exit(-1);
  }

  std::ifstream ref_file(argv[1]);
	std::ifstream query_file(argv[2]);

  if(!ref_file.is_open()) {
    std::cerr << "There was a problem openning the file \"" << argv[1] << "\"!\nAborting." << std::endl;
    exit(-2);
  }

  if(!query_file.is_open()) {
    std::cerr << "There was a problem openning the file \"" << argv[2] << "\"!\nAborting." << std::endl;
    exit(-2);
  }

  // Read in additions constrains (K - step in suffix array, L - minimal printed match) K > L !
  int K = atoi(argv[3]);
  int L = atoi(argv[4]);
  string genom_file = argv[1];
  string genom_query_file = argv[2];
  
  // Read in reference string (fasta format)
	std::string ref_string;
	std::vector<string> refdescr;
	std::vector<long> startpos;
	fasta_parser(genom_file, ref_string, refdescr, startpos);

  // Read in query string (fasta format)
	std::string query_string;
	std::vector<string> querydescr;
	std::vector<long> q_startpos;
  fasta_parser(genom_query_file, query_string, querydescr, q_startpos);
  
  // for loop to loop on all genoms in file TODO!
  
  //for ()

  // pad string with termination character $ (if needed)
  int pad_length = K - (ref_string.size() % K);
  if (ref_string.size() % K == 0) pad_length = 0;
  ref_string.append(1, TERMINATION_CHAR);

  // Create arrays to use for algorithm
  int N = ref_string.length();
  int *SA = new int[N + pad_length];
  int *sparseSA = new int[N / K + K];
  int *sparseISA = new int[N / K + K];
  int *sparseLCP = new int[N / K + K];

  
  // Creates Suffix Array using SA_IS algorithm
  sa_is(ref_string.c_str(), SA, N, 256, sizeof(char));

  // pad string with additional termination characters (doesn't effect the search)
  if (pad_length > 0){
    N = N + pad_length;
    for (int i=N-1; i>=pad_length; i--) SA[i] = SA[i-pad_length];
    for(int i =0; i<pad_length; i++) SA[i] = ref_string.length() - 1 + (pad_length - i);
    ref_string.append(pad_length, TERMINATION_CHAR);
  }

  
  // Generate sparse SA
  int size = 0;
  for (int i = 0; i < N; i++) {
    if (SA[i] % K == 0){
      sparseSA[size++] = (int) SA[i];
    }
  }

  // Generate ISA
  for(int i = 0; i < size  ; i++) {
    sparseISA[sparseSA[i] / K] = i ;
  }

  // Generate LCP
  int matched = 0;
  int j = 0;
  int i = 0;
  for(int iter = 0; iter < size; iter++) {
    i = (int) sparseSA[iter];
    j = (int) sparseSA[iter - 1];
    if(iter == 0) sparseLCP[iter] = -1; // request for first member
    else {
      while((i + matched < N) && (j + matched < N) && 
        (ref_string[i + matched] == ref_string[j + matched])) 
          ++matched;
      sparseLCP[iter] = (int) matched;
      matched = 0;
    }
  }
  
  
  
  for(int i = 0; i < N; i++) cout << "[" << i <<"] " << ref_string.substr(SA[i]) << endl;
   
  cout << endl << "Sparse SA: ";
  for (int i = 0; i < size; ++i)
    cout << sparseSA[i] << "   ";
  cout << endl << "Sparse ISA: ";
  for (int i = 0; i < size; ++i)
    cout << sparseISA[i] << "   ";

  cout << endl << "Sparse LCP: ";
  for (int i = 0; i < size; ++i)
    cout << sparseLCP[i] << "   ";
  
  
  
  // Search the genom for MEMs
  int p0 = 0;
  MEM(p0, ref_string, sparseISA, sparseLCP, sparseSA, query_string, K, N, L);
  
  // Delete all resources 
  delete[] SA;
  delete[] sparseSA;
  delete[] sparseISA;
  delete[] sparseLCP;
  
  return 0;
}


