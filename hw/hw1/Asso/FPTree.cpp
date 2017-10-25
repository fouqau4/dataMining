#include "FPTree.h"

#include <iostream>

using std::cout; using std::endl;

//	constructor
FPTnode::FPTnode( string item )
    : item( item ), next( nullptr )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count )
    : item( item ), count( count ), next( nullptr )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count, shared_ptr<FPTnode> child )
    : item( item ), count( count ), next( nullptr )
{
	this->child.clear();
    this->child.push_back( child );
}


//	member function
void FPTnode::update( vector<pair<string, uint32_t*>>& tran, shared_ptr<FPTnode>& root )
{
/*
	cout << __FUNCTION__ << endl;
	for( auto item : tran )
		cout << item.first << ":" << *item.second << " ";
	cout << endl;
*/
	shared_ptr<FPTnode> current_node = root;

	for( auto item : tran )
	{
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
				cout << "add new node: " << current_node->item << " --> " << item.first << endl;
				current_node->child.push_back( shared_ptr<FPTnode>( new FPTnode( item.first, 1 )  ) );
				cout << "pushed node :" << current_node->child.back()->item << ",ref count: " << current_node->child.back().use_count() << endl;
				cout << "change current_node" << endl;
				current_node = current_node->child.back();
				cout << "newcurrent node :" << current_node->item << ",ref count: " << current_node.use_count() << endl << endl;
			}
		}
		else
		{
			//	node is not found
			cout << "add new node: " << current_node->item << " --> " << item.first << endl;
			current_node->child.push_back( shared_ptr<FPTnode>( new FPTnode( item.first, 1 )  ) );
			cout << "pushed node :" << current_node->child.back()->item << ",ref count: " << current_node->child.back().use_count() << endl;
			cout << "change current_node" << endl;
			current_node = current_node->child.back();
			cout << "newcurrent node :" << current_node->item << ",ref count: " << current_node.use_count() << endl << endl;
		}

	}
}
