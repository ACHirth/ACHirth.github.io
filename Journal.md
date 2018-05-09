# Journal

## Week 1 and 2
  Only a few attempts have been made in the foray of binary neural network structures. Among these, BinaryConnect (Courbariaux et al, 2015) was one of the first. 
  Those that followed improved on BinaryConnect's example in various ways, and resulted in: Binarized Neural Network (Courbariaux et al, 2016), XNOR-net (Rastegari et al, 2016) and DoReFa-net (Zhou et al, 2016).
  However, as far as I could gather, all these systems implemented a binarization of full precision weights instead of a purely binary system. 
  It is remarked in these papers that completely binary systems are subject to high noise, low accuracy, and inconsistent convergence. 
  I hope to find a way to evade these pitfalls.

## Week 3
  I've realized a number of issues with my initial idea, and have attempted to remedy them.  
    The most prominent is that of the two-input structure, where reward was taken from another agent. This caused agents to simply choose to pass along the reward instead of producing any computation. It made the basic logic gate more than a one-agent structure.  
  I have settled on an initial draft iteration of my program structure, in the form of an agent - environment split. 
  The agent is in the form of a single node, with three inputs and one output. It contains a flag and a policy as state, in the form of a bit and a char. 
  Nodes have access to two functions, resolve and update. Resolve returns the action the agent would take given a particular pair of inputs and a policy. Update changes the policy of the node based on the previous iteration's reward.
  Additional details can be found in [abstract structure](./samples/abstract_structure.md)

## Week 4
  Implementation has been difficult to find time for, and has proceeded slowly. It is highly wordy and kind of messy, but the general structure seems ok. 
  I don't know how to implement a single bit as a data type in c++. What should conceptually be as simple as "data Bit = 0 | 1" is proving difficult to grok. 
  On the other hand, the representation of policy is incredibly clean, compacting an entire truth table into a single char. Doing the proper bit shifts for resolve will be a challenge however.
  Code will be at [Agent](./Agent.md) once it is presentable. 

## Week 5
  Midterms have further limited my time. Implementation of Agent has begun but is still incomplete. It is the priority for completion; Environment will come ASAP. 
  I'm now officially behind schedule.

## Week 6
  Agent is still under construction. Draft 1 is thrown away in favor of a rewrite. Figuring out how to represent the small data types is proving confusing. Deciding between stl:bitset and bit fields is based on conversions between them and other types. Not sure how to represent a single digit of hex better than an unsigned int bit fielded to 4

## Week 7
  I've chosen bitset for its ease of conversion. In particular, it allows the use of a bitset as an index into another bitset by way of `to_ulong()`. Agent is in a complete and workable place for a variety of test code. 
  It is surprisingly short and clean, (ignoring the comments,) belying the messy iteration it took to develop.

## Week 8
  Environment version 1 framework is in place. I'm catching up on my schedule!. I skipped Week 7's research goal in favor of implemenation time. 
  I worry about the recursive implementation of resolvetree, due to the foreseen extreme scaling problem of the project. A non-recursive rewrite of resolvetree will come when it is determined to be a performance bottleneck later down the line. 

## Week 9
  In a petty attempt to avoid iterators, I mis-designed some parts of Environment. In order to be scalable, dynamic arrays are all but required. A refactor using std::vector is in order next. 
  On the other hand, Environment empirically converges for one- and three-node trees, even with the current update function! In order to test on larger examples, a fuller test-data structure will be required.   
  The current system consists of an implicit binary tree of agents, with functions for traversing the tree, in order to resolve and update the tree of nodes. Additionally, a main function performs the primary loop, performing the evaluate-iterate cycle until the tree stabilizes (stops changing).  
  Further testing is in order.  
  Test data should consist of up to 2^order samples in the format: {2^order bools/bits of input data, 1 bool/bit of correct answer}
  Is it possible to cut off the "top" of the tree to produce a result consisting of more than 1 bit?

## Week 10
 The dynamic array issue has been solved, but another has shown itself. The previous "success" with three-node trees were not truly converging, but stabilized at four updates per cycle.  
 The one node 'tree' structure properly set itself after a single cycle, unchanging with further cycles. This is because for this base case of a single element tree, the primary loop simply updates the policy of that node once for each given input. The node then properly replicates the desired output for that complete set of inputs.  
 But the three node tree, only one level larger than the base case, fails to converge for a variety of inputs. For any given input, the loop would terminate with a stable zero modifications, but once run through the set of inputs, the newly modified tree is again modified. When looped across the input set, the tree is modified for four out of the 16 inputs.  Without true convergence, (zero updates), the model will be inaccurate for those particular inputs. This means that the system is not universal for any arbitrary input, and can only be expected to be accurate for a particular percentage of inputs. This is undesirable.  

## Week 12
 Attempt to solve convergence issue with different update pattern. Instead of updating all agents in the tree at once, attempt to update by level, starting from level zero, similarly to back propogation. The idea behind this is that it is better to change fewer agents per iteration, so start with the root node. Then resolve, or feed forward through the tree to evaluate a new result for that particular input.  
  An inverse strategy would involve updating the entirety of the nodes on the leaf layer first, then next highest, up until the root node. This strategy is based on the idea that a modification of the root node would have a large impact on the tree as a whole, and is not preferable in the case that only a small modification needs to be made for the tree to properly represent the function.  
  Upon testing, neither method bore fruit. But they both reduced the convergence delta from four to two, which may be indicative of an improvement, but is still decidedly suboptimal. 

## Week 13
 Attempt two to solve convergence issue. This time, the update pattern consists of updating each node in the tree, iteratively through the array, and running resolve on the tree between each update. Due to ease of implementation, the first test started with the root node and looped across the array. This method returned the convergence delta to 4 once more, equal to the "update all" method from the beginning.   
 While this may seem like a less optimal solution than the previous due to this, both methods are equally worthless without a delta of zero because the scaling of the error is as of yet undefined.   

## Week 14
 Split of effort: 
 1. Continue with previous week's one by one strategy, this time from the leaf nodes first. I expected little change from previous ordering ordering due to the equivilant results from the by-layer method. Next strategies should involve a possible reset of the tree between each update cycle, and procedurally updating an increasing number of agents until the entire tree is updated. This method may bear fruit due to attempting all possible configuration methods.  
 2. Research other methods of convergence with simple actors. Looking into elementary cellular automata has had iffy results, due to the difficulty in translating cellular automata structure to the agent network's. Additionally, the only proven turing complete case of elementary cellular automata just simulates another complete model, the cyclic tag system. That rabbit hole diverged too quickly from methods that could be transferred over. Upcoming focus will look to swarm intelligence strategies for potential solutions. 

## Week 15
  In an attempt to better describe the problem:   
    A binary tree of agents is represented in an array.  
    The only thing the agent "sees" is its environment variables, as provided by each of its two children. The nodes at the leaves are fed environment variables by the Environment system.  
    The agent produces output, sent to its parent, from its policy and input.  
    Resolving an agent consists of resolving both of it's subtrees, and returning the results of those two resolutions and the agent's policy.  
    Updating an agent consists of changing the output of the agent (its policy) for its most recently resolved inputs.   
    Updating the tree consists of updating the agents within the tree in a particular pattern. I believe this pattern is the core of the issue, as every other component of the system is in its simplest possible state. As of yet, All-at-once, by-level, and sequentially have been tried, as described above. However, these do not encompass all possible update patterns, or differences in tree state between training inputs.  
    Encompassing all possible patterns is the next implementation in testing.  
    Following swarm intelligence strategies has led to no inspiration on a better way to update agents. All agents involved in the algorithms I found were too complex to be applicable (they integrally relied on non-integer values), or were in a structure that I could not reconcile with a tree. 

## Week 16
  Midterm Week 2.0  
  No implementation testing.  
  Further research into swarm intelligence strategies has similarly led to no results that mimic actors so simple.   
  One thing I'd like to point out is how directly in opposition my structure is with regards to standard neural network algorithms. Most of them are fundamentally reliant on reducing percentages in order to be provably convergent. By guaranteeing that in every case or on average, the error of the network will be reduced with each iteration, the algorithm can be relied on to eventually converge to a stable error value.   
  Due to the intentional removal of fractional calculations, it may not be possible for my attempt here to be provably convergent. It seems like there is a very high likelihood that policy updates will oscillate, or flicker back and forth between two close-to-but-not-quite-optimal policies. I intend to look into binary representations of fractional numbers in order to possibly achieve a stable network state. 

