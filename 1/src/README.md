### Compiling
#### №1
```sh
gcc -g 1.c -o main -lncurses # Get the debug information
```
#### №2
```sh
gcc -g 2.c -o main -lc -lm # With included math and clib
```
### Debugging
```sh
gdb ./main
```
### Notes on building a .deb package and testing on Astra 1.6

Program 1 has been tested on Astra Linux SE 1.6   
Program 2 has been tested on Astra Linux SE 1.6   
