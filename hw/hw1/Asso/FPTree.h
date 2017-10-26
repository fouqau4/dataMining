#ifndef __MY_FPTREE_HEADER_FILE_

#define __MY_FPTREE_HEADER_FILE__
#define UINT uint32_t

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <map>
#include <deque>

#include <cstdint>

using std::vector;
using std::string;
using std::pair;
using std::shared_ptr;
using std::cout; using std::endl;
using std::map;
using std::deque;

using std::UINT;

class FPTnode;

typedef shared_ptr<FPTnode> FPTnode_ptr;
typedef shared_ptr<uint32_t> uint32_ptr;

class FPTnode
{
public:
	//	constructor
	FPTnode();
	FPTnode( string item );
	FPTnode( string item, UINT count, FPTnode_ptr parent );
	FPTnode( string item, UINT count, FPTnode_ptr parent, FPTnode_ptr child );

	//	destructor
	~FPTnode()
	{
//		cout << item <<  " node is deleted." << endl;
		child.clear();
	}

	// public method
	const string& getItem(){return item;};
	const UINT& getCount(){return count;};
	const FPTnode_ptr& getNext(){return next;};
	const FPTnode_ptr& getParent(){return parent;};
	const vector<FPTnode_ptr>& getChild(){return child;};
	void update( deque<pair<string, uint32_ptr>>& tran,
				 FPTnode_ptr& root, 
				 map<string, pair<uint32_ptr, FPTnode_ptr>>& tran_record,
				 const uint32_t& tran_num,
				 const double& min_sup );
private:
	// 	data member
	string item;
	UINT count;
	FPTnode_ptr parent;
	vector<FPTnode_ptr> child;
	FPTnode_ptr next;

	// private method
	void addNode( FPTnode_ptr& current_node, const string& item, map<string, pair<uint32_ptr, FPTnode_ptr>>& tran_record );
};
#endif
