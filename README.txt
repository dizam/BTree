C++ program that implements a BTree. The program reads a file containing a set of integers and stores them in a BTree. The number of children the internal nodes have isspecified by the second command line argument, and the number of integers each leaf node holds is specified by the third command line argument.
Example integer files are provided in this repository, such as BTree1.dat and BTree25.dat.
The executable BTree program included in this repository walks through each insert step by step on each press of the enter key for clarification of the process.
The executable BTree2 created by the Makefile only shows the final BTree with all the integers inserted.
For example, the command BTree2 BTree12.dat 3 2 will create a BTree using the integers in the file BTree12.dat with 3 children per internal node and 2 children per leaf node.
