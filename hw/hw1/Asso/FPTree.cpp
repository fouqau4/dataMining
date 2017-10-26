#include "FPTree.h"

#include <iostream>

using std::cout; using std::endl;

//	constructor
FPTnode::FPTnode( string item )
    : item( item ), parent( nullptr ), next( nullptr )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count, FPTnode_ptr parent = nullptr )
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
void FPTnode::update( vector<pair<string, uint32_ptr>>& tran,
//void FPTnode::update( deque<pair<string, uint32_ptr>>& tran,
					  FPTnode_ptr& root,
					  map<string, pair<uint32_ptr, FPTnode_ptr>>& tran_record,
					  const uint32_t& tran_num,
					  const double& min_sup )
{
	FPTnode_ptr current_node = root;

	for( auto item : tran )
	{
		if( static_cast<double>( *item.second ) / static_cast<double>( tran_num ) < min_sup )
		{
			tran_record.erase( item.first );
			cout << endl << "remove : " << item.first << endl << endl;
			continue;
		}

		cout << "[current node :" << current_node->item << ", count: " << current_node->count << "]" << endl;
		cout << "all child :";
		for( auto node : current_node->child )
			cout << node->item << " ";
		cout << endl;
		bool hit = false;
		if( !current_node->child.empty() )
		{
			//	find node
			for( auto node : current_node->child )
			{
				//	node is found
				if( node->item.compare( item.first ) == 0 )
				{
					cout << "hit node: " << item.first << endl;
					++node->count;
					current_node = node;
					cout << "newcurrent node :" << current_node->item << ",ref count: " << current_node.use_count() << endl << endl;
					hit = true;
					break;
				}
			}
			if( !hit )
			{
				//	node is not found
				addNode( current_node, item.first, tran_record );
			}
		}
		else
		{
			//	node is not found
			addNode( current_node, item.first, tran_record );
		}

	}
}

void FPTnode::addNode( FPTnode_ptr& current_node, const string& item, map<string, pair<uint32_ptr, FPTnode_ptr>>& tran_record )
{
//	cout << "add new node: " << current_node->item << " --> " << item << endl;
	current_node->child.push_back( FPTnode_ptr( new FPTnode( item, 1, current_node )  ) );
//	cout << "pushed node :" << current_node->child.back()->item << ",ref count: " << current_node->child.back().use_count() << endl;

	

//	cout << "change current_node" << endl;
	current_node = current_node->child.back();
	cout << "newcurrent node :" << current_node->item << ",ref count: " << current_node.use_count() << endl << endl;
/*
	if( current_node->next == nullptr ) cout << "current_node --> nullptr" << endl;
	if( tran_record[item].second == nullptr ) cout << "tran record: " <<  item <<  " --> nullptr" << endl;
*/
	current_node->next = tran_record[item].second;
	tran_record[item].second = current_node;
/*
	if( current_node->next == nullptr ) cout << "current_node --> nullptr" << endl;
	if( tran_record[item].second == nullptr ) cout << "tran record: " <<  item <<  " --> nullptr" << endl;

	cout << "tran record : " << tran_record[item].second->item << " --> " ;

	if( tran_record[item].second->next == nullptr )
		cout << "nullptr" << endl;
	else
		cout << tran_record[item].second->next->item << endl;
*/
}
