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

// finds the head (end == false) or tail (end == true) index of each character
void get_buckets(std::string &s, unsigned int *buckets, unsigned int alphabet_size, bool end) {
  unsigned int sum = 0;
  // compute bucket sizes
  for (int i = 0; i < s.size(); ++i) {
    buckets[s[i]]++;
  }
  for (int i = 0; i < alphabet_size; ++i) {
    sum += buckets[i];
    if (end) {
      buckets[i] = sum;
    } else {
      buckets[i] = sum - buckets[i];
    }
  }
}

int sa_is(std::string &s, unsigned int *SA, unsigned int n, unsigned int alphabet_size, unsigned int character_size) {

  if (SA == NULL || n < 0 || alphabet_size <= 0) {
    return -1;
  }

  if(n <= 1) {
    if (n == 1) {
      SA[0] = 0;
    }
    return 0;
  }

  bool *types = new bool[n];
  unsigned int *buckets = new unsigned int[alphabet_size](); // used for storing starts and ends of buckets

  type_array(s, types);
  get_buckets(s, buckets, alphabet_size, true);

  for (int i = 0; i < n; ++i) {
    SA[i] = 0;
  }

  for (int i = 1; i < n; ++i) {
    if (is_lms(types, i)) {
      buckets[s[i]]--;
      SA[buckets[s[i]]] = i;
    }
  }

  // TODO induce SA L-type
  // TODO induce SA S-type

  // TEST CODE START

  std::vector<int> lms_substrs(n); // Definition 3.4: (Sample Pointer Array) P1
  std::vector<int> s1(n); // zero-initialized

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

  // TEST CODE END

  // deallocate all memory to prevent leaks
  delete[] types;
  delete[] buckets;

  return 0;
}
