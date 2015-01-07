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
void type_array(std::string str, bool *types);

bool is_lms(bool *types, unsigned int i);

int sa_is(std::string &s, unsigned int *SA, unsigned int n, unsigned int alphabet_size, unsigned int character_size);

#endif /* SA_IS_H */
