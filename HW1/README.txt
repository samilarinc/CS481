## Makefile

I prepared a makefile that only writes the compile code, as the project consists of a single file.

## Usage

The argument list of the CLI is same as the example given in HW assignment. 
$ ./a.out -i /dir/pattern.fa -o /dir/text.fa -a (A-BF-BM-KMP)

If there are fewer arguments given, the program writes a message and exits. 
The program ignores the lines starting with '>' as in fasta formats. 

## Methods

Even if I think that method names describes their functionalities enough, I will give a simple description for each of them:

brute_force: Find pattern using brute force algoritm and return index
KMP_failure: Pre-process pattern to calculate failure function
KMP: Using failure function and strings, find and return index of pattern using Knuth-Morris-Pratt
bad_char: calculate the index of last occurence of each char given in alphabet (A, G, T, C)
good_suffix: calculate shift function using good suffix rules
BM: Using bad_char and good_suffix functions, find and return index of pattern using Boyer-Moore algoritm
write_result: write the output values in the format given in HW assignment
fasta_reader: read fasta files and return the text inside them as a single line and lines starting with '>' ignored
print_best: find and print the best algorithm, using compare number as prior and if they are same, using runtimes
main: parse args and call relevant functions for given inputs