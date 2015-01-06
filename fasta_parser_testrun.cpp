#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "fasta_parser.h"

//#include <getopt.h>
#include <time.h>
//#include <sys/time.h>

#include <cctype>

int main(int argc, char* argv[]) {
	char c;
	string filename = "testy.fasta";

	string ref;
	vector<string> refdescr; 
	vector<long> startpos;

	fasta_parser(filename, ref, refdescr, startpos);

	for (unsigned long i=0; i<ref.size(); ++i)
		std::cout << ' ' << ref[i];
	std::cout << '\n';

	cin >> c;

}