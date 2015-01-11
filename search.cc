#include <cmath>
#include "search.h"

using namespace std;


/**
* Returns left bound of interval. 
* Searches input string (S) for suffixes having "cmp_char" at "q_offset",
* using suffix array (SA).
*/
long binary_search_left (char cmp_char, interval_t interval, string &S, long *SA) {
	long depth = interval.depth;
	long start = interval.start;
	long end = interval.end;	
	
	if (cmp_char == S[SA[start] + depth]) {
		return start;
	}
	
	long middle;
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
long binary_search_right (char cmp_char, interval_t interval, string &S, long *SA) {
	long depth = interval.depth;
	long start = interval.start;
	long end = interval.end;	

	if (cmp_char == S[SA[end] + depth]){
		return end;
	}
	
	long middle;
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

interval_t topdown(char cmp_char, interval_t interval, string &S, long *SA) {
	long offset = interval.depth;
	long start = interval.start;
	long end = interval.end;	
	long lower_bound, upper_bound;

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


interval_t suffix_link (interval_t interval, long *ISA, long *SA, long *LCP) {
	long offset = interval.depth;
	long start = interval.start;
	long end = interval.end;
	
	offset = offset -  K;
	if (offset <= 0) return {-1, 0, 0};

	start = ISA[SA[start] / K + 1];
	end = ISA[SA[end] / K + 1];

	return expand_link({offset, start, end}, LCP); 
}

interval_t expand_link (interval_t interval, long *LCP) {
	long offset = interval.depth;
	long start = interval.start;
	long end = interval.end;	
	
	if (offset == 0) return {0, 0, N - 1};

	long T = 2 * offset * (long)ceil(log(N/K) / log(2.0));
	long e = 0;

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



interval_t search_string (string &S, long *SA, string &query_string) {
	long query_index = 0;
	long start = 0;
	long end = 11; 
	interval_t triplet;
		
	while (query_index < query_string.length()) {
		triplet = topdown (query_string[query_index], {query_index, start, end}, S, SA);
 
		if (triplet.depth == TOO_LOW) {
			return {0, 0};		
		}
		
		query_index = triplet.depth;
		start = triplet.start;
		end = triplet.end;
	}

	return {triplet.depth, triplet.start, triplet.end};
}


interval_t traverse(long query_index, interval_t interval, long size, string &S, long *SA, string &query) {
	interval_t triplet_tmp;
	interval_t not_found = {-1, 0, 0};
 
	while (query_index + interval.depth < query.length()) {
		triplet_tmp = topdown (query [query_index + interval.depth],	interval, S, SA);

		if (triplet_tmp.depth == TOO_LOW)
			return interval;	

		interval = triplet_tmp;
		if (interval.depth >= size) 
			break;		
	}
	
	return interval;
}

void print_MEM (long query_index, long ref_string_index, long length, string &S){
  cout << '>';
  for (long i = query_index; i <= length; i += 1) {
    cout << S[i + 1];
  }
    
  cout << '\n';
  
} 

void findL (long query_index, long ref_string_index, long length, string &S, string &query) { // K is step, K-SA
	for (long k = 0; k <= K - 1 ; k += 1){
		if (query_index == 0 or ref_string_index == 0){
		  print_MEM (query_index, ref_string_index, length, S);
			return ;	
		}
		
		if (query[query_index - 1] != S[ref_string_index - 1] and length >= L){
			print_MEM (query_index, ref_string_index, length, S);
			return ;
		}

		query_index -= 1; 
		ref_string_index -= 1;
		length += 1;
	}
}



void collect_MEMs (long curr_index, interval_t SA_i, interval_t MEM_i, string &S, string &query, long *SA, long *LCP) {
	long SA_start = SA_i.start;
	long SA_end = SA_i.end;
	long SA_index = SA_i.depth;
	long MEM_start = MEM_i.start;
	long MEM_end = MEM_i.end;
	long MEM_index = MEM_i.depth;
	
	for (long i = MEM_start; i <= MEM_end; i += 1)
		findL (curr_index, SA[i], MEM_index, S, query);

	while (MEM_index >= SA_index) { 
		if (MEM_end + 1 < N/K) {
			MEM_index = MAX2 (LCP[MEM_start], LCP[MEM_end + 1]);
		} else {
		  MEM_index = LCP[MEM_start];
		}
  
		if (MEM_index >= SA_index) {
			while (LCP[MEM_start] >= MEM_index) {
				MEM_start -= 1;
				findL (curr_index, SA[MEM_start], MEM_index, S, query);
			}
			while ((MEM_end + 1) < N/K and LCP[MEM_end + 1] >= MEM_index) {
				MEM_end += 1;
				findL (curr_index, SA[MEM_end], MEM_index, S, query);
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
void MEM(long query_index, string &S, long *ISA, long *LCP, long *SA, string &query) {
	interval_t SA_interval = {0, 0, N / K - 1};
	interval_t MEM_interval = {0, 0, N / K - 1};
	long curr_index = query_index;

	while (curr_index < (query.length() - (K - query_index))) {
		SA_interval = traverse (curr_index, SA_interval, L - (K - 1), S, SA, query);
		MEM_interval = traverse (curr_index, MEM_interval, query.length(), S, SA, query);

		if (SA_interval.depth <= 1) {
			SA_interval = MEM_interval = {0, 0, N / K - 1};
			curr_index += K;
			continue;
		}

		if (SA_interval.depth >= (L - (K - 1)))
			collect_MEMs(curr_index, SA_interval, MEM_interval, S, query, SA, LCP); //
		curr_index += K;

		SA_interval = suffix_link (SA_interval, ISA, SA, LCP);
		MEM_interval = suffix_link (MEM_interval, ISA, SA, LCP);

		if (SA_interval.depth == TOO_LOW) {
			SA_interval = MEM_interval = {0,  0, N / K - 1};
			continue;
		}
	}

	return ; // MEMs;
}

