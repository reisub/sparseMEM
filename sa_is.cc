#include "sa_is.h"

/*
  * Creates a type array, true means S-Type, false means L-Type.
*/
void type_array(const char *s, bool *types, int n) {
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

bool is_lms(bool *types, int i) {
  if(i > 0 && types[i] == S_TYPE && types[i - 1] == L_TYPE) {
    return true;
  } else {
    return false;
  }
}

// finds the head (end == false) or tail (end == true) index of each character
void get_buckets(const char *s, int n, int *buckets, int alphabet_size, bool end) {
  unsigned int sum = 0;
  // compute bucket sizes
  for (int i = 0; i < n; ++i) {
    buckets[static_cast<unsigned char>(s[i])]++;
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

void induce_sa_l(const char *s, int *SA, int n, bool *types, int *buckets, int alphabet_size) {
  // find starts of buckets
  get_buckets(s, n, buckets, alphabet_size, false);
  int tmp;
  for (int i = 0; i < n; ++i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == L_TYPE) {
      SA[buckets[static_cast<unsigned char>(s[tmp])]] = tmp;
      buckets[static_cast<unsigned char>(s[tmp])]++;
    }
  }
}

void induce_sa_s(const char *s, int *SA, int n, bool *types, int *buckets, int alphabet_size) {
  // find ends of buckets
  get_buckets(s, n, buckets, alphabet_size, true);
  int tmp;
  for (int i = n - 1; i >= 0; --i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == S_TYPE) {
      buckets[static_cast<unsigned char>(s[tmp])]--;
      SA[buckets[static_cast<unsigned char>(s[tmp])]] = tmp;
    }
  }
}

int sa_is(const char *s, int *SA, int n, int alphabet_size) {

  if (SA == NULL || n < 0 || alphabet_size < 1) {
    return -1;
  }

  if(n <= 1) {
    if (n == 1) {
      SA[0] = 0;
    }
    return 0;
  }

  /*
  * STAGE 1: reduce the problem by at least 1/2
  */

  bool *types = new bool[n];
  int *buckets = new int[alphabet_size]();

  type_array(s, types, n);
  get_buckets(s, n, buckets, alphabet_size, true);

  for (int i = 0; i < n; ++i) {
    SA[i] = -1;
  }

  std::cout << std::endl << s << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << (types[i] ? "S" : "L");
  }
  std::cout << std::endl;
  for (int i = 0; i < n; ++i) {
    std::cout << (is_lms(types, i) ? "*" : " ");
  }
  std::cout << std::endl;

  for (int i = 1; i < n; ++i) {
    if (is_lms(types, i)) {
      buckets[static_cast<unsigned char>(s[i])]--;
      SA[buckets[static_cast<unsigned char>(s[i])]] = i;
    }
  }

  induce_sa_l(s, SA, n, types, buckets, alphabet_size);
  induce_sa_s(s, SA, n, types, buckets, alphabet_size);

  delete[] buckets;

  // compact all the sorted substrings into the first n1 items of SA
  int n1 = 0;
  for (int i = 0; i < n; ++i) {
    if (is_lms(types, SA[i])) {
      SA[n1] = SA[i];
      n1++;
    }
  }

  for (int i = 0; i < n; ++i) {
    SA[i] = -1;
  }

  int name = 0, prev = -1;
  for (int i = 0; i < n1; ++i) {
    int pos = SA[i];
    bool diff = false;
    for (int d = 0; d < n; ++d) {
      if (prev == -1
        || s[pos + d] != s[prev + d]
        || types[pos + d] != types[prev + d]) {
        diff = true;
        break;
      } else if (d > 0 && (is_lms(types, pos + d) || is_lms(types, prev + d))) {
        break;
      }

      if (diff) {
        name++;
        prev = pos;
      }
      pos /= 2;
      SA[n1 + pos] = name - 1;
    }
  }

  for (int i = n - 1, j = n - 1; i >= n1; --i) {
    if (SA[i] >= 0) {
      SA[j] = SA[i];
      j--;
    }
  }

  /*
  * STAGE 2: solve the reduced problem
  */

  int *SA1 = SA;
  char *s1 = (char *) SA + n - n1;

  if (name < n1) {
    sa_is(s1, SA1, n1, name - 1);
  } else {
    for (int i = 0; i < n1; ++i) {
      SA1[static_cast<unsigned char>(s1[i])] = i;
    }
  }

  /*
  * STAGE 3: induce the result for the original problem
  */

  std::cout << 1 << std::endl;

  buckets = new int[alphabet_size]();
  get_buckets(s, n, buckets, alphabet_size, true);

  for (int i = 1, j = 0; i < n; ++i) {
    if (is_lms(types, i)) {
      s1[j] = i;
      j++;
    }
  }

  std::cout << 3 << std::endl;

  for (int i = 0; i < n1; ++i) {
    std::cout << SA1[i] << " " << n1 << std::endl;
    SA1[i] = s1[SA1[i]];
  }

  std::cout << 4 << std::endl;

  for (int i = n1; i < n; ++i) {
    SA[i] = -1;
  }

  std::cout << 5 << std::endl;

  for (int i = n1 - 1, j = 0; i >= 0; --i) {
    j = SA[i];
    SA[i] = -1;
    buckets[static_cast<unsigned char>(s[j])]--;
    SA[buckets[static_cast<unsigned char>(s[j])]] = j;
  }

  std::cout << 7 << std::endl;

  induce_sa_l(s, SA, n, types, buckets, alphabet_size);
  induce_sa_s(s, SA, n, types, buckets, alphabet_size);

  // deallocate all memory to prevent leaks
  delete[] types;
  delete[] buckets;

  return 0;
}
