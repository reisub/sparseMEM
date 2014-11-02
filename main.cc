#include <cstdlib>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

/*
  * Creates a type array, true means S-Type, false means L-Type.
*/
void type_array(std::string str, bool *types) {

  unsigned int last_index = str.size() - 1;

  // the last type depends if we have an explicit termination character in the string
  if (str[last_index] == TERMINATION_CHAR) {
    types[last_index] = true;
  } else {
    types[last_index] = false;
  }

  for (int i = last_index - 1; i >= 0; --i) {
    if (str[i] < str[i + 1]) {
      types[i] = true;
    } else if (str[i] > str[i + 1]) {
      types[i] = false;
    } else {
      types[i] = types[i + 1];
    }
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
  sa.reserve(ref_string.size());
  bool *types = new bool[ref_string.size()];

  suffix_array(ref_string, sa);
  type_array(ref_string, types);

  for (int i = 0; i < sa.size(); ++i) {
    std::cout << "[" << i << "]\t" << sa[i] << (types[sa[i]] ? "\tS\t" : "\tL\t") << ref_string.substr(sa[i]) << std::endl;
  }

  return 0;
}