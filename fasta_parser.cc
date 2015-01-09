#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctype.h>

#include "fasta_parser.h"


// Concatenate new sequences to set, keep track of lengths.
// NOTE: Concatenation using the '`' character to separate strings!
void fasta_parser(string filename, string &output_string, vector<string> &meta_data, vector<long> &startpos) {
	string meta, line;
	long length = 0;

	startpos.push_back(0);

	ifstream data(filename.c_str());

    if(!data.is_open()) { cerr << "unable to open " << filename << endl; exit(1); } 

	else cout << "all fine so far..." <<endl;


	while(!data.eof()) {
		getline(data, line); // Load one line at a time.
		if(line.length() == 0) continue;

		long start = 0, end = line.length() - 1;

		// Meta tag line and start of a new sequence.
		if(line[0] == '>') {

			
			for(long i = start; i <= end; i++) { if(line[i] == ' ') break; meta += line[i]; }

			meta_data.push_back(meta);

			output_string += '`'; // ` character used to separate strings
			startpos.push_back(output_string.length()-1);
			cout << output_string.length() <<endl;

		  // Reset parser state.
		  start = 0; meta = ""; length = 0;
		}

	
		else { // Collect sequence data.
		 
		for(long i = 0; i <= end; i++) { 
			output_string += (line[i]);
			}
		}
	}

	output_string.erase(output_string.begin());
	startpos.erase(startpos.begin());

	cerr << "# output_string.length=" << output_string.length() << endl;
	for(long i = 0; i < (long)meta_data.size(); i++) {
		cerr << "# " << meta_data[i] << " " << startpos[i] << endl;
	}

}