
/*
 *  This file is part of lz-rlbwt.
 *  Copyright (c) by
 *  Nicola Prezza <nicolapr@gmail.com>,
 *  Djamal Belazzougui, Fabio Cunial, Travis Gagie, and Mathieu Raffinot
 *
 *   lz-rlbwt is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   lz-rlbwt is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details (<http://www.gnu.org/licenses/>).
 */

#include <utils.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sdsl/suffix_arrays.hpp>

using namespace std;
using namespace sdsl;

void help(){
	cout << "fmi-search: count/locate pattern occurrences. Note that this program" << endl;
	cout << "discards the output (i.e. number of pattern occurrences), and should be used only for benchmark purposes." << endl << endl;
	cout << "Usage: fmi-search [options] <index_basename> <patterns_file>" << endl;
	cout << "   -l                  locate occurrences (otherwise only count)" << endl;
	cout << "   <index_basename>    basename of all index files" << endl;
	cout << "   <patterns_file>     file in pizza&chili format containing the patterns." << endl;
	exit(0);
}

int main(int argc, char** argv){

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;

    if(argc !=4 and argc!=3) help();

    string index_file;
    string patterns_file;
    bool locate = false;

    if(argc==3){

    	index_file = argv[1];
    	patterns_file = argv[2];

    }else{

    	if(string(argv[1]).compare("-l") != 0) help();
    	locate = true;

    	index_file = argv[2];
    	patterns_file = argv[3];

    }

    auto t1 = high_resolution_clock::now();

    csa_wt<wt_huff<rrr_vector<127> >, 512, 1024> fm_index;
    load_from_file(fm_index, index_file);

	auto t2 = high_resolution_clock::now();

	cout << "searching patterns ... " << endl;
	ifstream ifs(patterns_file);

	//read header of the pizza&chilli input file
	//header example:
	//# number=7 length=10 file=genome.fasta forbidden=\n\t
	string header;
	std::getline(ifs, header);

	ulint n = get_number_of_patterns(header);
	ulint m = get_patterns_length(header);

	ulint last_perc = 0;

	ulint occ_tot=0;

	//extract patterns from file and search them in the index
	for(ulint i=0;i<n;++i){

		ulint perc = (100*i)/n;
		if(perc>last_perc){
			cout << perc << "% done ..." << endl;
			last_perc=perc;
		}

		string p = string();

		for(ulint j=0;j<m;++j){
			char c;
			ifs.get(c);
			p+=c;
		}

		size_t occ = sdsl::count(fm_index, p.begin(), p.end());

		if(locate) sdsl::locate(fm_index, p.begin(), p.end());

		occ_tot += occ;

	}

	double occ_avg = (double)occ_tot / n;

	cout << endl << occ_avg << " average occurrences per pattern" << endl;

	ifs.close();

	auto t3 = high_resolution_clock::now();

	//printRSSstat();

	uint64_t load = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	cout << "Load time : " << load << " milliseconds" << endl;

	uint64_t search = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
	cout << "number of patterns n = " << n << endl;
	cout << "pattern length m = " << m << endl;
	cout << "total number of occurrences  occ_t = " << occ_tot << endl;

	cout << "Total time : " << search << " milliseconds" << endl;
	cout << "Search time : " << (double)search/n << " milliseconds/pattern (total: " << n << " patterns)" << endl;
	cout << "Search time : " << (double)search/occ_tot << " milliseconds/occurrence (total: " << occ_tot << " occurrences)" << endl;

}
