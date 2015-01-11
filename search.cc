#include <cmath>
#include "search.h"

using namespace std;


/**
* Returns left bound of interval. 
* Searches input string (S) for suffixes having "cmp_char" at "q_offset",
* using suffix array (SA).
*/
int binary_search_left (char cmp_char, interval_t interval, string &S, int *SA) {
	int depth = interval.depth;
	int start = interval.start;
	int end = interval.end;	
	
	if (cmp_char == S[SA[start] + depth]) {
		return start;
	}
	
	int middle;
	while ((end - start) > 1) {
		middle = (end + start) / 2;	
		if (cmp_char <= S[SA[middle] + depth]) {
			end = middle;		
		} else {
			start = middle;
		}
	}

	return end;
}

/**
* Returns right bound of interval. 
* Searches input string (S) for suffixes having "cmp_char" at "q_offset",
* using suffix array (SA).
*/
int binary_search_right (char cmp_char, interval_t interval, string &S, int *SA) {
	int depth = interval.depth;
	int start = interval.start;
	int end = interval.end;	

	if (cmp_char == S[SA[end] + depth]){
		return end;
	}
	
	int middle;
	while ((end - start) > 1) {
		middle = (end + start) / 2;	
		if (cmp_char < S[SA[middle] + depth]) {
			end = middle;		
		} else {
			start = middle;
		}
	}

	return start;
}

/**
* Returns interval containing "cmp_char" if it exists, else returns not_found
* It uses binary_search to find both boundaries (in suffix array).
*/

interval_t topdown(char cmp_char, interval_t interval, string &S, int *SA) {
	int offset = interval.depth;
	int start = interval.start;
	int end = interval.end;	
	int lower_bound, upper_bound;

	if (cmp_char < S[SA[start] + offset]){
		return {-1, 0, 0};
	}
	if (cmp_char > S[SA[end] + offset]){
		return {-1, 0, 0};
	}

	lower_bound = binary_search_left (cmp_char, interval, S, SA);
	upper_bound = binary_search_right (cmp_char, interval, S, SA);
	
	if (lower_bound <= upper_bound) {
		return {offset + 1, lower_bound, upper_bound};
	}	

	return {-1, 0, 0};
}


interval_t suffix_link (interval_t interval, int *ISA, int *SA, int *LCP, int K, int N) {
	int offset = interval.depth;
	int start = interval.start;
	int end = interval.end;
	
	offset = offset -  K;
	if (offset <= 0) return {-1, 0, 0};

	start = ISA[SA[start] / K + 1];
	end = ISA[SA[end] / K + 1];

	return expand_link({offset, start, end}, LCP, K, N); 
}

interval_t expand_link (interval_t interval, int *LCP, int K, int N) {
	int offset = interval.depth;
	int start = interval.start;
	int end = interval.end;	
	
	if (offset == 0) return {0, 0, N - 1};

	int T = 2 * offset * (long)ceil(log(N/K) / log(2.0));
	int e = 0;

	while (start >= 0 and LCP[start] >= offset) {		
		e += 1;
		if (e >= T)
			return 	{-1, 0, 0};
		start -= 1;
	}

	while (end <= (N - 1) and LCP[end + 1] >= offset) {
		end += 1;		
		e += 1;
		if (e >= T)
			return 	{-1, 0, 0};	
	}
	 
	return {offset, start, end};
}


interval_t traverse(int query_index, interval_t interval, int size, string &S, int *SA, string &query) {
	interval_t triplet_tmp;
 
	while (query_index + interval.depth < (signed) query.length()) {
		triplet_tmp = topdown (query [query_index + interval.depth],	interval, S, SA);

		if (triplet_tmp.depth == -1 )
			return interval;	

		interval = triplet_tmp;
		if (interval.depth >= size) 
			break;		
	}
	
	return interval;
}

void print_MEM (int query_index, int ref_string_index, int length, string &S){
 printf ("%c", '>');
  for (int i = query_index; i <= length; i += 1) {
    printf ("%c", S[i]);
  }
  printf ("\n");
  
} 

void findL (int query_index, int ref_string_index, int length, string &S, string &query, int K, int L) { // K is step, K-SA
	for (int k = 0; k <= K - 1 ; k += 1){
		if (query_index == 0 or ref_string_index == 0){
		  print_MEM (query_index + 1, ref_string_index, length, S); // TODO. Why " +1 " ???
			return ;	
		}
		
		if (query[query_index - 1] != S[ref_string_index - 1] and length >= L){
			print_MEM (query_index + 1, ref_string_index, length, S); // TODO. Why " +1 " ???
			return ;
		}

		query_index -= 1; 
		ref_string_index -= 1;
		length += 1;
	}
}



void collect_MEMs (int curr_index, interval_t SA_i, interval_t MEM_i, string &S, string &query, int *SA, int *LCP, int K, int N, int L) {
	int SA_index = SA_i.depth;
	int MEM_start = MEM_i.start;
	int MEM_end = MEM_i.end;
	int MEM_index = MEM_i.depth;
	
	for (int i = MEM_start; i <= MEM_end; i += 1)
		findL (curr_index, SA[i], MEM_index, S, query, K, L);

	while (MEM_index >= SA_index) { 
		if (MEM_end + 1 < N/K) {
			MEM_index = max (LCP[MEM_start], LCP[MEM_end + 1]);
		} else {
		  MEM_index = LCP[MEM_start];
		}
  
		if (MEM_index >= SA_index) {
			while (LCP[MEM_start] >= MEM_index) {
				MEM_start -= 1;
				findL (curr_index, SA[MEM_start], MEM_index, S, query, K, L);
			}
			while ((MEM_end + 1) < N/K and LCP[MEM_end + 1] >= MEM_index) {
				MEM_end += 1;
				findL (curr_index, SA[MEM_end], MEM_index, S, query, K, L);
			}
		}
  }
	return ;
}
/*
*
*
*
*/
void MEM(int query_index, string &S, int *ISA, int *LCP, int *SA, string &query, int K, int N, int L) {
	interval_t SA_interval = {0, 0, N / K - 1};
	interval_t MEM_interval = {0, 0, N / K - 1};
	int curr_index = query_index;

  if (L < K) return;
  
	while (curr_index < (N - (K - query_index))) {
		SA_interval = traverse (curr_index, SA_interval, L - (K - 1), S, SA, query);
		MEM_interval = traverse (curr_index, MEM_interval, query.length(), S, SA, query);

		if (SA_interval.depth <= 1) {
			SA_interval = MEM_interval = {0, 0, N / K - 1};
			curr_index += K;
			continue;
		}

		if (SA_interval.depth >= (L - (K - 1)))
			collect_MEMs(curr_index, SA_interval, MEM_interval, S, query, SA, LCP, K, N, L); //
		curr_index += K;

		SA_interval = suffix_link (SA_interval, ISA, SA, LCP, K, N);
		MEM_interval = suffix_link (MEM_interval, ISA, SA, LCP, K, N);

		if (SA_interval.depth == -1) {
			SA_interval = MEM_interval = {0,  0, N / K - 1};
			continue;
		}
	}

	return ; // MEMs;
}

