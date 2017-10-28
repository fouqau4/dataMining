#ifndef __MY_FPTREE_HEADER_FILE__

#define __MY_FPTREE_HEADER_FILE__
#define UINT uint32_t

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <map>
#include <deque>
#include <set>

#include <cstdint>

using std::vector;
using std::string;
using std::pair;
using std::shared_ptr;
using std::cout; using std::endl; using std::cin;
using std::map;
using std::deque;
using std::set;

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
	FPTnode( string item, UINT count, FPTnode_ptr parent = nullptr );
	FPTnode( string item, UINT count, FPTnode_ptr parent, FPTnode_ptr child );

	//	destructor
	~FPTnode()
	{
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
				 map<string, pair<uint32_ptr, FPTnode_ptr>>& one_itemset,
				 const UINT& tran_num,
				 const double& min_sup );
	void update_cond( deque<FPTnode_ptr>& tran,
	  				  FPTnode_ptr& root,
					  const UINT& sup_num );
	void genFreqPat( FPTnode_ptr& root,
						 map<set<string>, UINT>& asso_rule,
						 const string& item,
						 const UINT& tran_num,
						 const double& min_sup );
private:
	// 	data member
	string item;
	UINT count;
	FPTnode_ptr parent;
	vector<FPTnode_ptr> child;
	FPTnode_ptr next;

	// private method
	void addNode( FPTnode_ptr& current_node, const string& item, map<string, pair<uint32_ptr, FPTnode_ptr>>& one_itemset );
};
#endif
