
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include "Agent.h"
using namespace std;
/* Environment Class
 * Auguste Hirth
 * Composes Agents into a binary tree with a heap ordered array.
 * Each layer propogates it's results forward or up the tree. 
 * Tree type: Complete implicit binary tree;
 * To do: Switch to dynamic array of agents. 
 * 	create and generalize for map of bools for input.  
 */
#define torder 5

class Environment{ 
	public:
	bool reward = 0; //start with global
	int order = 2; 
	int nagents = 4; //2^order
	Agent agents[4]; 
	
	Environment() {}

	Environment(int order) : order(order){
		nagents = 1 << order;
		
	}

	int left(int i){ return 2*i; }
	int right(int i){ return 2*i + 1; }
	int parent(int i){ return i/2; }
	
	void enumerate(){
		for(int i = 1; i < nagents; i++) {
			cout << "e.agents[" << i << "].policy: " << agents[i].policy << endl;
			cout << "e.agents[" << i << "].environment: " << agents[i].environment << endl;
		}
	}
	// from an array of nagents bool inputs,
	// initialize the environments for the layer N nodes.  
	void initializetree(bool inputs[]){
		for(int i = nagents/2; i < nagents; i++){
			agents[i].environment[0] = inputs[2*(i - nagents/2)];
			agents[i].environment[1] = inputs[2*(i - nagents/2) + 1];
		}
	}

	//recursive version:
	//resolves agent at index after setting it's environment
	//environment formed by recursive resolve on it's children. 
	bool resolvetree(int index){
		if (right(index) > nagents -1){ //is a leaf node aka layer N
			return agents[index].resolve();
		}else { // not layer N
			agents[index].environment[0] = resolvetree(left(index));
			agents[index].environment[1] = resolvetree(right(index));
			return agents[index].resolve();
		}
	}
	
	//update policy for each node by calling update() for it. 
	//Always call resolve first, which updates environment for each node
	//Returns the number of policies changed. 
	int updatetree(bool reward){
		int delta = 0;
		for(int i = 1; i < nagents; i++) { 
			delta += agents[i].update(reward); 
		}
		return delta;
	}


};

int main(){
	Environment e;
	/*
	cout << "nagents: " << e.nagents << endl;
	cout << "e.agents[0].environment: " << e.agents[0].environment << endl;
	cout << "e.agents[0].policy: " << e.agents[0].policy << endl;
	cout << "e.agents[0].modify: " << e.agents[0].modify << endl;
	cout << "done" << endl;
	*/
	e.enumerate();

	bool inputs[16][4] = {
		{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1},
		{0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1},
		{1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1},
		{1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}};

	int delta = 1;
	bool answers[16] = {1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1};  
	bool reward = 0; 
	bool result = 1; 
	for(int i = 0; i < 16; i++){
		e.initializetree(inputs[i]); //NEW SAMPLE
		cout << "inputs: " << inputs[i][0] << ", " << inputs[i][1] << ", " << inputs[i][2] << ", " << inputs[i][3] << endl;
		delta = 1;
		while(delta > 0){	//CORE LOOP: LOOP UNTIL NO CHANGES
			result = e.resolvetree(1);	//EVALUATE
			cout << "result: " << result << endl;
			reward = (result == answers[i]);	// CHECK EVALUATE RESULT
			delta = e.updatetree(reward);	//ITERATE
			cout << "delta: " << delta << endl;
		}
	}
	
	e.enumerate();

}
