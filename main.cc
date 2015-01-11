#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "fasta_parser.h"
#include "sa_is.h"
#include "search.h"

struct suffix {
  unsigned int index;
  std::string suffix_string;
};

bool compare(suffix i, suffix j) {
  return i.suffix_string < j.suffix_string ;
}

// Creates a sufix array for the given string using a simple and slow method
void suffix_array(std::string str, std::vector<int> &sa) {
  std::vector<suffix> suffixes;

  for (unsigned int i = 0; i < str.size(); ++i) {
    suffix *s = new suffix;
    s->index = i;
    s->suffix_string = str.substr(i);
    suffixes.push_back(*s);
    delete s;
  }

  std::sort(suffixes.begin(), suffixes.end(), compare);
  for (unsigned int i = 0; i < suffixes.size(); ++i) {
    sa.push_back(suffixes[i].index);
  }
}

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
  int N = ref_string.length();

  std::vector<int> sa;
  sa.reserve(N);
  bool *types = new bool[N];

  suffix_array(ref_string, sa);
  type_array(ref_string.c_str(), types, N, sizeof(char));

  for (unsigned int i = 0; i < sa.size() ; ++i) {
    std::cout << "[" << i << "]\t" << sa[i] << (types[sa[i]] ? "\tS\t" : "\tL\t")
    << ref_string.substr(sa[i]) << std::endl;
  }

  int *SA = new int[N];
  int *sparseSA = new int[N / K];
  int *sparseISA = new int[N / K];
  int *sparseLCP = new int[N / K];
  short unsigned int A_, C_, T_, G_, BROJ;
  A_ = C_ = T_ = G_ = BROJ =0;
  // Creates Suffix Array using SA_IS algorithm
  sa_is(ref_string.c_str(), SA, N, 256, sizeof(char));
  for (int i = 0; i < N; i++){
	if (ref_string.substr(sa[i])[0] == 'A') A_++;
	if (ref_string.substr(sa[i])[0] == 'C') C_++;
	if (ref_string.substr(sa[i])[0] == 'T') T_++;
	if (ref_string.substr(sa[i])[0] == 'G') G_++;	
  }

  
  sparseSA[0] = SA[0 * K];
  
  
  // Generate Sparse Suffix Array, A, C, T, G
  int j =0;
  for (int i = 1; i < N; ++i) {
  
    if(A_>K){
		sparseSA[j] = SA[i * K];
		A_ = A_-K;
	}
	
	else if (C_>K){
		sparseSA[j] = SA[i * K + A_];
		C_ = C_-K;
	}
	
	else if (T_>K){
		sparseSA[j] = SA[i * K + A_ + C_];
		T_ = T_-K;
	}
	
	else if (G_ >K){
		sparseSA[j] = SA[i * K + A_ + C_ + T_];
		G_ = G_ -K;
	}
	
	else break;
    
	j++;
	//sparseSA[i] = SA[i * K];
  }
  
  BROJ = j;

  
  
  // Generate ISA A
  for(int i = 0; i < j; i++) {
		sparseISA[sparseSA[i]/K] = i;
  }

  // Generate LCP
  int h = 0;
  for(int i = 0; i < N/K ; i+=K) {
    int m = sparseISA[i];
    if(m==0) {
      sparseLCP[m] = 0;
    }
    else {
      int j = sparseSA[m-1];
      while(i+h < N && j+h < N && ref_string[i+h] == ref_string[j+h]) {
        h++;
      }
      sparseLCP[m] = h;
    }
    h = std::max(0, h - K);
  }

  printf("\nSparse SA: ");
  for (int i = 0; i < N/K; ++i)
    printf("%d ", sparseSA[i]);

  printf("\nSparse ISA: ");
  for (int i = 0; i < N/K; ++i)
    printf("%d ", sparseISA[i]);

  printf("\nSparse LCP: ");
  for (int i = 0; i < N/K; ++i)
    printf("%d ", sparseLCP[i]);
  printf("\n");


  // Search for MEMs:
  printf("\tRef.\tQuery\tLength\n");
  int p0 = 0;
  MEM(p0, ref_string, sparseISA, sparseLCP, sparseSA, query_string, K, N, L);

  return 0;
}


