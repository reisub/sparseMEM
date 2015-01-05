

/**
* Returns left bound of interval. 
* Searches input string (S) for suffixes having "comparing_character" at "query_offset",
* using suffix array (SA).
*/
long binary_search_left (char comparing_character, long query_offset, long start_interval, long end_interval){
	if (comparing_character == S[SA[start_interval] + query_offset]){
		return start_interval;
	}
	
	long middle;
	while ((end_interval - start_interval) > 1){
		middle = (end_interval + start_interval) / 2;	
		if (comparing_character <= S[SA[end_interval] + query_offset]){
			end_interval = middle;		
		} else {
			start_interval = middle;
		}
	
	return end_interval;
}

/**
* Returns right bound of interval. 
* Searches input string (S) for suffixes having "comparing_character" at "query_offset",
* using suffix array (SA).
*/
long binary_search_left (char comparing_character, long query_offset, long start_interval, long end_interval){
	if (comparing_character == S[SA[start_interval] + query_offset]){
		return end_interval;
	}
	
	long middle;
	while ((end_interval - start_interval) > 1){
		middle = (end_interval + start_interval) / 2;	
		if (comparing_character < S[SA[end_interval] + query_offset]){
			end_interval = middle;		
		} else {
			start_interval = middle;
		}
	
	return start_interval;
}
