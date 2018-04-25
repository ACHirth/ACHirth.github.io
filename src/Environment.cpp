
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
 * To do:
 * 	create and generalize for map of bools for input.  
 */
#define torder 5

class Environment{ 
	public:
	bool reward = 0; //start with global
	int order = 2; 	//number of levels
	int nagents = 4; //2^order, 3 agents + 0 index
	//Agent agents[4]; 
	vector<Agent> agents = vector<Agent>(4);
	
	Environment() {}

	Environment(int order) : order(order){
		nagents = 1 << order;
		agents = vector<Agent>(nagents);
		
		
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
	// size of inputs must be == 2N
	void initializetree(vector<bool> inputs){
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
	
	//same as above, except only updates a single level. 
	//each level goes from 2^level to 2^level+1 - 1
	int updatetree(bool reward, int level){
		int delta = 0;
		int start = 1 << level;
		int end = 1 << (level+1);
		for(int i = start; i < end; i++) { 
			delta += agents[i].update(reward); 
		}
		return delta;
	}

	int updateagent(bool reward, int index){
		return agents[index].update(reward);
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
	// Inputs is of size 16 by 4 atm, or 2^N by N where N is 2^order
	vector<vector<bool>> inputs = {
		{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1},
		{0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1},
		{1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1},
		{1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}};

	int delta = 0;
	bool answers[16] = {1, 1, 1, 0,  1, 1, 1, 1,  1, 1, 1, 1,  1, 1, 1, 1};  
	bool reward = 0; 
	bool result = 1;
	int level = 0;
	int index = 0;
	int tdelta = 4;
while(tdelta > 0){
	tdelta = 0;
	for(int i = 0; i < 16; i++){
		e.initializetree(inputs[i]); //NEW SAMPLE
		//cout << "inputs: " << inputs[i][0] << ", " << inputs[i][1] << ", " << inputs[i][2] << ", " << inputs[i][3] << endl;
		delta = 1;
		//level = 0;
		index = 0;
		while(delta > 0){	//CORE LOOP: LOOP UNTIL NO CHANGES
			result = e.resolvetree(1);	//EVALUATE
			//cout << "result: " << result << endl;
			reward = (result == answers[i]);	// CHECK EVALUATE RESULT
			//delta = e.updatetree(reward, level);	//ITERATE
			delta = e.updateagent(reward, index); //index mod nagents-1 + 1 to skip zero index
			//level++;
			//level = level % 2;	// progress through levels
			index = index % 3 + 1;
			tdelta += delta;
			//cout << "delta: " << delta << endl;
		}
	}
	cout << "tdelta: " << tdelta << endl;
	e.enumerate();
}
	
	e.enumerate();
}
