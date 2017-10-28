#include "FPGrowth.h"
#include <iostream>
#include <string>
	
using std::cout;
using std::endl;
using std::string; using std::stod;

int main( int argc, char* argv[] )
{
	switch( argc )
	{
	case 2:
		FPGrowth( argv[1], 0.1f );
		break;
	case 3:
		FPGrowth( argv[1], stod( string( argv[2] ) ) );
		break;
	default:
		// default case
		cout << "[HELP]" << endl
			 << "[*] To run FPGrowth :" << endl
			 << "\t$ ./asso <filename>" << endl;
	}
	return 0;
}
