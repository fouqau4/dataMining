#include "FPTree.h"

#include <iostream>

using std::cout; using std::endl;

//	constructor
FPTnode::FPTnode( string item )
    : item( item ), parent( nullptr ), next( nullptr )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count, FPTnode_ptr parent )
    : item( item ), count( count ), parent( parent ), next( nullptr )
{
	child.clear();
}

FPTnode::FPTnode( string item, UINT count, FPTnode_ptr parent, FPTnode_ptr child )
    : item( item ), count( count ), parent( parent ), next( nullptr )
{
	this->child.clear();
    this->child.push_back( child );
}


//	member function
void FPTnode::update( deque<pair<string, uint32_ptr>>& tran,
					  FPTnode_ptr& root,
					  map<string, pair<uint32_ptr, FPTnode_ptr>>& one_itemset,
					  const uint32_t& tran_num,
					  const double& min_sup )
{
	FPTnode_ptr current_node = root;

	for( auto item : tran )
	{
		//	check minimum support
		if( static_cast<double>( *item.second ) / static_cast<double>( tran_num ) < min_sup )
		{
			one_itemset.erase( item.first );
			continue;
		}

		bool hit = false;
		if( !current_node->child.empty() )
		{
			//	find node
			for( auto node : current_node->child )
			{
				//	node is found
				if( node->item.compare( item.first ) == 0 )
				{
					++node->count;
					current_node = node;
					hit = true;
					break;
				}
			}
			if( !hit )
			{
				//	node is not found
				addNode( current_node, item.first, one_itemset );
			}
		}
		else
		{
			//	node is not found
			addNode( current_node, item.first, one_itemset );
		}

	}
}

void FPTnode::addNode( FPTnode_ptr& current_node, const string& item, map<string, pair<uint32_ptr, FPTnode_ptr>>& one_itemset )
{
	//	add node as a child of current_node
	current_node->child.push_back( FPTnode_ptr( new FPTnode( item, 1, current_node )  ) );
	//	update current_node
	current_node = current_node->child.back();
	//	update item record list
	current_node->next = one_itemset[item].second;
	one_itemset[item].second = current_node;
}

void FPTnode::update_cond( deque<FPTnode_ptr>& tran,
						   FPTnode_ptr& root,
						   const UINT& sup_num )
{
	FPTnode_ptr current_node = root;
	for( auto item : tran )
	{
		bool hit = false;
		if( !current_node->child.empty() )
		{
			//	find node
			for( auto node : current_node->child )
			{
				//	node is found
				if( node->item.compare( item->getItem() ) == 0 )
				{
					node->count += sup_num;
					current_node = node;
					hit = true;
					break;
				}
			}
			if( !hit )
			{
				//	node is not found
				current_node->child.push_back( FPTnode_ptr( new FPTnode( item->getItem(), sup_num, current_node ) ) );
				current_node = current_node->child.back();
			}
		}
		else
		{
			//	node is not found
			current_node->child.push_back( FPTnode_ptr( new FPTnode( item->getItem(), sup_num, current_node ) ) );
			current_node = current_node->child.back();
		}
	}
}

void FPTnode::genFreqPat( FPTnode_ptr& root,
						  map<set<string>, UINT>& asso_rule,
						  const string& item,
						  const UINT& tran_num,
						  const double& min_sup )
{
	FPTnode_ptr current_node = root;

	deque< deque<pair<set<string>, UINT>> > cache;

	vector<UINT> idx_stack;
	//	visit child_0 of root
	idx_stack.push_back( 0 );

	while( 1 )
	{
if( item == "" )
{
cout << "[current node] : " << current_node->getItem() << endl;
}
		//	internal node
		if( !current_node->child.empty() )
		{

			UINT children_num = current_node->child.size();

			//	next visit child
			UINT current_idx = idx_stack.back();
			idx_stack.pop_back();

			if( current_idx < children_num )
			{
				current_node = current_node->child[current_idx];

				//	next child to visit
				idx_stack.push_back( ++current_idx );
				//	start pos. of child_i
				idx_stack.push_back( 0 );
				continue;
			}

			//	has visited all children
			if( current_node->parent )
			{
				//	add rules
				deque<pair<set<string>, UINT>> ccache;

				while( current_idx-- )
				{
					for( auto pat_pair : cache.back() )
					{
						set<string> pat( pat_pair.first );
						pat.insert( current_node->getItem() );
if( item == "" )
{
	cout << "add pattern to cache:";
	for( auto x : pat )
		cout << " " << x;
	cout << endl << pat_pair.second;
	cout << endl;
}

						ccache.push_back( pat_pair );
						ccache.push_back( pair<set<string>, UINT>( pat, pat_pair.second ) );
					}
					cache.pop_back();
				}

				set<string> ss = { current_node->getItem(), item };
if( item == "" )
{
	cout << "add pattern to cache:";
	for( auto x : ss )
		cout << " " << x;
	cout << endl << current_node->getCount();
	cout << endl;
}
//				asso_rule[ ss ] += current_node->getCount();

				ccache.push_back( pair<set<string>, UINT>( ss, current_node->getCount() ) );
				cache.push_back( ccache );

				//	go back to parent
				current_node = current_node->parent;
			}
			else
			{
				for( auto pat_cache : cache )
				{
					for( auto pat : pat_cache )
					{
						asso_rule[ pat.first ] += pat.second;
					}
				}
				return;
			}
		}
		//	leaf node
		else if( current_node->parent )
		{
			//	no child node, pop start pos.
			idx_stack.pop_back();

			//	cache 
			set<string> ss = { current_node->getItem(), item };
			cache.push_back( deque<pair<set<string>, UINT>>{ pair<set<string>, UINT>( ss, current_node->getCount() ) } );

if( item == "" )
{
	cout << "add pattern to cache:";
	for( auto x : ss )
		cout << " " << x;
	cout << endl << current_node->getCount();
	cout << endl;
}
			//	add rule
//			asso_rule[ ss ] += current_node->getCount();

			//	go back to parent
			current_node = current_node->parent;
		}
		else
			return;
	}
}

void FPTnode::showTree( FPTnode_ptr& root )
{
	FPTnode_ptr current_node = root;
	vector<UINT> idx_stack;
	idx_stack.push_back( 0 );
	while( 1 )
	{
		if( !current_node->child.empty() )
		{
//			cout << "[child of " << current_node->item << "] :" << endl;
			for( auto child : current_node->child )
				cout << " " << child->item;
			cout << "|" << endl;
			UINT child_num = current_node->child.size();
			UINT idx = idx_stack.back();
			idx_stack.pop_back();
			if( idx < child_num )
			{
				current_node = current_node->child[idx];
				idx_stack.push_back( ++idx );
				idx_stack.push_back( 0 );
				continue;
			}
			
			if( !current_node->parent )
				return;
			idx_stack.pop_back();
			current_node = current_node->parent;
		}
		else if( current_node->parent )
		{
			idx_stack.pop_back();
			current_node = current_node->parent;
cout << "Go back to parent" << endl;
		}
		else
			return;
	}
}
