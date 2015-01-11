#include <iostream>
#include "search.cc"
#include <string>
#include <assert.h>

using namespace std;



int main(){
	long SA[] = {11, 10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2};
	string S = "mississippi$";
	long LCP[] = {-1, 0, 1, 1, 4, 0, 0, 1, 0, 2, 1, 3};
	long ISA[] = {5, 4, 11, 9, 3, 10, 8, 2, 7, 6, 1, 0};
	char c = 'i';
	
	assert ( 3 == MAX3(1,2,3));
	assert ( 2 == MAX3(2,1,0));
	assert ( 5 == MAX3(5,5,5));
	cout << "Test passed. (MAX2 and MAX3 macros)";	
	
	assert(1 == binary_search_left(c, {0, 0, 11}, S, SA));
	assert(4 == binary_search_right(c, { 0, 0, 11}, S, SA));
	cout << "Test passed. (binary search)" << endl;

	interval_t ret_val = topdown(c, {0, 0, 11}, S, SA);
	assert (ret_val.depth == 1);
	assert (ret_val.start == 1);
	assert (ret_val.end == 4);
	cout << "Test passed. (topdown search)" << endl;

	interval_t empty, interval;

	string random_string = "ABC";
	empty = search_string(S, SA, random_string);
	assert (empty.start == 0);
	assert (empty.end == 0);
	cout << "Test passed. (search_string, empty response)" << endl;

	string str = "is";
	interval_t aa = search_string(S, SA, str);
	cout << aa.start << endl;
	assert (aa.start == 3);
	cout << aa.end << endl;
	assert (aa.end == 4);
	cout << "Test passed. (search_string, valid response)" << endl;

	c = 's';
	ret_val = topdown(c, {1, 3, 4}, S, SA);
	assert (ret_val.depth == 2);
	assert (ret_val.start == 3);
	assert (ret_val.end == 4);
	cout << "Test passed. (topdown search)" << endl;
	
	// missIppi
	c = 'i';
	ret_val = topdown(c, {3, 3, 4}, S, SA);
	assert (ret_val.depth == 4);
	assert (ret_val.start == 3);
	assert (ret_val.end == 4);
	cout << "Test passed. (topdown search)" << endl;

	// missiPpi
	c = 'p';
	ret_val = topdown(c, {4, 3, 4}, S, SA);
	assert (ret_val.depth == 5);
	assert (ret_val.start == 3);
	assert (ret_val.end == 3);
	cout << "Test passed. (topdown search)" << endl;
	


	// issiPpi
	string stringg("issippi$");
	ret_val = traverse (0 , {0, 0, 11}, 14, S, SA, stringg);
	assert (ret_val.depth == 8);
	assert (ret_val.start == 3);
	assert (ret_val.end == 3);
	cout << "Test passed. (topdown search)" << endl;


	//////////////////////////
	// array tests:
	long sparse_SA[] = {10, 4, 0, 8, 6, 2};
	string string_S = "mississippi$";
	long sparse_ISA[] = {2, 5, 1, 4, 3, 0};
	long sparse_LCP[] = {-1, 1, 0, 0, 0, 1};  
	string query_string ("issip");
	
	MEM(0, S, ISA, LCP, SA, query_string);
	
	return 0;
	
}

