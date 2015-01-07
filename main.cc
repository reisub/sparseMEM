#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "sa_is.h"

#define TERMINATION_CHAR '$'

struct suffix {
  unsigned int index;
  std::string suffix_string;
};

bool compare(suffix i, suffix j) {
  return i.suffix_string < j.suffix_string ;
}

/*
  * Creates a sufix array for the given string using a simple and slow method
*/
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

  if(argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file containing reference string>" << std::endl;
    exit(-1);
  }

  std::ifstream ref_file(argv[1]);

  if(!ref_file.is_open()) {
    std::cerr << "There was a problem openning the file \"" << argv[1] << "\"!\nAborting." << std::endl;
    exit(-2);
  }

  std::string ref_string;
  std::getline(ref_file, ref_string);

  std::cout << "String: " << ref_string << std::endl;

  std::vector<int> sa;
  sa.reserve(ref_string.size());
  bool *types = new bool[ref_string.size()];

  suffix_array(ref_string, sa);
  type_array(ref_string.c_str(), types, ref_string.size());

  for (unsigned int i = 0; i < sa.size(); ++i) {
    std::cout << "[" << i << "]\t" << sa[i] << (types[sa[i]] ? "\tS\t" : "\tL\t")
    << ref_string.substr(sa[i]) << std::endl;
  }

  sa_is(ref_string.c_str(), new unsigned int[ref_string.size()], ref_string.size(), 128);

  return 0;
}
