# mlscheduler

A scheduler that uses Machine Learning tools

There are an array of distinguishing features of a scheduler - what it optimizes for, how it is implemented and what it is used for. There are also some unique characteristics of a scheduler that break it apart from all the rest. Here I will list the characteristics of my scheduler:

1. Task Scheduler - this scheduler is a simple tasking scheduler that will be given an array of tasks (TBD - what details the array will contain) and schedule the tasks depending on the number resources available to be allocated to the tasks. (TBD - the nuance of the problem, eg. Analyzing emails to determine how committed TAs are to extra work and scheduling work according to emails)

2. Shortest Job First Scheduling - the tasks will be assigned resources according to the length of the task, to give best average response time. This algorithm has been selected to optimize latency over throughput.
