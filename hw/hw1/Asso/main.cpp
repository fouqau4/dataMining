#include "FPGrowth.h"
#include <iostream>
#include <string>
#include <chrono>

using std::cout;
using std::endl;
using std::string; using std::stod;

using std::time; using std::time_t; using std::difftime;

int main( int argc, char* argv[] )
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> diff;
	switch( argc )
	{
	case 2:
		start = std::chrono::system_clock::now();
		FPGrowth( string( argv[1] ), 0.1f, 0.5f );
		end = std::chrono::system_clock::now();
		diff =  end - start;
		cout << endl << "Elapsed time: " << diff.count() << "s" << endl;
		break;
	case 3:
		start = std::chrono::system_clock::now();
		FPGrowth( string( argv[1] ), stod( string( argv[2] ) ), 0.5f );
		end = std::chrono::system_clock::now();
		diff =  end - start;
		cout << endl << "Elapsed time: " << diff.count() << "s" << endl;
		break;
	case 4:
		start = std::chrono::system_clock::now();
		FPGrowth( string( argv[1] ), stod( string( argv[2] ) ), stod( string( argv[3] ) ) );
		end = std::chrono::system_clock::now();
		diff =  end - start;
		cout << endl << "Elapsed time: " << diff.count() << "s" << endl;
		break;
	default:
		// default case
		cout << "[HELP]" << endl
			 << "[*] To run FPGrowth :" << endl
			 << "\t$ ./asso <filename>" << endl;
	}
	return 0;
}
