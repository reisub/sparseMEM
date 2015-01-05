using namespace std;

extern string S;
extern long SA[];

/*
*
*
*/
struct interval_t {
	long query_index;
	long start, end;
}


/**
* Returns left bound of interval. 
* Searches input string (S) for suffixes having "comparing_character" at "query_offset",
* using suffix array (SA).
*/
long binary_search_left (char comparing_character, long query_offset, long start_interval, long end_interval) {
	if (comparing_character == S[SA[start_interval] + query_offset]) {
		return start_interval;
	}
	
	long middle;
	while ((end_interval - start_interval) > 1) {
		middle = (end_interval + start_interval) / 2;	
		if (comparing_character <= S[SA[middle] + query_offset]) {
			end_interval = middle;		
		} else {
			start_interval = middle;
		}
	}

	return end_interval;
}

/**
* Returns right bound of interval. 
* Searches input string (S) for suffixes having "comparing_character" at "query_offset",
* using suffix array (SA).
*/
long binary_search_right (char comparing_character, long query_offset, long start_interval, long end_interval) {
	if (comparing_character == S[SA[end_interval] + query_offset]){
		return end_interval;
	}
	
	long middle;
	while ((end_interval - start_interval) > 1) {
		middle = (end_interval + start_interval) / 2;	
		if (comparing_character < S[SA[middle] + query_offset]) {
			end_interval = middle;		
		} else {
			start_interval = middle;
		}
	}

	return start_interval;
}

interval_t topdown (char comparing_character, long query_offset, long start_interval, long end_interval) {
	interval_t not_found = {-1, 0, 0}
	interval_t interval;
	
	long lower_bound, upper_bound;

	if (comparing_character < S[SA[start_interval] + query_offset]){
		return not_found;
	}
	if (comparing_character > S[SA[end_interval] + query_offset]){
		return not_found;
	}

	lower_bound = binary_search_left (comparing_character, query_offset, start_interval, end_interval);
	upper_bound = binary_search_right (comparing_character, query_offset, start_interval, end_interval);
	
	if (lower_bound > upper_bound) {
		return not_found;
	}	

	interval = {query_offset + 1, lower_bound, upper_bound};
	return interval;
}

