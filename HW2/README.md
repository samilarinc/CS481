## Makefile

This makefile just contains a line to run my code with python3. It uses ./in as input argument. If you want to use your own file input you should change it.



## Usage

The argument list of the CLI is same as the example given in HW assignment. 

$ python main.py -i "input_file_location" 



## Classes and Methods

class node: defines a single node on the tree
	has_child: takes a char as argument and returns the child node if there is an edge with given character else returns None
	__str__: returns id of the node as a string

class tree: defines a tree built from nodes
	__init__: sets root node
	build_tree: builds the tree with given patterns
	link_failure: defines failure links between nodes and prints the building process while doing this
	search_word: searchs a word on the tree starting from root. two options: if internal_search is true, if word is found returns the node which contains the last character of the word else None; if internal_search is false, returns a list contains all words found while searching the given word (might be an empty list)

text_search: searchs the patterns in the given text using Aho-Corasick Algorithm and prints the patterns found with their occuring indexes in the text string

