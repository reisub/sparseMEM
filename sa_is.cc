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

int sa_is(std::string &s, unsigned int *SA) {
  // TODO checks
  int n = s.size();
  if(n == 0) {
    return 0;
  } else if (n == 1) {
    SA[0] = 0;
    return 0;
  } else {
    // TODO calculate
    std::cout << "TODO" << std::endl;
    return 0;
  }
}