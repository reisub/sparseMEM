#include <iostream>
#include "search.cc"
#include <string>
#include <assert.h>

using namespace std;

int main(){
  // Regual suffix array
	int SA[] = {11, 10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2};
	string S = "mississippi$";
	int LCP[] = {-1, 0, 1, 1, 4, 0, 0, 1, 0, 2, 1, 3};
	int ISA[] = {5, 4, 11, 9, 3, 10, 8, 2, 7, 6, 1, 0};
	char c = 'i';
	
	assert(1 == binary_search_left(c, {0, 0, 11}, S, SA));
	assert(4 == binary_search_right(c, { 0, 0, 11}, S, SA));
	cout << "Test passed. (binary search)" << endl;

	interval_t ret_val = topdown(c, {0, 0, 11}, S, SA);
	assert (ret_val.depth == 1);
	assert (ret_val.start == 1);
	assert (ret_val.end == 4);
	cout << "Test passed. (topdown search)" << endl;

	interval_t empty, interval;
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

	// Array tests
	cout << "Full MEM test (1), suffix array" << endl;
	string query_string ("issip");
	// K == 1 (step), N == size of S (11), L = min match to print (npr. 2) 
	MEM(0, S, ISA, LCP, SA, query_string, 1, 11, 2);
	
	// Sparse 
	cout << "Full MEM test (2), sparse suffix array" << endl;
	int sparse_SA[] = {10, 4, 0, 8, 6, 2};
	string string_S = "mississippi$";
	int sparse_ISA[] = {2, 5, 1, 4, 3, 0};
	int sparse_LCP[] = {-1, 1, 0, 0, 0, 1};
	int K = 2;
	int L = 3;
	MEM(0, string_S, sparse_ISA, sparse_LCP, sparse_SA, query_string, K, string_S.length(), L);
	
	// Sparse 2
	cout << "Full MEM test (3), sparse suffix array" << endl;
	int SA_a[]  =  {70,45,15,40,10,65,35,5,60,30,0,55,25,50,20};
  int ISA_a []= { 10, 7, 4, 2,14,12,9,6,3,1,13,11,8,5,0};
  int LCP_a []= {  -1 ,0  ,25   ,1  ,30   ,0   ,5  ,35   ,2  ,10  ,40   ,1  ,15   ,0  ,20 };
	query_string = "CTAACCCTAACCCTAACCCTAACCCTAACCCTAA";
	S = "CCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTAACCCTA";
	K = 5; L = 30;	
	MEM(0, S, ISA_a, LCP_a, SA_a, query_string, K, string_S.length(), L );
	
return 0;
}


