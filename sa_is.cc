#include "sa_is.h"

inline int chr(const char *s, int i, int cs) {
  if (cs == sizeof(int)) {
    return ((int *) s)[i];
  } else {
    return ((char *) s)[i];
  }
}

// Creates a type array, true means S-Type, false means L-Type.
void type_array(const char *s, bool *types, int n, int cs) {
  types[n - 1] = S_TYPE;
  types[n - 2] = L_TYPE;

  for (int i = n - 3; i >= 0; --i) {
    if (chr(s, i, cs) < chr(s, i + 1, cs)) {
      types[i] = S_TYPE;
    } else if (chr(s, i, cs) > chr(s, i + 1, cs)) {
      types[i] = L_TYPE;
    } else {
      types[i] = types[i + 1];
    }
  }
}

inline bool is_lms(bool *types, int i) {
  if(i > 0 && types[i] == S_TYPE && types[i - 1] == L_TYPE) {
    return true;
  } else {
    return false;
  }
}

// finds the head (end == false) or tail (end == true) index of each character
void get_buckets(const char *s, int n, int *buckets, int alphabet_size, bool end, int cs) {
  unsigned int sum = 0;
  for (int i = 0; i <= alphabet_size; ++i) {
    buckets[i] = 0;
  }
  // compute bucket sizes
  for (int i = 0; i < n; ++i) {
    buckets[chr(s, i, cs)]++;
  }
  for (int i = 0; i <= alphabet_size; ++i) {
    sum += buckets[i];
    if (end) {
      buckets[i] = sum;
    } else {
      buckets[i] = sum - buckets[i];
    }
  }
}

void induce_sa_l(const char *s, int *SA, int n, bool *types, int *buckets, int alphabet_size, int cs) {
  // find starts of buckets
  get_buckets(s, n, buckets, alphabet_size, false, cs);
  int tmp;
  for (int i = 0; i < n; ++i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == L_TYPE) {
      SA[buckets[chr(s, tmp, cs)]] = tmp;
      buckets[chr(s, tmp, cs)]++;
    }
  }
}

void induce_sa_s(const char *s, int *SA, int n, bool *types, int *buckets, int alphabet_size, int cs) {
  // find ends of buckets
  get_buckets(s, n, buckets, alphabet_size, true, cs);
  int tmp;
  for (int i = n - 1; i >= 0; --i) {
    tmp = SA[i] - 1;
    if (tmp >= 0 && types[tmp] == S_TYPE) {
      buckets[chr(s, tmp, cs)]--;
      SA[buckets[chr(s, tmp, cs)]] = tmp;
    }
  }
}

int sa_is(const char *s, int *SA, int n, int alphabet_size, int cs) {

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
  int *buckets = new int[alphabet_size + 1]();

  type_array(s, types, n, cs);
  get_buckets(s, n, buckets, alphabet_size, true, cs);

  for (int i = 0; i < n; ++i) {
    SA[i] = -1;
  }

  for (int i = 1; i < n; ++i) {
    if (is_lms(types, i)) {
      buckets[chr(s, i, cs)]--;
      SA[buckets[chr(s, i, cs)]] = i;
    }
  }

  induce_sa_l(s, SA, n, types, buckets, alphabet_size, cs);
  induce_sa_s(s, SA, n, types, buckets, alphabet_size, cs);

  delete[] buckets;

  // compact all the sorted substrings into the first n1 items of SA
  int n1 = 0;
  for (int i = 0; i < n; ++i) {
    if (is_lms(types, SA[i])) {
      SA[n1] = SA[i];
      n1++;
    }
  }

  for (int i = n1; i < n; ++i) {
    SA[i] = -1;
  }

  int name = 0, prev = -1;
  for (int i = 0; i < n1; ++i) {
    int pos = SA[i];
    bool diff = false;
    for (int d = 0; d < n; ++d) {
      if (prev == -1
        || chr(s, pos + d, cs) != chr(s, prev + d, cs)
        || types[pos + d] != types[prev + d]) {
        diff = true;
        break;
      } else if (d > 0 && (is_lms(types, pos + d) || is_lms(types, prev + d))) {
        break;
      }
    }
  for (int i = 0; i < n; ++i) {
      std::cout << SA[i] << " ";
      }
  std::cout << std::endl;


    if (diff) {
      name++;
      prev = pos;
    }
    pos /= 2;
    SA[n1 + pos] = name - 1;
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
  int *s1 = SA + n - n1;

  if (name < n1) {
    sa_is((char *)s1, SA1, n1, name - 1, sizeof(int));
  } else {
    for (int i = 0; i < n1; ++i) {
      SA1[s1[i]] = i;
    }
  }

  /*
  * STAGE 3: induce the result for the original problem
  */

  buckets = new int[alphabet_size + 1]();
  get_buckets(s, n, buckets, alphabet_size, true, cs);

  for (int i = 1, j = 0; i < n; ++i) {
    if (is_lms(types, i)) {
      s1[j] = i;
      j++;
    }
  }

  for (int i = 0; i < n1; ++i) {
    SA1[i] = s1[SA1[i]];
  }

  for (int i = n1; i < n; ++i) {
    SA[i] = -1;
  }

  for (int i = n1 - 1, j = 0; i >= 0; --i) {
    j = SA[i];
    SA[i] = -1;
    buckets[chr(s, j, cs)]--;
    SA[buckets[chr(s, j, cs)]] = j;
  }

  induce_sa_l(s, SA, n, types, buckets, alphabet_size, cs);
  induce_sa_s(s, SA, n, types, buckets, alphabet_size, cs);

  // deallocate all memory to prevent leaks
  delete[] types;
  delete[] buckets;

  return 0;
}
