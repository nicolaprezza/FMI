#include <iostream>
#include <cassert>
#include <utils.h>
#include <sdsl/suffix_arrays.hpp>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace sdsl;
using namespace std;

string out_basename=string();
string input_file=string();

ulint sr = 9;

void help(){
	cout << "fmi-build: build FM index of input text file" << endl << endl;
	cout << "Usage: fmi-build [options] <input_file_name>" << endl;
	cout << "   -o <basename>         use 'basename' as prefix for all index files. Default: basename is the specified input_file_name. Index name will be basename.fmi"<<endl;
	cout << "   -sr <value>           the suffix array and inverse suffix array sample rates are both set to 2^value. Accepted values: 5,..,10. Default = 9."<<endl;
	cout << "   <input_file_name>     input text file." << endl;
	exit(0);
}

void parse_args(char** argv, int argc, int &ptr){

	assert(ptr<argc);

	string s(argv[ptr]);
	ptr++;

	if(s.compare("-o")==0){

		if(ptr>=argc-1){
			cout << "Error: missing parameter after -o option." << endl;
			help();
		}

		out_basename = string(argv[ptr]);
		ptr++;

	}else if(s.compare("-sr")==0){

		if(ptr>=argc-1){
			cout << "Error: missing parameter after -sr option." << endl;
			help();
		}

		sr = atoi(argv[ptr]);

		if(sr<5 or sr>10){

			cout << "Error: -sr accepts values in the range [5,10]." << endl;
			help();

		}

		ptr++;

	}else{
		cout << "Error: unrecognized '" << s << "' option." << endl;
		help();
	}

}

template<class index_t>
void build(string input, string output){

	index_t fm_index;

	construct(fm_index, input, 1); // generate index
	store_to_file(fm_index, output); // save it

}

int main(int argc, char** argv){

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;

    auto t1 = high_resolution_clock::now();

	//parse options

    out_basename=string();
    input_file=string();
	int ptr = 1;

	if(argc<2) help();

	while(ptr<argc-1)
		parse_args(argv, argc, ptr);

	input_file = string(argv[ptr]);

	if(out_basename.compare("")==0)
		out_basename = string(input_file);

	string index_file = string(out_basename)+string(".fmi");

	cout << "Building FM-index of input file '" << input_file << "'" << endl;
	cout << "Name '" << index_file << "' will be used for output index file." << endl;

	string input;

	{

		std::ifstream fs(input_file);
		std::stringstream buffer;
		buffer << fs.rdbuf();

		input = buffer.str();

	}

	switch(sr){

		case 5: build<csa_wt<wt_huff<rrr_vector<127> >, 32, 32> >(input_file, index_file); break;
		case 6: build<csa_wt<wt_huff<rrr_vector<127> >, 64, 64> >(input_file, index_file); break;
		case 7: build<csa_wt<wt_huff<rrr_vector<127> >, 128, 128> >(input_file, index_file); break;
		case 8: build<csa_wt<wt_huff<rrr_vector<127> >, 256, 256> >(input_file, index_file); break;
		case 9: build<csa_wt<wt_huff<rrr_vector<127> >, 512, 512> >(input_file, index_file); break;
		case 10: build<csa_wt<wt_huff<rrr_vector<127> >, 1024, 1024> >(input_file, index_file); break;
		default: break;

	}


	auto t2 = high_resolution_clock::now();

	uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	cout << "Total time : " << time << " milliseconds" << endl;

}
