/*
 * generates patterns in pizza&chili format. Adds an option that allows controlling the maximum number of occurrences of generated patterns
 *
 */


#include <utils.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sdsl/suffix_arrays.hpp>

using namespace std;
using namespace sdsl;

bool locate_=false;
ulint sr=9;

void help(){
	cout << "genpatterns: generates patterns in pizza&chili format, with an option to control the maximum number of occurrences " << endl;
	cout <<	"of generated patterns. This program requires to build the FM index of the input file first. Warning: the program loops" << endl;
	cout <<	"until a pattern with < max_occ is found, so it loops forever if all text substrings have more than this number of occurrences! " << endl << endl;
	cout << "Usage: genpatterns <input_text> <index_name> <length> <number> <out_patterns_file> <max_occ>" << endl;
	cout << "   <input_text>    	input text" << endl;
	cout << "   <index_name>		index file (with .fmi extension). Must be the index of <input_text> !" << endl;
	cout << "   <length>		patterns length" << endl;
	cout << "   <number>		number of patterns to generate" << endl;
	cout << "   <out_patterns_file>	output file in pizza&chili format containing the generated patterns." << endl;
	cout << "   <max_occ>		maximum number of occurrences per generated pattern." << endl;
	exit(0);
}

/*
 * extracts random substring of given length from text
 */
string extract(string & text, int length){

	int i = rand() % ((text.length()-length)+1);

	string p;

	for(int j=i;j<i+length;++j){

		p += text[j];

	}

	return p;

}

int main(int argc, char** argv){

	srand(time(NULL));

    if(argc != 7) help();

    int max_occ = atoi(argv[6]);
    int length = atoi(argv[3]);
    int number = atoi(argv[4]);

    cout << "Generating " << number << " patterns of length " << length << " and maximum number of occurrences " << max_occ << endl;
    cout <<	"from file " << argv[1] << " whose index is " << argv[2] << ". Writing output to file " << argv[5] << endl;

    //index

    cout << "\nloading index ... " << flush;
    csa_wt<wt_huff<rrr_vector<127> >, 256, 256> fmi;
    load_from_file(fmi, argv[2]);
    cout << " done."<< endl;

    cout << "loading text ... " << flush;
    string text;

    {
    	ifstream inFile;
    	inFile.open(argv[1]);

    	stringstream strStream;
    	strStream << inFile.rdbuf();
    	text = strStream.str();

    }
    cout << " done."<< endl;

    std::ofstream outfile (argv[5]);
    string header = "# number=";
    header.append(std::to_string(number));
    header.append(" length=");
    header.append(std::to_string(length));
    header.append(" file=");
    header.append(string(argv[1]));
    header.append(" forbidden=\n");

    outfile.write(header.c_str(),header.length());

    int n=0;//number o fgenerated patterns

    cout << "generating patterns ... " << endl;

    int last_perc=-1;;

    while(n<number){

    	int perc = (100*n)/number;

    	if(perc%10==0 and perc!=last_perc){

    		last_perc=perc;
    		cout << " " << perc << "%"<< endl;

    	}

    	string p = extract(text,length);
    	size_t occ = sdsl::count(fmi, p.begin(), p.end());

    	if(occ <= max_occ){

    		outfile.write(p.c_str(),p.length());
    		n++;

    	}

    }

    cout << "done!" << endl;


}
