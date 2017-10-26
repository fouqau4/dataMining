#ifndef __MY_FPTREE_HEADER_FILE_

#define __MY_FPTREE_HEADER_FILE__
#define UINT uint32_t

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <map>

#include <cstdint>

using std::vector;
using std::string;
using std::pair;
using std::shared_ptr;
using std::cout; using std::endl;
using std::map;

using std::UINT;

class FPTnode;

typedef shared_ptr<FPTnode> FPTnode_ptr;

class FPTnode
{
public:
	//	constructor
	FPTnode();
	FPTnode( string item );
	FPTnode( string item, UINT count );
	FPTnode( string item, UINT count, FPTnode_ptr child );

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
	void update( vector<pair<string, uint32_t*>>& tran, FPTnode_ptr& root, map<string, pair<uint32_t, FPTnode_ptr>>& tran_record );
	// 	data member
	string item;
	UINT count;
	vector<FPTnode_ptr> child;
	FPTnode_ptr next;
private:

	// private method
	void addNode( FPTnode_ptr& current_node, const string& item, map<string, pair<uint32_t, FPTnode_ptr>>& tran_record );
};
#endif
