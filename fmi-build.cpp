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


void help(){
	cout << "fmi-build: build FM index of input text file" << endl << endl;
	cout << "Usage: fmi-build [options] <input_file_name>" << endl;
	cout << "   -o <basename>         use 'basename' as prefix for all index files. Default: basename is the specified input_file_name"<<endl;
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

	}else{
		cout << "Error: unrecognized '" << s << "' option." << endl;
		help();
	}

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

	csa_wt<wt_huff<rrr_vector<127> >, 512, 1024> fm_index;

	construct(fm_index, input_file, 1); // generate index
	store_to_file(fm_index, index_file); // save it

	auto t2 = high_resolution_clock::now();

	uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	cout << "Total time : " << time << " milliseconds" << endl;

}
