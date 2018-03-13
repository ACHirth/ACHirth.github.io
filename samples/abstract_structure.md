# Abstract Structure - Outdated
Consists of two structures, Agent and Environment
## Agent
A standard perceptron is generally represented by:\
![image](./images/perceptron.png)\
\
My binary perceptron is significantly simplified:\
![image](./images/bperceptron.png)\
\
Represents a single node. \
Has a:
  - Policy: bitset<4>
  - Environment: bitset<2>
  - Learning flag: one bit  // unused so far
  - Reward: one bit // modified to pass-in
    
Has two functions:  Both are wrappers for equivilantly named global functions atm.  
  - resolve: environment + policy -> action
  - update: environment + policy + reward -> policy

## Environment

Has a: 
  - int order: number of layers
  - array of Agents of size 2^order

Has 7 functions: 
  - initializetree: set environment variables for each node on layer N
  - resolvetree: recursively (currently) resolve any node based on the resolution of it's children
  - updatetree: call update() on each of the Agent nodes
  - left, right and parent: index of other nodes relative to any node i 
  - enumerate: print state of the tree array

The environment is a control and management system that maintains an implicit binary tree as a level order array
It operates in an Evaluate-Iterate cycle:\
Evaluate: play a sample through the network and recording the outcome with resolvetree, check it against the proper outcome, and determine an appropriate reward\
Iterate: progress through the network and update policy based on reward and actions of previous evaluate with updatetree.\


## Example
Example training a single node to represent AND:

1. Agent nodes begin zeroed out:\
![image](./images/inode.png)
2. For training samples 00, 01, and 10:
  - Policy:0 properly evaluates to 0
  - Environment sees this as equal to the proper outcome and sets reward to 1
  - Further iteration on these samples will not change the state
3. For training sample 11
  - Policy:0 evaluates to 0
  - Environment sees this as incorrect and sets reward to 0
  - Next iteration will see reward = 0 and update the bit index corresponding with the inputs
  - Evaluate cycle 2 returns with a reward of 1
  - Next iteration will not modify state
4. Final state has, in a sense, converged:\
![image](./images/andnode.png)

The current implementation properly replicates this! (3/12/2018)\
