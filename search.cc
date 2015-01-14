#include <cmath>
#include "search.h"
#include <iostream>
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
	
	//cout << "BSEARCH_RIGHT" << endl;
	
	int middle;
	while ((end - start) > 1) {
	  middle = (end + start) / 2;
	  /*
	  cout << "middle: " << middle << endl;
	  cout << "chr: " << cmp_char << endl;
	  cout << "depth: " << depth << endl;
	  cout << "S[SA[middle] + depth]: " << S[SA[middle] + depth] << endl; 
	  */
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
	
	//cout << "topdown" << endl;

	if (cmp_char < S[SA[start] + offset]){
		return {-1, 0, 0};
	}
	if (cmp_char > S[SA[end] + offset]){
		return {-1, 0, 0};
	}
  //cout  <<"interval: " << interval.depth <<" " << interval.start << " " << interval.end << endl;
	lower_bound = binary_search_left (cmp_char, interval, S, SA);
	//cout << "lower bound " << lower_bound << endl;
	upper_bound = binary_search_right (cmp_char, interval, S, SA);
	//cout << "upper bound " << upper_bound << endl;
	if (lower_bound <= upper_bound) {
		return {offset + 1, lower_bound, upper_bound};
	}	

	return {-1, 0, 0};
}

/*
* Builds suffix links for a (sparse) suffix array
*/
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
/*
* Addition function to suffix_link.
* Tries to build a interval by looking only at  
*/

interval_t expand_link (interval_t interval, int *LCP, int K, int N) {
	int offset = interval.depth;
	int start = interval.start;
	int end = interval.end;	
	
	if (offset == 0) return {0, 0, N - 1};

	int T = 2 * offset * log(N/K) / log(2.0);
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

/*
* Computes interval d:[s..e] of Suffix array that contains query string.
* d = depth = character's found (triplet.depth)
* s = start = starting index of Suffix array that contains character on query_index location in query_string
* e = end = ending index of suffix array
* -1 == character (interval) not found 
*/
interval_t traverse(int query_index, interval_t interval, int size, string &S, int *SA, string &query) {
	interval_t triplet_tmp;
  /*
  cout << "\nTravese\n";
  cout << "query_index: " << query_index <<   "\n";
  cout << "string: " << S <<"\n";
  cout << "query: " << query << "\n";
  cout << "size: " << size << endl ;
  cout << "interval: " << interval.depth <<" " << interval.start << " " << interval.end << endl;
  */
  int i = 0;
	while (query_index + interval.depth < (signed) query.length()) {
		triplet_tmp = topdown (query [query_index + interval.depth],	interval, S, SA);
    //cout << i++ <<"interval: " << triplet_tmp.depth <<" " << triplet_tmp.start << " " << triplet_tmp.end << endl;
		
		if (triplet_tmp.depth == -1 )
			return interval;	

		interval = triplet_tmp;
		if (interval.depth >= size) 
			break;		
	}
	
	return interval;
}

/*
* Prints out all maximal exact matches of at least L characters
*/
void print_MEM (int query_index, int ref_string_index, int length){
  cout << "\t" << ref_string_index  << "\t" << query_index << "\t" << length << endl; 
} 

/* 
* Tries to expand matched query 
*/
void findL (int query_index, int ref_string_index, int length, string &S, string &query, int K, int L) { // K is step, K-SA
	for (int k = 0; k < K  ; k += 1) {
		if (query_index == 0 or ref_string_index == 0) {
		  if (length >= L) print_MEM (query_index, ref_string_index, length); 
			else		return ;	
		}
		
		else if (query[query_index - 1] != S[ref_string_index - 1]) {
			if (length >= L) print_MEM (query_index + 1, ref_string_index, length + 1); 
			else return ;
		}

		query_index -= 1; 
		ref_string_index -= 1;
		length += 1;
	}
}
/*
* Collects all the matches
*/
void collect_MEMs (int curr_index, interval_t SA_i, interval_t MEM_i, string &S, string &query, int *SA, int *LCP, int K, int N, int L) {
	int SA_index = SA_i.depth;
	int SA_start = SA_i.start;
	int SA_end = SA_i.end;
	int MEM_start = MEM_i.start;
	int MEM_end = MEM_i.end;
	int MEM_index = MEM_i.depth;
	
	for (int i = MEM_start; i <= MEM_end; i += 1)
		findL (curr_index, SA[i], MEM_index, S, query, K, L);

  if(SA_index == MEM_start && SA_end == MEM_end) return;
  
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
* Main functions for finding Maximal Exact Matches.
* It uses 2 triplets (intervals): SA_interval and MEM_interval for to locate indexes in Suffix Array which  
* contain query string (traverse function). 
* Collect_MEMs functions is used to locate maximal matches in using these intervals (and auxillary arrays:
* LCP (longest common prefix) and ISA (inverse Suffix Array)
* Function suffix_link is used to narrow down intervals (looks for 'missing' suffixes in sparse arrays) 
* by building suffix links during search time (instead of build it before search - like in suffix trees) 
*/
void MEM(int query_index, string &S, int *ISA, int *LCP, int *SA, string &query, int K, int N, int L) {
	interval_t SA_interval = {0, 0, N / K - 1};
	interval_t MEM_interval = {0, 0, N / K - 1};
	int curr_index = query_index;
	/*
  cout << "\nMEM\n";
  cout << "query_index: " << query_index <<   "\n";
  cout << "string: " << S <<"\n";
  cout << "query: " << query << "\n";
  cout << "K: " << K << " N:" << N << " L: " << L <<endl ;
  */
  if (L < K) return;
  
	while (curr_index < (query.length() - (K - query_index))) {
		SA_interval = traverse (curr_index, SA_interval, L - (K - 1), S, SA, query);
		//cout << "SA: " << SA_interval.depth <<" " <<SA_interval.start << " " << SA_interval.end << endl;
		if (SA_interval.depth > MEM_interval.depth) MEM_interval = SA_interval;
    //cout << "MEM: " << MEM_interval.depth <<" " <<MEM_interval.start << " " << MEM_interval.end << endl;
		if (SA_interval.depth <= 1) {
			SA_interval = MEM_interval = {0, 0, N / K - 1};
			curr_index += K;
			continue;
		}

		if (SA_interval.depth >= (L - (K - 1))) {
			MEM_interval = traverse (curr_index, MEM_interval, query.length(), S, SA, query);
			collect_MEMs(curr_index, SA_interval, MEM_interval, S, query, SA, LCP, K, N, L); //
		  curr_index += K;
		  
		  SA_interval = suffix_link (SA_interval, ISA, SA, LCP, K, N);
		  if (SA_interval.depth == -1) {
			  SA_interval = MEM_interval = {0,  0, N / K - 1};
			  continue;
		  }
		  MEM_interval = suffix_link (MEM_interval, ISA, SA, LCP, K, N);
    
    } else {
      curr_index +=K ;
      SA_interval = suffix_link (SA_interval, ISA, SA, LCP, K, N);
		  if (SA_interval.depth == -1) {
			  SA_interval = MEM_interval = {0,  0, N / K - 1};
			  continue;
		  }
		  MEM_interval = SA_interval;
		
		}
	}

	return ; // MEM;
}

