#include <iostream>
#include <fstream>
#include <vector>

struct suffix {
  unsigned int index;
  std::string suffix_string;
};

bool compare(suffix i, suffix j) {
  return i.suffix_string < j.suffix_string ;
}

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

  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <file containing reference string> <file containing string for matching>" << std::endl;
    exit(-1);
  }

  std::ifstream ref_file(argv[1]);
  std::ifstream match_file(argv[2]);

  std::string ref_string;
  std::string match_string;

  std::getline(ref_file, ref_string);
  std::getline(match_file, match_string);

  std::cout << "Reference string: " << ref_string << std::endl;
  std::cout << "Match string:     " << match_string << std::endl;

  std::vector<int> sa;

  suffix_array(ref_string, sa);

  for (int i = 0; i < sa.size(); ++i) {
    std::cout << "[" << i << "] " << sa[i] << std::endl;
  }

  return 0;
}