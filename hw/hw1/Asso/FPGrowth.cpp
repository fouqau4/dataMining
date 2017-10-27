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
using std::sort;
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
	vector<deque<pair<string, uint32_ptr>>> transaction;
	transaction.push_back( deque<pair<string, uint32_ptr>>() );

	uint32_t tran_num = 0;
	map<string, pair<uint32_ptr, FPTnode_ptr>> tran_record;
	while( input_file >> s )
	{
		if( s.compare( "." )  != 0 )
		{
			if( tran_record[s].first == nullptr )
			{
				tran_record[s].first = uint32_ptr( new uint32_t( 0 ) );
			}
			++( *( tran_record[s].first.get() ) );
			transaction[tran_num].push_back( pair<string, uint32_ptr>( s, tran_record[s].first ) );
			tran_record[s].second = nullptr;
//			cout << s << " ";
		}
		else
		{
			++tran_num;
			transaction.push_back( deque<pair<string, uint32_ptr>>() );
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
		fptree_root->update( tran, fptree_root, tran_record, tran_num, min_sup );

		//	show sorted transaction
		#ifdef SORTED_TRAN
			for( auto item : tran )
				cout << item.first << ":" << *item.second << " ";
			cout << endl;
		#endif

	}

//cout << "transaction num :" << tran_num << endl << "mininum support ratio:" << min_sup << endl;

	//	show 1-item counter
	#ifdef ONE_ITEM
		for( auto i : tran_record )
			cout << i.first << ":" << i.second.first << endl;
	#endif

	#ifdef TRAN
		cout << endl << "[item link]" << endl;
		for( auto i : tran_record )
		{
			cout << "\n[" << i.first << ":" << *i.second.first.get() << "]" << endl;
			FPTnode_ptr tmp = i.second.second;
			FPTnode_ptr conditional_fpt( new FPTnode( "[cond FP Tree root]" ) );
			map<string, UINT> asso_rule;

			uint32_t c = 0;
			while( tmp != nullptr )
			{
				deque<FPTnode_ptr> cond_patt_base;
				cond_patt_base.clear();
				FPTnode_ptr tmp_1 = tmp;
				UINT tmp_count = tmp_1->getCount();
				do
				{
					cond_patt_base.push_front( FPTnode_ptr( new FPTnode( tmp_1->getItem(), tmp_count ) ) );
					tmp_1 = tmp_1->getParent();
				}while( tmp_1->getParent() != nullptr );
				cond_patt_base.pop_back();

				cout << "conditional pattern base:";
				for( auto x : cond_patt_base )
					cout << " " << x->getItem();
				cout << endl;

				cout << "[build conditional FP Tree]" << endl;
				conditional_fpt->update_cond( cond_patt_base, conditional_fpt, tmp_count );

				c += tmp->getCount();
				tmp = tmp->getNext();
			}
			cout << "[generate frequent patterns]" << endl;
			conditional_fpt->generate_rules( conditional_fpt, asso_rule, i.first, tran_num, min_sup );

			cout << "association rules : " << endl;
			for( auto x : asso_rule )
				cout << x.first << ":" << x.second << endl;

			cout << c <<  endl;
		}
	#endif

	input_file.close();
	#ifdef DEBUG
		cout << "[FPGrowth End]" << endl;	
	#endif
}


