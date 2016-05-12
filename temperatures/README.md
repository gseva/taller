
## Map-Reduce

This project seeks to implement a map-reduce principle to solve a particular
problem: in this case, to calculate the highest temperature from a bunch of
files with daily maximum temperature for some month. The idea is to run
multiple clients performing the *map* and send the data to the server, which will
run the *reduce* in multiple threads.

Usage:

```
make
./server port
./client ip port < some_data
```

