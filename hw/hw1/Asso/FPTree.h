#ifndef __MY_FPTREE_HEADER_FILE_

#define __MY_FPTREE_HEADER_FILE__
#define UINT uint32_t

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <iostream>

#include <cstdint>

using std::vector;
using std::string;
using std::pair;
using std::shared_ptr;
using std::cout; using std::endl;

using std::UINT;

class FPTnode
{
public:
	string item;
	UINT count;
	vector<shared_ptr<FPTnode>> child;
	shared_ptr<FPTnode> next;
	

//	constructor
	FPTnode();
	FPTnode( string item );
	FPTnode( string item, UINT count );
	FPTnode( string item, UINT count, shared_ptr<FPTnode> child );

//	destructor
	~FPTnode()
	{
//		cout << item <<  " node is deleted." << endl;
		child.clear();
	}
// public method
	void update( vector<pair<string, uint32_t*>>& tran, shared_ptr<FPTnode>& root );

};
#endif
