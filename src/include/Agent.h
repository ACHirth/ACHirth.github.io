
#include <iostream>
#include <string>
#include <bitset>
using namespace std;
/* 
 * Agent node class, for use by Environment
 * Auguste Hirth
 * This is a new rewrite, attempting to use std::bitset
 * Bitset and bool are used side by side to represent a conceptual difference
 * Bitset has shift and other operations performed on it, wheras bool doesnt
 * Critically, bitset indexes right to left. 
 */

struct Agent{ 
	bitset<2> environment; 
	//bool reward;
	bool modify; 
	bitset<4> policy;
	
	Agent() : environment(), modify(false), policy() {}
	bool resolve(); 
	bool update(bool reward);
};


// returns a new bitset on the stack using the integer constructor. Pass in a hex value for clarity's sake
bitset<4> to_bitset(unsigned int policy);

// return the value of the policy bitset for that particular set of inputs. 
// to_ulong converts the two bits into an index 0 to 3
bool resolve(bitset<2> environment, bitset<4> policy);

// returns a the policy on the stack either updated or not. 
bitset<4> update(bitset<2> environment, bitset<4> policy, bool reward);
