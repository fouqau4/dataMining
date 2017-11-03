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

void FPGrowth( string filename, double min_sup, double min_conf )
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
	UINT total_item_num = 0;
	uint64_t beg = 0;

	input_file >> s;
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
	vector<deque<pair<string, uint32_ptr>>> all_tran;
	all_tran.push_back( deque<pair<string, uint32_ptr>>() );

	UINT tran_num = 0;
	/*	one itemset container :
			key( string ) : name of one itemset
			value( pair<uint32_ptr, FPTnode_ptr> ) : 
				the first place of value is used as a counter for one itemset.
				the second place of value is a pointer in FPTnode type and will be used when construct conditional fp tree.
	*/
	map<string, pair<uint32_ptr, FPTnode_ptr>> one_itemset;

	string file_ext = filename.substr( filename.find_last_of( '.' ) + 1 );
	if( file_ext == "asso" )
	{
		//	read transactions from input file
		while( input_file >> s )
		{
			//	check whether reaching the end of transaction
			if( s.compare( "." )  != 0 )
			{
				//	new item
				if( one_itemset[s].first == nullptr )
				{
					one_itemset[s].first = uint32_ptr( new UINT( 0 ) );
				}

				//	update counter of item
				++( *( one_itemset[s].first.get() ) );
				//	add item to transaction
				all_tran[tran_num].push_back( pair<string, uint32_ptr>( s, one_itemset[s].first ) );
				one_itemset[s].second = nullptr;
			}
			//	reach the end of current transaction
			else
			{
				++tran_num;
				all_tran.push_back( deque<pair<string, uint32_ptr>>() );
			}
		}
		all_tran.pop_back();
	}
	else if( file_ext == "csv" )
	{
		while( input_file >> s )
		{
			UINT current_item = 0;
			beg = 0;
			while( 1 )
			{
				std::string::size_type n = s.find( ',', beg );
				string current_item_name = s.substr( beg , n - beg );
				if( current_item_name != "?" && current_item_name != "" )
				{
					//	new item
					if( one_itemset[item_name[current_item]].first == nullptr )
					{
						one_itemset[item_name[current_item]].first = uint32_ptr( new UINT( 0 ) );
					}

					//	update counter of item
					++( *( one_itemset[item_name[current_item]].first.get() ) );
					//	add item to transaction
					all_tran[tran_num].push_back( pair<string, uint32_ptr>( item_name[current_item], one_itemset[item_name[current_item]].first ) );
					one_itemset[item_name[current_item]].second = nullptr;
				}
				if( n == std::string::npos )
					break;
				++current_item;
				beg = n + 1;
			}
			++tran_num;
			all_tran.push_back( deque<pair<string, uint32_ptr>>() );
		}
		all_tran.pop_back();
	}
	else
	{
		input_file.close();
		return;
	}
	item_name.clear();

	//	sort transaction & build FP-Tree

	FPTnode_ptr fptree_root( new FPTnode( "[FP Tree root]" ) );
	for( auto tran : all_tran )
	{
		//	sort transaction by the priority of one itemset
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

	for( auto item : one_itemset )
	{
		//	access the FPTnode pointed by one itemset container
		FPTnode_ptr tmp = item.second.second;
		FPTnode_ptr conditional_fpt( new FPTnode( "[cond FP Tree root]" + item.first ) );

		/*	find conditional pattern basis & build conditional fp tree :
				go through the linked list of item to find all conditional pattern basis of current item.
				merge all conditional pattern basis to generate a conditional fp tree.
		*/
		while( tmp != nullptr )
		{
			deque<FPTnode_ptr> cond_patt_base;
			cond_patt_base.clear();

			//	find conditional pattern base
			FPTnode_ptr tmp_1 = tmp;
			UINT tmp_count = tmp_1->getCount();
			do
			{
				cond_patt_base.push_front( FPTnode_ptr( new FPTnode( tmp_1->getItem(), tmp_count ) ) );
				tmp_1 = tmp_1->getParent();
			}while( tmp_1->getParent() != nullptr );
			cond_patt_base.pop_back();

			#ifdef CPB
				cout << "[conditional pattern base of " << item.first << " ]" << endl;
				for( auto node : cond_patt_base )
					cout << " " << node->getItem();
				cout << endl;
			#endif

			//	update conditional FP Tree by merging new conditional pattern base
			conditional_fpt->update_cond( cond_patt_base, conditional_fpt, tmp_count );

			//	go to next position in linked list
			tmp = tmp->getNext();
		}

		//	generate frequent pattern
		map<set<string>, UINT> sub_pat;
		conditional_fpt->genFreqPat( conditional_fpt, sub_pat, item.first );

		//	update frequent pattern map
		freq_pat.insert( pair<set<string>, UINT >( set<string>{ item.first }, *item.second.first.get() ) );
		for( auto pat : sub_pat )
		{
			//	whether the frequent pattern satisfying the minimun support ratio
			if( static_cast<double>( pat.second ) / static_cast<double>( tran_num ) >= min_sup )
				freq_pat.insert( pair<set<string>, UINT >( set<string>{ pat.first }, pat.second ) );
		}
	}

	//	show frequent patterns
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

	/*	generate association rules :
			the first place of element is an association rule.
			the second place of element is the confidence of this rule.
	*/
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

	//	sort all association rules by their confidence
	sort( asso_rule.begin(), asso_rule.end(), assoCmp() );

	//	show all association rules
	#ifdef ASSO
		int i = 0;
		for( auto x : asso_rule )
			cout << ++i << ". " << x.first << "  <conf(" << std::setprecision( 2 ) << x.second << ")>" << endl;
	#endif


	input_file.close();
	#ifdef DEBUG
		cout << endl << "[FPGrowth End]" << endl;	
	#endif
}


