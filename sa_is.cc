#include "sa_is.h"

/*
  * Creates a type array, true means S-Type, false means L-Type.
*/
void type_array(const char *s, bool *types, unsigned int n) {
  types[n - 1] = S_TYPE;
  types[n - 2] = L_TYPE;

  for (int i = n - 3; i >= 0; --i) {
    if (s[i] < s[i + 1]) {
      types[i] = S_TYPE;
    } else if (s[i] > s[i + 1]) {
      types[i] = L_TYPE;
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
void get_buckets(const char *s, unsigned int n, unsigned int *buckets, unsigned int alphabet_size, bool end) {
  unsigned int sum = 0;
  // compute bucket sizes
  for (unsigned int i = 0; i < n; ++i) {
    buckets[static_cast<unsigned char>(s[i])]++;
  }
  for (unsigned int i = 0; i < alphabet_size; ++i) {
    sum += buckets[i];
    if (end) {
      buckets[i] = sum;
    } else {
      buckets[i] = sum - buckets[i];
    }
  }
}

void induce_sa_l(const char *s, unsigned int *SA, unsigned int n, bool *types, unsigned int *buckets, unsigned int alphabet_size) {
  // find starts of buckets
  get_buckets(s, n, buckets, alphabet_size, false);
  int tmp;
  for (unsigned int i = 0; i < n; ++i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == L_TYPE) {
      SA[buckets[static_cast<unsigned int>(s[tmp])]] = tmp;
      buckets[static_cast<unsigned int>(s[tmp])]++;
    }
  }
}

void induce_sa_s(const char *s, unsigned int *SA, unsigned int n, bool *types, unsigned int *buckets, unsigned int alphabet_size) {
  // find ends of buckets
  get_buckets(s, n, buckets, alphabet_size, true);
  int tmp;
  for (int i = n - 1; i >= 0; --i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == S_TYPE) {
      SA[buckets[static_cast<unsigned int>(s[tmp])]] = tmp;
      buckets[static_cast<unsigned int>(s[tmp])]--;
    }
  }
}

int sa_is(const char *s, unsigned int *SA, unsigned int n, unsigned int alphabet_size) {

  if (SA == NULL) {
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

  type_array(s, types, n);
  get_buckets(s, n, buckets, alphabet_size, true);

  for (unsigned int i = 0; i < n; ++i) {
    SA[i] = 0;
  }

  for (unsigned int i = 1; i < n; ++i) {
    if (is_lms(types, i)) {
      buckets[static_cast<unsigned char>(s[i])]--;
      SA[buckets[static_cast<unsigned char>(s[i])]] = i;
    }
  }

  induce_sa_l(s, SA, n, types, buckets, alphabet_size);
  induce_sa_s(s, SA, n, types, buckets, alphabet_size);

  // delete[] buckets;

  // TEST CODE START

  std::vector<int> lms_substrs(n); // Definition 3.4: (Sample Pointer Array) P1
  std::vector<int> s1(n); // zero-initialized

  std::cout << std::endl << s << std::endl;
  for (unsigned int i = 0; i < n; ++i) {
    std::cout << (types[i] ? "S" : "L");
  }
  std::cout << std::endl;
  for (unsigned int i = 0; i < n; ++i) {
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
