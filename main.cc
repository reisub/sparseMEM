#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "fasta_parser.h"
#include "sa_is.h"

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
    std::cerr << "Usage: " << argv[1] << " <file containing reference string>" << std::endl;
		std::cerr << "Usage: " << argv[2] << " <file containing query string>" << std::endl;
		std::cerr << "Usage: " << argv[3] << " <index level of sparseSA, K-SA>" << std::endl;
		std::cerr << "Usage: " << argv[4] << " <size of minimal match>" << std::endl;

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

	std::string ref_string;
	std::vector<string> refdescr;
	std::vector<long> startpos;

	fasta_parser(argv[1], ref_string, refdescr, startpos);
	
	std::int K = atoi(argv[3]);
  std::int L = atoi(argv[4]);
  std::int N = ref_string;

	std::string query_string;
  std::getline(query_file, query_string);
  std::cout << "Query: " << query_string << std::endl;

  std::vector<int> sa;
  sa.reserve(ref_string.size());
  bool *types = new bool[ref_string.size()];

  suffix_array(ref_string, sa);
  type_array(ref_string.c_str(), types, ref_string.size(), sizeof(char));

  for (unsigned int i = 0; i < sa.size(); ++i) {
    std::cout << "[" << i << "]\t" << sa[i] << (types[sa[i]] ? "\tS\t" : "\tL\t")
    << ref_string.substr(sa[i]) << std::endl;
  }

  int *SA = new int[N];
  int *sparseSA = new int[N / K];
  int *ISA = new int[N)];
  
  // Creates Suffix Array using SA_IS algorithm
  sa_is(ref_string.c_str(), SA, N, 256, sizeof(char));
  
  // Create 
  // Creates Invese SA
  for ( int i = 0; i < N; ++i) {;}
  
  for ( int i = 0; i < N); ++i) {
    printf("%d ", SA[i]);
    if (SA[i] != sa[i]) {
      std::cout << "<-!!! ";
    }
  }
  printf("\n");
  
  

  return 0;
}
