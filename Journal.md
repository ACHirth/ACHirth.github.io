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

## Week 17
 After reading some new references (to be added to sources), I have two different things to work on:  
 Attempt to test my model with the established proof of perceptron networks. If I can reconcile the two models, it may be an effective way to prove or disprove the effectiveness of the modified version. This also involves figuring out if some problems are actually solvable, like the xor issue with a one-layer perceptron. 
 Go back and document the effectiveness of various types of update patterns. This seems like the less valuable result, since it may be possible to derive effectiveness from a proof of validity.  
 I am hesitant to increase the size of each agent, however it may be necessary.   
 "randomly selecting your way into a correct answer is not machine learning" (paraphrased)  
 It may be required to increase the scope of each agent to represent its history, not simply its most recent modification.  
 In the same vein, the "test all possible update patterns" solution option is also not learning any more, and far too computationally expensive to consider.  
 I'll be moving forward with trying to prove my model in the pattern of the [neuron.dvi paper]  

## Week 18
 Started with an attempt to model the truth function node within the linear pattern of the classical perceptron formula: w1x1 + w2x2 >= threshold.  
 It is proven that xor is not modelable by this linear formula due to it's lack of linear separability.  
 The proof of theestablished perceptron formula's converge is based on the number of updates required to reach termination.  
 It details an angular representation, gradually moving the line that the formula represents closer to the true formula.  
 It shows that since the angle between the current and the target line is a function of the number of iterations of the formula, defined by t, and is a reducing term, the number of iterations is bounded on some number due to the termination condition.  
 However, this linear representation is not sufficient for the boolean function node, due to its capability to set itself to an xor term.  
 When interpreted as a function of boolean variables, the linear function used above mimics a conjunctive normal form, and lacks enough expressions to represent even a minimized form of xor, making it unsuitable as a representation method.  
 The cnf for xor has a whole rabbit hole that I chose not to follow, starting with a "tseytin transformation".  
 However, boolean algebra as a whole could be a good way to represent the function that the tree is intended to represent.  
 

## Week 19
 To attempt to track update cycles, in the same vein as counting iterations for the perceptron proof method, a more rigorous definition of policy transformation is required.  
 A policy is a 4bit value representing a boolean function on two variables. An update consists of flipping a single bit in that boolean function, causing the policy to represent a different, adjacent formula.  
 Adjacent in this context is defined by a policy that has one bit difference from another policy. In the context of a boolean function, it's return value is only different for one of the four possible pairs of inputs.  
 For example, False(0000) is adjacent to AND(1000), and NOR(0001), but not XOR(0110).  
 Can be represented with a graph, of 16 vertices, each with an edge connecting it to one if its four adjacent formulas.  
 For the termination condition, in the original perceptron method, the updates would stop when the approximated function is "close enough" to the target function, compared as the error relative to a threshold. That is, if the error is low enough, termination ends, and the updated weights are the same as the previous weights, causing the weights at time t+1 to be equal to those at t.  
 Similarly, update cycles for the boolean function node terminates when the error is at zero, or the function represented by the tree accurately represents the function of the input variables.  
 It follows that to guarantee termination, there must exist a configuration of the tree that represents the function defined by the input variables for the error to equal zero, and the termination to be guaranteed.  
 In the context of the single node tree, the claim is that for any sample of two input variables and one output variable, there exists 8 two arity boolean functions that return the desired output given the input. The total number of patterns of two inputs to one boolean output is 2^4 = 16, which matches the number of boolean functions.  
 When extended to four arity functions, the total number of distinct sets of 4 boolean variables is 2^4, so the total number of patterns of possible inputs to outputs for four arity boolean functions is 2^2^4, or 2^16. This pattern has been previously documented in a variety of settings.  
 When represented as a boolean algebra, with boolean inputs {A, B, C, D} as the environment variables, and two arity boolean functions {f1, f2, f3} as the policy:  
 the resolution of the three-node tree is represented by the boolean expression:  
 (A f1 B) f2 (C f3 D)  
 Now the goal is to construct a boolean expression of three two-arity boolean functions and four variables that satisfies the four-arity boolean function represented by a complete set of input data (16 entries of 0 or 1, for each of the 16 possible combinations of 4 boolean variables).  
 Importantly, the ordering of the input variables A-D is unmodified due to the way they are "loaded" into the tree and the functions f1-f3 can be any of the 16 two-arity boolean functions.  
 I had no reason to believe that this format, using any of the two-arity boolean functions applied to each of the input variables, would be incapable of representing all of the four-arity boolean functions.  
 However there a number of problems with the idea.  
 The indicator was the number of a functions of that format. Since the variables' positions are set, the number of permutations is given by the number of options for each of the functions: or 16^3.  
 This is equal to 2^12, and obviously less than 2^16.  
 This indicates that there are significantly fewer unique functions of the tree's format than there are unique boolean functions of four variables.  
 Therefore it is not possible to represent each of the four arity boolean functions with a boolean expression of the tree's format.  
 Additionally, in the tree's format, variables on the left side of the function can never be compared to variables on the right side of the function except as a component of a function with the other variable on its side, and vice versa.   
 This is due to not only the ordering of the variables, but the parenthesization format. Both of these are intrinsic to the current design of the tree structure, but further research into the other options for these formats could allow the format to represent all 2^16 functions.  
 Interestingly, a function of four boolean functions has a number of permutations equal to 16^4, or 2^16, exactly equal to our desired number. But how would one construct a valid boolean expression from four variables and four two-arity boolean functions?   
 All this is closely related to a field known as logic synthesis. It is the study of minimized digital logic expressions and generating them from other functions and complete input/expected output sets. Simple boolean algebra expressions can be minimized and generated by the karnaugh map method, and more reasonably with computers by the quine-mccluskey algorithm.  
 But the quine-mcclusckey algorithm grows on an order of 3^n(ln(n)) with the number of input variables n, making it an unreasonable function in practice.  
 The Espresso heuristic logic minimizer method came after in 1995, significantly reducing the computation time required, but at the cost of rarely producing a sub-optimal result (a function that is not completely minimized).  
 Additional methods or tools were produced, like UC berkeley's SIS, UCLA's RASP, and UC boulder's BOLD. Research on these ended around 2004 and the study of logic synthesis left the academic scene.  
 Digital circuit design companies picked up the study as a component of electronic design automation and at least partially disappeared from public domain.  



