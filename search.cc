#include <cmath>
#include "search.h"

using namespace std;


/**
* Returns left bound of interval. 
* Searches input string (S) for suffixes having "cmp_char" at "q_offset",
* using suffix array (SA).
*/
long binary_search_left (string &S, long *SA, char cmp_char, triplet_t triplet) {
	long offset = triplet.index;
	long start = triplet.interval.start;
	long end = triplet.interval.end;	
	
	if (cmp_char == S[SA[start] + offset]) {
		return start;
	}
	
	long middle;
	while ((end - start) > 1) {
		middle = (end + start) / 2;	
		if (cmp_char <= S[SA[middle] + offset]) {
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
long binary_search_right (string &S, long *SA, char cmp_char, triplet_t triplet) {
	long offset = triplet.index;
	long start = triplet.interval.start;
	long end = triplet.interval.end;	

	if (cmp_char == S[SA[end] + offset]){
		return end;
	}
	
	long middle;
	while ((end - start) > 1) {
		middle = (end + start) / 2;	
		if (cmp_char < S[SA[middle] + offset]) {
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

triplet_t topdown_search (string &S, long *SA, char cmp_char, triplet_t triplet) {
	long offset = triplet.index;
	long start = triplet.interval.start;
	long end = triplet.interval.end;	
	long lower_bound, upper_bound;
	triplet_t not_found = {TOO_LOW, {0, 0}};
	triplet_t found;

	if (cmp_char < S[SA[start] + offset]){
		return not_found;
	}
	if (cmp_char > S[SA[end] + offset]){
		return not_found;
	}

	lower_bound = binary_search_left (S, SA, cmp_char, triplet);
	upper_bound = binary_search_right (S, SA, cmp_char, triplet);
	
	if (lower_bound > upper_bound) {
		return not_found;
	}	

	found = {offset + 1, {lower_bound, upper_bound}};
	return found;
}


triplet_t suffix_link (long *ISA, long *SA, long *LCP, triplet_t triplet) {
	long offset = triplet.index;
	long start = triplet.interval.start;
	long end = triplet.interval.end;

	triplet_t empty_triplet = {TOO_LOW, {0, 0}};
	
	offset -= K;
	if (offset <= 0) 
		return empty_triplet;

	start = ISA[SA[start] / K + 1];
	end = ISA[SA[end] / K + 1];

	triplet = expand_link(LCP, {offset, {start, end}});
	
	return triplet; 
}

triplet_t expand_link (long *LCP, triplet_t triplet) {
	long offset = triplet.index;
	long start = triplet.interval.start;
	long end = triplet.interval.end;	

	triplet_t starting = {0, {0, N-1}};
	triplet_t empty = {-1, {0, 0}};
	triplet_t interval;
	
	if (offset == 0) 
		return starting;

	long T = 2 * offset * log(N);
	long e = 0;

	while (start >= 0 and LCP[start] >= offset) {
		start -= 1;		
		e += 1;
		if (e >= T)
			return 	empty;
	}

	while (end <= (N - 1) and LCP[end + 1] >= offset) {
		end += 1;		
		e += 1;
		if (e >= T)
			return 	empty;	
	}
	
	interval = {offset, {start, end}};
	return interval;
}



interval_t search_string (string &S, long *SA, string &query_string) {
	long query_index = 0;
	long start = 0;
	long end = COUNTOF(S); 
	triplet_t triplet;
	interval_t empty = {0, 0};
		
	while (query_index < query_string.length()) {
		triplet = topdown_search (S, SA, query_string[query_index], 
					{ query_index, {start, end}} 
				);
 
		if (triplet.index == TOO_LOW) {
			return empty;		
		}
		
		query_index = triplet.index;
		start = triplet.interval.start;
		end = triplet.interval.end;
	}

	return triplet.interval;
}


triplet_t traverse(string &S, long *SA, string &query, long query_index, triplet_t triplet, long size) {
	triplet_t triplet_tmp;
	triplet_t not_found = {-1, {0, 0}};
 
	while (query_index + triplet.index < query.length()) {
		triplet_tmp = topdown_search (S, SA, 
						query [query_index + triplet.index],
					 	triplet);

		if (triplet_tmp.index == TOO_LOW)
			return not_found;	

		triplet = triplet_tmp;
		if (triplet.index >= size) 
			break;		
	}
	
	return triplet;
}

void print_MEM (long query_index, long ref_string_index, long length, string &S){
  cout << '>';
  for (long i = query_index; i <= length; i += 1) {
    cout << S[i + 1];
  }
    
  cout << '\n';
  
} 

void findL (long query_index, long ref_string_index, long length, string &S, string &query) { // K is step, K-SA
	for (long k = 0; k < K; k += 1){
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



void collect_MEMs (long *SA, long *LCP, long curr_index, triplet_t SA_i, triplet_t MEM_i, string &S, string &query) {
	long SA_start = SA_i.interval.start;
	long SA_end = SA_i.interval.end;
	long SA_index = SA_i.index;
	long MEM_start = MEM_i.interval.start;
	long MEM_end = MEM_i.interval.end;
	long MEM_index = MEM_i.index;
	
	for (long i = MEM_start; i < MEM_end; i += 1)
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
void find_MEMs(string &S, long *ISA, long *LCP, long *SA, string &query, long query_index) {
	triplet_t SA_interval = {0, {0, N / K - 1}};
	triplet_t MEM_interval = {0, {0, N / K - 1}};
	long curr_index = query_index;

	while (curr_index < (query.length() - (K - query_index))) {
		SA_interval = traverse (S, SA, query, curr_index, SA_interval, L - (K - 1));
		MEM_interval = traverse (S, SA, query, curr_index, SA_interval, query.length());

		if (SA_interval.index <= 1) {
			SA_interval = MEM_interval = {0, {0, N / K - 1}};
			curr_index += K;
			continue;
		}

		if (SA_interval.index >= (L - (K - 1)))
			collect_MEMs(SA, LCP, curr_index, SA_interval, MEM_interval, S, query); //
		curr_index += K;

		SA_interval = suffix_link (ISA, SA, LCP, SA_interval);
		MEM_interval = suffix_link (ISA, SA, LCP, MEM_interval);

		if (SA_interval.index == TOO_LOW) {
			SA_interval = MEM_interval = {0,  {0, N / K - 1}};
			continue;
		}
	}

	return ; // MEMs;
}

