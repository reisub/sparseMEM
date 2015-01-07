#ifndef SA_IS_H
#define SA_IS_H

#include <cstdlib>

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define TERMINATION_CHAR '$'
#define S_TYPE true
#define L_TYPE false

/*
  * Creates a type array, true means S-Type, false means L-Type.
*/
void type_array(const char *s, bool *types, int n);

bool is_lms(bool *types, int i);

int sa_is(const char *s, int *SA, int n, int alphabet_size);

#endif /* SA_IS_H */
