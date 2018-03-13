
#include <iostream>
#include <string>
#include <bitset>
#include "Agent.h"
using namespace std;
/* 
 * Agent node class, for use by Environment
 * Auguste Hirth
 * This is a new rewrite, attempting to use std::bitset
 * Bitset and bool are used side by side to represent a conceptual difference
 * Bitset has shift and other operations performed on it, wheras bool doesnt
 * Critically, bitset indexes right to left. 
 */

/*
struct Agent{ 
	bitset<2> environment; 
	//bool reward;
	bool modify; 
	bitset<4> policy;
	bool resolve();
}   * reminder for clarity, removed to avoid redefinition from header
*/

bool Agent::resolve(){ return ::resolve(environment, policy); }

bool Agent::update(bool reward) { 
	bitset<4> temp = policy;
	policy = ::update(environment, policy, reward);
	return (temp != policy);
}

// returns a new bitset on the stack using the integer constructor. Pass in a hex value for clarity's sake
bitset<4> to_bitset(unsigned int policy){
	return bitset<4>(policy);
}

// return the value of the policy bitset for that particular set of inputs. 
// to_ulong converts the two bits into an index 0 to 3
bool resolve(bitset<2> environment, bitset<4> policy){
	return policy[environment.to_ulong()];
}

// returns a the policy on the stack either updated or not. 
bitset<4> update(bitset<2> environment, bitset<4> policy, bool reward){
	if (!reward){ return policy.flip(environment.to_ulong());}
	return policy;
}

/*
// preliminary testing
int main(int nargs, char **args){ 
	Agent alpha; 
	alpha.policy = 0xb; //on the way in bitsets are represented as hex
	alpha.environment = 0x2;
	
	cout << "alpha.policy: " << alpha.policy << endl;
	cout << "hex << alpha.policy: " << hex << alpha.policy.to_ulong() << endl;
	//on the way out the bitset must be converted to a ulong and represented as hex by cout
	for (int i = 0; i < 4 ; i++){
		cout << (alpha.policy)[i] << endl;
	}

	cout << "alpha.environment: " << alpha.environment << endl;
	cout << "hex << alpha.environment: " << hex << alpha.environment << endl;
	
	alpha.environment = 0x3; //1,1
	cout << "policy[" << alpha.environment << "]: " << resolve(alpha.environment, (alpha.policy)) << endl;
	alpha.environment = 0x2; //1,0
	cout << "policy[" << alpha.environment << "]: " << resolve(alpha.environment, alpha.policy) << endl;
	alpha.environment = 0x1; //0,1
	cout << "policy[" << alpha.environment << "]: " << resolve(alpha.environment, (alpha.policy)) << endl;
	alpha.environment = 0x0; //0,0
	cout << "policy[" << alpha.environment << "]: " << resolve(alpha.environment, (alpha.policy)) << endl;
	alpha.environment = 0x2; //1,0
	cout << "update(environment, policy, 0): " << update(alpha.environment, alpha.policy, 0) << endl;
	cout << "update(" << alpha.environment << ", " << alpha.policy << ", 0): " << update(alpha.environment, alpha.policy, 0) << endl;

	cout << "alpha.policy" << alpha.policy << endl;
	cout << "alpha.resolve()" << alpha.resolve() << endl;
	cout << "alpha.update(0)->policy" << alpha.update(0)->policy << endl;
	cout << "alpha.resolve()" << alpha.resolve() << endl;

}

*/
