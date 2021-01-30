# AutomaticReferenceCounting

This project is a representation of a automatic ref counting system, that uses a very similar idea to what malloc/free do in C language. 
The idea behind it is when allocating a chunck of memory on the heap, in the sizeof parameter of the malloc, ask for additional 1 byte size and use it to store the retain count there.

## Installation

In the Makefile folder, open a terminal and run make all(previous to this you need to have a C compiler installed). After that the Makefile will generate a main executable that you can run after with ./main. 

```bash
make
./main
```

## Usage

The project behaves like an automatic ref counting system. Its main 3 functionalities are related with the functions: allocate, retain and release.

1. Allocate takes in the number of bytes to be allocated, and in addition, adds one more where it is set the retain_count to 1. It also creates the dictionary, if it doesn't exist, that holds the locks used to retain/release specfic instances.
2. Retain take as an argument the address to be retained, offset in backs with 1  byte, increments the count by using and uses a lock to ensure the process is thread safe.
3. Release take as an argument the address to be release, offset in backs with 1  byte, decrements the count by using and uses a lock to ensure the process is thread safe.
