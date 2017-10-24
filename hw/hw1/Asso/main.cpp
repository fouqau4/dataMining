#include "FPGrowth.h"
#include <iostream>

using std::cout;
using std::endl;

int main( int argc, char* argv[] )
{
	switch( argc )
	{
	case 2:
		FPGrowth( argv[1] );
		break;
	default:
		// default case
		cout << "[HELP]" << endl
			 << "[*] To run FPGrowth :" << endl
			 << "\t$ ./asso <filename>" << endl;
	}
	return 0;
}
