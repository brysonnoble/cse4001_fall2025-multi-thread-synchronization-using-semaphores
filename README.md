# Problem 1: No-starve Readers-writers Solution
## Input & Output
Input: `./cse4001_sync 1`

Sample output: 
```
Reader 5: Reading.
Reader 3: Reading.
Writer 4: Writing.
Writer 1: Writing.
Writer 2: Writing.
Writer 1: Writing.
Reader 1: Reading.
Reader 4: Reading.
Writer 3: Writing.
Reader 1: Reading.
```
## Screenshot
![Solution 1 Output](/sol1.png "Solution 1 Output")

# Problem 2: Writer-priority Readers-writers Solution
## Input & Output
Input: `./cse4001_sync 2`

Sample output: 
```
Reader 2: Reading.
Writer 1: Writing.
Writer 4: Writing.
Writer 2: Writing.
Writer 5: Writing.
Writer 3: Writing.
Writer 1: Writing.
Writer 4: Writing.
Writer 2: Writing.
Writer 5: Writing.
```
## Screenshot
![Solution 2 Output](/sol2.png "Solution 2 Output")

# Problem 3: Dining Philosophers Solution #1
## Input & Output
Input: `./cse4001_sync 3`

Sample output: 
```
Philosopher 0: Thinking
Philosopher 2: Thinking
Philosopher 1: Thinking
Philosopher 3: Thinking
Philosopher 4: Thinking
Philosopher 2: Eating
Philosopher 0: Eating
Philosopher 0: Thinking
Philosopher 3: Eating
Philosopher 2: Thinking
```
## Screenshot
![Solution 3 Output](/sol3.png "Solution 3 Output")

# Problem 4: Dining Philosophers Solution #2
## Input & Output
Input: `./cse4001_sync 4`

Sample output: 
```
Philosopher 0: Thinking
Philosopher 1: Thinking
Philosopher 2: Thinking
Philosopher 3: Thinking
Philosopher 4: Thinking
Philosopher 1: Eating
Philosopher 4: Eating
Philosopher 1: Thinking
Philosopher 4: Thinking
Philosopher 2: Eating
```
## Screenshot
![Solution 4 Output](/sol4.png "Solution 4 Output")
