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

  for (int i = 0; i < str.size(); ++i) {
    suffix *s = new suffix;
    s->index = i;
    s->suffix_string = str.substr(i);
    suffixes.push_back(*s);
    delete s;
  }

  std::sort(suffixes.begin(), suffixes.end(), compare);
  for (int i = 0; i < suffixes.size(); ++i) {
    sa.push_back(suffixes[i].index);
  }
}

int main(int argc, char *argv[]) {

  if(argc != 4) {
    std::cerr << "Usage: " << argv[0] << "<match minimum length> <file containing reference string> <file containing string for matching>" << std::endl;
    exit(-1);
  }

  unsigned int minimum_size = atoi(argv[1]);

  std::ifstream ref_file(argv[2]);
  std::ifstream match_file(argv[3]);

  std::string ref_string;
  std::string match_string;

  std::getline(ref_file, ref_string);
  std::getline(match_file, match_string);

  std::cout << "Reference string: " << ref_string << std::endl;
  std::cout << "Match string:     " << match_string << std::endl;

  std::vector<int> sa;
  std::vector<std::string::iterator> lms_substr_pointers; // Definition 3.4: (Sample Pointer Array) P1
  sa.reserve(ref_string.size());
  bool *types = new bool[ref_string.size()];

  suffix_array(ref_string, sa);
  type_array(ref_string, types);

  for (int i = 0; i < sa.size(); ++i) {
    std::cout << "[" << i << "]\t" << sa[i] << (types[sa[i]] ? "\tS\t" : "\tL\t")
    << ref_string.substr(sa[i]) << std::endl;
  }

  std::cout << std::endl << ref_string << std::endl;
  for (int i = 0; i < ref_string.size(); ++i) {
    std::cout << (types[i] ? "S" : "L");
  }
  std::cout << std::endl;
    for (int i = 0; i < ref_string.size(); ++i) {
    std::cout << (is_lms(types, i) ? "*" : " ");
    if(is_lms(types, i)) {
      lms_substr_pointers.push_back(ref_string.begin() + i);
    }
  }
  std::cout << std::endl;

  sa_is(ref_string, new unsigned int[ref_string.size()]);

  return 0;
}