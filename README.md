# mlscheduler

A scheduler that uses a simple Machine Learning algorithm and implements a Shortest Job First Scheduling policy

1. Vector and priority queue classes are used in the implementation, along with other utility functions

2. The project implements multi-threaded execution of an input file of instructions and logs the time taken for each instruction to complete. These times are used in future scheduling of the same instructions and the times are updated and averaged across multiple executions of instructions.

3. The use case of the project is in batch processing, where the same instructions must be executed a large number of times.

Limitations:

The scheduler does not take into account dependencies across instructions and due to the multi-threaded nature of the program, no instruction except the first instruction of the input file is guaranteed a position in the queue.
