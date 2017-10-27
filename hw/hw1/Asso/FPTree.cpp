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

void FPTnode::update_cond( deque<FPTnode_ptr>& tran,
						   FPTnode_ptr& root,
						   const UINT& sup_num )
{
	FPTnode_ptr current_node = root;

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
				if( node->item.compare( item->getItem() ) == 0 )
				{
					cout << "hit node: " << item->getItem() << endl;
					node->count += sup_num;
					current_node = node;
					cout << "newcurrent node :" << current_node->item << ",ref count: " << current_node.use_count() << endl << endl;
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

void FPTnode::generate_rules( FPTnode_ptr& root,
							  map<string, UINT>& asso_rule,
							  const string& item,
							  const UINT& tran_num,
							  const double& min_sup )
{
	cout << endl;
	FPTnode_ptr current_node = root;
	vector<UINT> idx_stack;
	//	visit child_0 of root
	idx_stack.push_back( 0 );
	while( 1 )
	{
		cout << "[current node : " << current_node->getItem() << " ]" << endl;
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
			if( current_node->parent )
			{
			double dt =  static_cast<double>( current_node->getCount() ) / static_cast<double>( tran_num );
			cout << dt << " : " << min_sup << endl;
				if( static_cast<double>( current_node->getCount() ) / static_cast<double>( tran_num ) >= min_sup )
				{ 
					//	add rules
					vector<pair<string, UINT>> v;
					for( auto rule : asso_rule )
					{
						v.push_back( pair<string, UINT>( rule.first, rule.second ) );
					}
					for( auto rule : v )
					{
						asso_rule[ current_node->getItem() + "," + rule.first ] += rule.second;
					}
					asso_rule[ current_node->getItem() + "," + item ] += current_node->getCount();
				}
				//	go back to parent
				current_node = current_node->parent;
			}
			else
				return;
		}
		//	leaf node
		else if( current_node->parent )
		{
			//	no child node, pop start pos.
			idx_stack.pop_back();
			double dt =  static_cast<double>( current_node->getCount() ) / static_cast<double>( tran_num );
			cout << dt << " : " << min_sup << endl;
			
			if( static_cast<double>( current_node->getCount() ) / static_cast<double>( tran_num ) >= min_sup )
			{ 
				//	add rule
				asso_rule[ current_node->getItem() + "," + item ] += current_node->getCount();
			}

			//	go back to parent
			current_node = current_node->parent;
		}
		else
			return;
	}
}
