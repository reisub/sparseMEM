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

  // Read in reference string (fasta format)
	std::string ref_string;
	std::vector<string> refdescr;
	std::vector<long> startpos;
	fasta_parser(argv[1], ref_string, refdescr, startpos);

  // Read in query string (fasta format)
	std::string query_string;
	std::vector<string> querydescr;
	std::vector<long> q_startpos;
  fasta_parser(argv[2], query_string, querydescr, q_startpos);

  int K = atoi(argv[3]);
  int L = atoi(argv[4]);

  // pad string with $
  int pad_length = K - (ref_string.size() % K);
  if (ref_string.size() % K == 0) pad_length = 0;
  ref_string.append(1, TERMINATION_CHAR);

  int N = ref_string.length();

  int *SA = new int[N + pad_length];
  int *sparseSA = new int[N / K + K];
  int *sparseISA = new int[N / K + K];
  int *sparseLCP = new int[N / K + K];

  // Creates Suffix Array using SA_IS algorithm

  sa_is(ref_string.c_str(), SA, N, 256, sizeof(char));

  if (pad_length > 0){
	  N = N + pad_length;
	  for (int i=N-1; i>=pad_length; i--) SA[i] = SA[i-pad_length];
	  for(int i =0; i<pad_length; i++) SA[i] = ref_string.length() - 1 + (pad_length - i);
	  ref_string.append(pad_length, TERMINATION_CHAR);
  }

  

  int size = 0;
  for (int i = 0; i < N; i++) {
    if (SA[i] % K == 0){
      sparseSA[size++] = (int) SA[i];
    }
  }
/*
  if(pad_length % K != 0) {
    std::cout << "pad_length: " << pad_length << std::endl;
    ref_string.append(pad_length, TERMINATION_CHAR);
    N = ref_string.size();
    j += pad_length - 1;

    for (int i = N - 1; i >= pad_length - 1; --i) {
      sparseSA[i] = sparseSA[i - (pad_length) + 1];
    }
    for (int i = 0; i <= pad_length; i += K) {
      sparseSA[i] = N - i - 1;
    }
  }
*/

  // Generate ISA A
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
  
  /*
  cout << endl << "Sparse SA: ";
  for (int i = 0; i < size; ++i)
    cout << sparseSA[i] << "   ";

  cout << endl << "Sparse ISA: ";
  for (int i = 0; i < size; ++i)
    cout << sparseISA[i] << "   ";

  cout << endl << "Sparse LCP: ";
  for (int i = 0; i < size; ++i)
    cout << sparseLCP[i] << "   ";
  */
  // Search for MEMs:
  //cout << endl << "\tRef.\tQuery\tLength" << endl;
  int p0 = 0;
  MEM(p0, ref_string, sparseISA, sparseLCP, sparseSA, query_string, K, N, L);

  return 0;
}


