#include "FPGrowth.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::fstream;
using std::ios;

void FPGrowth( char* filename, double min_sup, double min_conf )
{
//	cout << filename << endl;
	fstream input_file;
	input_file.open( filename, ios::in );
	if( !input_file.is_open() )
		cout << "[ERROR]: \"" << filename << "\" does not exist." << endl;
	#ifdef DEBUG
		cout << "[OK]: Open the file \"" << filename << "\" successfully." << endl;
	#endif
	input_file.close();
		
}

