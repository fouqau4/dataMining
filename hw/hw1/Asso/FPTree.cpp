#include "FPTree.h"

#include <iostream>

using std::cout; using std::endl;

FPTnode::FPTnode( string item )
    : item( item )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count )
    : item( item ), count( count )
{
    child.clear();
}

FPTnode::FPTnode( string item, UINT count, FPTnode* child )
//FPTnode::FPTnode( string item, UINT count, shared_ptr<FPTnode> child )
    : item( item ), count( count )
{
	this->child.clear();
    this->child.push_back( child );
}

void FPTnode::update( vector<pair<string, uint32_t*>>& tran )
{
/*
	cout << __FUNCTION__ << endl;
	for( auto item : tran )
		cout << item.first << ":" << *item.second << " ";
	cout << endl;
*/
//	FPTnode current_node = this;
	for( auto item : tran )
	{
		cout << "create" << endl;
		make_shared<FPTnode> tmp( new FPTnode( item.first, 1 ) );
		child.push_back( tmp );
/*
		if( !current_node->child.empty() )
		{
			
		}
		else
		{
			shared_ptr<FPTnode> tmp( new FPTnode( item.first, 1 ) );
			current_node->child.push_back( tmp );
			current_node = 
		}
*/
	}
}
