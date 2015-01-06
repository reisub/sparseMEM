using namespace std;

#define COUNTOF(x) (sizeof(x)/sizeof(*x))

extern string S;
extern long SA[];

/*
*
*
*/


struct interval_t {
	long start, end;
};

struct triplet_t {
	long index;
	struct interval_t interval;
};


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

/**
* Returns interval containing "compairing_character" if it exists, else returns not_found
* It uses binary_search to find both boundaries (in suffix array).
*/

triplet_t topdown_search (char comparing_character, long query_offset, long start_interval, long end_interval) {
	triplet_t not_found = {-1, {0, 0}};
	triplet_t interval;
	
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

	interval = {query_offset + 1, {lower_bound, upper_bound}};
	return interval;
}

interval_t search_string(string query_string) {
	long query_index = 0;
	long start_interval = 0; // TODO: refactor names
	long end_interval = 11; // TODO: use dinamyc information!

	triplet_t triplet;
	interval_t empty_interval = {0, 0};
		
	while (query_index < query_string.length()) {
		triplet = topdown_search (query_string[query_index], 
					query_index,
					start_interval,
					end_interval); 
		if (triplet.index == -1) {
			return empty_interval;		
		}
		
		query_index = triplet.index;
		start_interval = triplet.interval.start;
		end_interval = triplet.interval.end;
	}

	return triplet.interval;
}

/*
triplet_t traverse(p, d:[s..e], M) {
	while (p + d < P.length()){
		d2 : [s2..e2] = topdown(P[p+d], d:[s..e])
		if (d2 == empty_interval)
			return d:[s..e]	

		d : [s..e] = d2 : [s2..e2]
		if (d >= M) 
			break;		
	}
	
	return d:[s..e];
}

void findL (p, i, q){
	for (k = 0; k < K; k += 1){
		if (p == 0 or i == 0){
			MEM at i in S, p in P, length q;
			return ;	
		}
		
		if (P[p-1] != S[i-1] and q >= L){
			MEM at i in S, p in P, length q;
			return ;
		}

		p -= 1; 
		i -= 1;
		q += 1;
	}
}


void collect_MEMs (p, d:[s..e], q:[l..r]){
	for (i = l; i < r; i += 1)
		findL (p, SA[i], q);

	while q >= d 
		if r + 1 < n/K
			q = max (LCP[l], LCP[r + 1]);
		else q = LCP[l];

		if q >= d
			while LCP[l] >= q
				l = l-1
				findL (p, SA[l], q)
			while r+1 < n/K and LCP[r + 1] >= q
				r += 1
				findL (p, SA[r], q);

	return ;
}


void find_MEMs(p0) {
	d:[s..e] = q:[l..r] = 0:[0..n / (K-1)]
	p = p0
	while p < (P.length() - (K- p0))
		d:[s..e] = traverse(p, d:[s..e], L - (K - 1))
		q:[l..r] = traverse(p, q:[l..r], P.length())

		if d <= 1
			d:[s..e] = q:[l..r] = 0:[0..n / (K-1)]
			p = p + K
			continue

		if d >= (L - (K - 1))
			collect_MEMS(p, d:[s..e], q:[l..r])

		p += K

		d:[s..e] = suffix_link (d:[s..e])
		q:[l..r] = suffix_link (q:[l..r])

		if d == empty
			d:[s..e] = q:[l..r] = 0:[0..n / (K-1)]
			continue
}


*/












