#include "FPGrowth.h"
#include "FPTree.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <memory>

#include <cstdint>

using std::cout; using std::endl;
using std::fstream; using std::ios;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::sort;
using std::shared_ptr;

using std::uint64_t; using std::uint32_t;

void FPGrowth( char* filename, double min_sup, double min_conf );

struct {
	bool operator()( pair<string, uint32_t*>& a, pair<string, uint32_t*>& b ) const
	{
		return *a.second > *b.second;
	}
} tranCmp;

void FPGrowth( char* filename, double min_sup, double min_conf )
{
	fstream input_file;

	input_file.open( filename, ios::in );
	#ifdef DEBUG
		if( !input_file.is_open() )
			cout << "[ERROR]: \"" << filename << "\" does not exist." << endl;
		else
			cout << "[OK]: Open the file \"" << filename << "\" successfully." << endl;
	#endif

	vector<string> item_name;

	string s;
	input_file >> s;

	uint32_t total_item_num = 0;
	uint64_t beg = 0;
	while( 1 )
	{
		std::string::size_type n = s.find( ',', beg );
		item_name.push_back( s.substr( beg , n - beg ) );
		++total_item_num;
		if( n == std::string::npos )
			break;
		beg = n + 1;
	}

	vector<vector<pair<string, uint32_t*>>> transaction;
	transaction.push_back( vector<pair<string, uint32_t*>>() );

	uint32_t transaction_num = 0;
	map<string, uint32_t> counter;
	while( input_file >> s )
	{
		if( s.compare( "." )  != 0 )
		{
			transaction[transaction_num].push_back( pair<string, uint32_t*>( s, &(++counter[s]) ) );
//			cout << s << " ";
		}
		else
		{
			++transaction_num;
			transaction.push_back( vector<pair<string, uint32_t*>>() );
//			cout << endl;
		}
	}
	transaction.pop_back();

	//	sort transaction

	shared_ptr<FPTnode> fptree_root( new FPTnode( "[FP Tree root]" ) );
	cout << endl << endl;
	for( auto tran : transaction )
	{
		sort( tran.begin(), tran.end(), tranCmp );
		fptree_root->update( tran, fptree_root );

		//	show sorted transaction
		#ifdef SORTED_TRAN
			for( auto item : tran )
				cout << item.first << ":" << *item.second << " ";
			cout << endl;
		#endif

	}
//	fptree_root->child.clear();
//	cout << "clear child nodes" << endl;

	//	show 1-item counter
	#ifdef ONE_ITEM
		for( auto i : counter )
			cout << i.first << ":" << i.second << endl;
	#endif

	input_file.close();
	#ifdef DEBUG
		cout << "[FPGrowth End]" << endl;	
	#endif
}


