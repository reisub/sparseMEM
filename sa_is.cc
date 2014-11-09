#include "sa_is.h"

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

bool is_lms(bool *types, unsigned int i) {
  if(i > 0 && types[i] && !types[i - 1]) {
    return true;
  } else {
    return false;
  }
}

int sa_is(std::string &s, unsigned int *SA, unsigned int n, unsigned int k, unsigned int start_index) {
  bool *types = new bool[n];
  type_array(s, types);

  std::vector<int> lms_substrs(n); // Definition 3.4: (Sample Pointer Array) P1
  std::vector<int> s1(n); // zero-initialized
  std::vector<int> buckets(k); // zero-initialized

  std::cout << std::endl << s << std::endl;
  for (int i = 0; i < s.size(); ++i) {
    std::cout << (types[i] ? "S" : "L");
  }
  std::cout << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << (is_lms(types, i) ? "*" : " ");
    if(i < (n - 1) && is_lms(types, i)) {
      lms_substrs.push_back(i);
    }
  }
  std::cout << std::endl;

  // TODO calculate
  std::cout << "TODO" << std::endl;
  return 0;
}
