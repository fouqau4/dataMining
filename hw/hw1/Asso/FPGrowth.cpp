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
#include <deque>

#include <cstdint>

using std::cout; using std::endl; using std::cin;
using std::fstream; using std::ios;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::sort; using std::stable_sort;
using std::shared_ptr;
using std::deque;

using std::uint64_t; using std::uint32_t;

void FPGrowth( char* filename, double min_sup, double min_conf );

class tranCmp {
public:
	bool operator()( pair<string, uint32_ptr>& a, pair<string, uint32_ptr>& b ) const
	{
		return *a.second > *b.second;
	}
};

void FPGrowth( char* filename, double min_sup, double min_conf )
{
	//	load input file
	fstream input_file;

	input_file.open( filename, ios::in );
	#ifdef DEBUG
		if( !input_file.is_open() )
			cout << "[ERROR]: \"" << filename << "\" does not exist." << endl;
		else
			cout << "[OK]: Open the file \"" << filename << "\" successfully." << endl;
	#endif

	//	record items' name
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

	//	build transactions & count 1-itemset
	vector<vector<pair<string, uint32_ptr>>> transaction;
	transaction.push_back( vector<pair<string, uint32_ptr>>() );
//	vector<deque<pair<string, uint32_ptr>>> transaction;
//	transaction.push_back( deque<pair<string, uint32_ptr>>() );

	uint32_t tran_num = 0;
	map<string, pair<uint32_ptr, FPTnode_ptr>> tran_record;
	while( input_file >> s )
	{
		if( s.compare( "." )  != 0 )
		{
			if( tran_record[s].first == nullptr )
			{
//				cout << "nullptr" << endl;
				tran_record[s].first = uint32_ptr( new uint32_t( 0 ) );
			}
//			cout << *tran_record[s].first.get() << endl;
			++( *( tran_record[s].first.get() ) );
			transaction[tran_num].push_back( pair<string, uint32_ptr>( s, tran_record[s].first ) );
			tran_record[s].second = nullptr;
//			cout << s << " ";
		}
		else
		{
			++tran_num;
			transaction.push_back( vector<pair<string, uint32_ptr>>() );
//			transaction.push_back( deque<pair<string, uint32_ptr>>() );
//			cout << endl;
		}
	}
	transaction.pop_back();

	//	sort transaction & build FP-Tree

	FPTnode_ptr fptree_root( new FPTnode( "[FP Tree root]" ) );
	cout << endl << endl;
	for( auto tran : transaction )
	{
		sort( tran.begin(), tran.end(), tranCmp() );
//		stable_sort( tran.begin(), tran.end(), tranCmp() );
		fptree_root->update( tran, fptree_root, tran_record, tran_num, min_sup );

		//	show sorted transaction
		#ifdef SORTED_TRAN
			for( auto item : tran )
				cout << item.first << ":" << *item.second << " ";
			cout << endl;
		#endif

	}

	//	find conditional pattern base
/*
	for( auto item : tran_record )
	{
		f
	}
*/
	//	show 1-item counter
	#ifdef ONE_ITEM
		for( auto i : tran_record )
			cout << i.first << ":" << i.second.first << endl;
	#endif

	#ifdef TRAN
		cout << endl << "[item link]" << endl;
		for( auto i : tran_record )
		{
			cout << "[" << i.first << ":" << i.second.first << "]" << endl;
			FPTnode_ptr tmp = i.second.second;
//			FPTnode_ptr conditional_fpt( new FPTnode( "[cond FP Tree root]" );

			uint32_t c = 0;
			while( tmp != nullptr )
			{
				cout << "all preffix : [" << tmp->getCount() << "] ";
//				deque<FPTnode_ptr> cond_patt_base;
//				cond_patt_base.clear();
				FPTnode_ptr tmp_1 = tmp;
				do
				{
//					cond_patt_base.push_front( tmp_1 );
					cout << " " << tmp_1->getItem();
					tmp_1 = tmp_1->getParent();
				}while( tmp_1->getParent() != nullptr );
//				cond_patt_base.pop_back();
				cout << endl;

				c += tmp->getCount();
				tmp = tmp->getNext();
			}
			cout << c <<  endl;
		}
	#endif

	input_file.close();
	#ifdef DEBUG
		cout << "[FPGrowth End]" << endl;	
	#endif
}


