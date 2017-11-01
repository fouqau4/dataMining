#include "FPGrowth.h"
#include <iostream>
#include <string>

#include <ctime>
	
using std::cout;
using std::endl;
using std::string; using std::stod;

using std::time; using std::time_t; using std::difftime;

int main( int argc, char* argv[] )
{
	time_t beg, end;
	double diff;
	switch( argc )
	{
	case 2:
		beg = time( NULL );
		FPGrowth( string( argv[1] ), 0.1f, 0.5f );
		end = time( NULL );
		diff =  difftime( end, beg );
		cout << endl << "Elapsed time: " << diff << "s" << endl;
		break;
	case 3:
		beg = time( NULL );
		FPGrowth( string( argv[1] ), stod( string( argv[2] ) ), 0.5f );
		end = time( NULL );
		diff =  difftime( end, beg );
		cout << endl << "Elapsed time: " << diff << "s" << endl;
		break;
	case 4:
		beg = time( NULL );
		FPGrowth( string( argv[1] ), stod( string( argv[2] ) ), stod( string( argv[3] ) ) );
		end = time( NULL );
		diff =  difftime( end, beg );
		cout << endl << "Elapsed time: " << diff << "s" << endl;
		break;
	default:
		// default case
		cout << "[HELP]" << endl
			 << "[*] To run FPGrowth :" << endl
			 << "\t$ ./asso <filename>" << endl;
	}
	return 0;
}
