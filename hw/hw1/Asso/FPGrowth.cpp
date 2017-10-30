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
#include <set>
#include <iomanip>

#include <cstdint>

using std::cout; using std::endl; using std::cin;
using std::fstream; using std::ios;
using std::string; using std::to_string;
using std::vector;
using std::map;
using std::pair;
using std::sort; using std::includes; using std::set_difference;
using std::shared_ptr;
using std::deque;
using std::set; using std::multiset;

using std::uint64_t; using std::uint32_t;

void FPGrowth( char* filename, double min_sup, double min_conf );

class tranCmp {
public:
	bool operator()( const pair<string, uint32_ptr>& a, const pair<string, uint32_ptr>& b ) const
	{
		return *a.second > *b.second;
	}
};

class freqPatCmp{
public:
	bool operator()( const pair<set<string>, UINT>& a, const pair<set<string>, UINT>& b ) const
	{
		if( a.second == b.second )
			return a.first.size() < b.first.size();
		return a.second > b.second;
	}
};
class assoCmp {
public:
	bool operator()( const pair<string, double>& a, const pair<string, double>& b ) const
	{
		return a.second > b.second;
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

	UINT total_item_num = 0;
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

	UINT tran_num = 0;
	map<string, pair<uint32_ptr, FPTnode_ptr>> one_itemset;
	while( input_file >> s )
	{
		if( s.compare( "." )  != 0 )
		{
			if( one_itemset[s].first == nullptr )
			{
				one_itemset[s].first = uint32_ptr( new UINT( 0 ) );
			}
			++( *( one_itemset[s].first.get() ) );
			transaction[tran_num].push_back( pair<string, uint32_ptr>( s, one_itemset[s].first ) );
			one_itemset[s].second = nullptr;
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
		//	sort all transactions
		sort( tran.begin(), tran.end(), tranCmp() );
		//	build & update FP-Tree
		fptree_root->update( tran, fptree_root, one_itemset, tran_num, min_sup );

		//	show sorted transaction
		#ifdef SORTED
			for( auto item : tran )
				cout << item.first << ":" << *item.second << " ";
			cout << endl;
		#endif

	}

	//	build frequent patterns
	multiset<pair<set<string>, UINT>, freqPatCmp> freq_pat;

	//	map<string, pair<uint32_ptr, FPTnode_ptr>> one_itemset;
	for( auto item : one_itemset )
	{
		FPTnode_ptr tmp = item.second.second;
		FPTnode_ptr conditional_fpt( new FPTnode( "[cond FP Tree root]" ) );
		map<set<string>, UINT> sub_pat;
/*
if( item.first == "4" || item.first == "5" )
	cout << item.first << endl;
*/
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

			//	update conditional FP Tree
			conditional_fpt->update_cond( cond_patt_base, conditional_fpt, tmp_count );

			//	go to next position in linked list
			tmp = tmp->getNext();
		}

		#ifdef CON_FPT
if( item.first == "4" || item.first == "5" )
{
			conditional_fpt->showTree( conditional_fpt );			
	cin.ignore();
}
		#endif

		//	generate frequent pattern
		conditional_fpt->genFreqPat( conditional_fpt, sub_pat, item.first, tran_num, min_sup );
/*
if( item.first == "4" || item.first == "5" )
{
	for( auto zz : sub_pat )
	{
		for( auto zzz : zz.first )
			cout << " " << zzz;
		cout << " : " << zz.second <<  endl;
	}
	cout << "----"<<endl;
}
*/
		//	update frequent pattern map
		freq_pat.insert( pair<set<string>, UINT >( set<string>{ item.first }, *item.second.first.get() ) );
for( auto xx : sub_pat )
{
	if( static_cast<double>( xx.second ) / static_cast<double>( tran_num ) >= min_sup )
		freq_pat.insert( pair<set<string>, UINT >( set<string>{ xx.first }, xx.second ) );
}		
//		freq_pat.insert( sub_pat.begin(), sub_pat.end() );
	}

	//	generate association rules
	deque<pair<string, double>> asso_rule;
	for( auto it = freq_pat.begin() ; it != std::prev( freq_pat.end(), 1 ) ; ++it )
	{
		for( auto x = next( it, 1 ) ; x != freq_pat.end() ; ++x )
		{
			if( includes( x->first.begin(), x->first.end(), it->first.begin(), it->first.end() ) )
			{
				set<string> diff;
				set_difference( x->first.begin(), x->first.end(), it->first.begin(), it->first.end(), std::inserter( diff, diff.begin() ) );
				string s = "[ ";
				for( auto item : it->first )
				{
					if( item != *it->first.begin() )
						s += ", ";
					s += item;
				}
				s += " ]: " + to_string( it->second ) + " ==> [ ";
				for( auto item : diff )
				{
					if( item != *diff.begin() )
						s += ", ";
					s += item ;
				}
				s += " ]: " + to_string( x->second );
				asso_rule.push_back( pair<string, double>( s, static_cast<double>( x->second ) / static_cast<double>( it->second ) ) );	
			}
		}
	}

	sort( asso_rule.begin(), asso_rule.end(), assoCmp() );
	#ifdef ASSO
		int i = 0;
		for( auto x : asso_rule )
			cout << ++i << ". " << x.first << "  <conf(" << std::setprecision( 2 ) << x.second << ")>" << endl;
	#endif

	#ifdef FREQ
		cout << "[Frequent Patterns]" << endl;
		for( auto pat : freq_pat )
		{
			cout << "{ ";
			for( auto item : pat.first )
				cout << item << " ";
			cout << "}:" << pat.second << endl;
		}
	#endif

	input_file.close();
	#ifdef DEBUG
		cout << endl << "[FPGrowth End]" << endl;	
	#endif
}


