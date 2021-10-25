#include<iostream>
#include<getopt.h>
#include<chrono>
#include<sys/stat.h>
#include<vector>

#define ALPHABET_LEN 4
#define MAX(a, b) ((a>b) ?(a) :(b))

using namespace std;
size_t length = 0;
size_t comparison_bf;
size_t comparison_kmp;
size_t comparison_bm;

/*
g++ hw1.cpp && ./a.out -i text.fa -o pat.fa -a A
*/
// BM is working from the end to beginning 
// Should be fixed!!!

int brute_force(string pattern, string text)
{
    size_t pattern_length = pattern.length();
    size_t text_length = text.length();
    comparison_bf = 0;
    int i = 0;
    bool found = false;
    while(i < text_length){
        int j = 0;
        int i_temp = i;
        found = true;
        while(j < pattern_length){
            comparison_bf++;
            if(pattern[j] != text[i_temp]){
                found = false;
                break;
            }
            j++;
            i_temp++;
        }
        if(found){
            return i;
        }
        i++;
    }
    return -1;
}

int* KMP_failure(string pattern, size_t len)
{
    int* failure = new int[len];
    failure[0] = 0;
    int i = 1;
    int j = 0;
    while(i < len){
        if(pattern[i] == pattern[j]){
            failure[i] = j+1;
            i++;
            j++;
        }
        else if(j > 0){
            j = failure[j-1];
        }
        else{
            failure[i] = 0;
            i++;
        }
    }
    return failure;
}

int KMP(string pattern, string text)
{
    size_t pattern_length = pattern.length();
    size_t text_length = text.length();
    comparison_kmp = 0;
    int* failure = KMP_failure(pattern, pattern_length);
    int i = 0;
    int j = 0;
    while(i < text_length){
        bool flag = text[i] == pattern[j];
        comparison_kmp += 1;
        if(flag){
            j++;
            i++;
        }
        if(j == pattern_length){
            return i - j;
        }
        else if((i < text_length) && !flag){
            if(j != 0){
                j = failure[j - 1];
            }
            else
                i++;
        }
    }
    return -1;
}

int* bad_char(string pattern)
{
    int i, len = pattern.length();
    uint8_t letter;
    int* badchar = new int[ALPHABET_LEN];
    for (i = 0; i < ALPHABET_LEN; i++)
        badchar[i] = -1;
    for (i = 0; i < len; i++){
        letter = (uint8_t) pattern[i];
        letter = letter>>1;
        letter = letter & 0b00000011;
        badchar[letter] = i-1;
    }
    return badchar;
}

int* good_suffix(string pattern)
{
	size_t len = pattern.length();
    int i = len;
    int j = len + 1;
    // vector<int> bpos(len+1), shifts(len+1,0); 
	int* bpos = new int[len+1];
    int* shifts = new int[len+1];
    for(int k = 0; k <= len; k++){
        shifts[k] = 0;
    } 
	bpos[i]= j;
	while(i> 0)  
	{
		while( j<=len && pattern[i-1]!= pattern[j-1])
		{
			if(shifts[j]==0)
				shifts[j] = j-i;
			j = bpos[j]; 
		}
		i--;
		j--;
		bpos[i] = j;
	}
	j = bpos[0];
	for(i=0; i<=len; i++)
	{
		if(shifts[i] == 0)
			shifts[i] = j;
		if(i == j)
			j = bpos[j];
	}
	return shifts;
}	

int BM(string pattern, string text)
{
    int i = 0;
    int j;
    comparison_bm = 0;
    int pattern_length = pattern.length();
    int text_length = text.length();
    int badShift, goodShift;
    int* badChars = bad_char(pattern);
    int* shifts = good_suffix(pattern);
    shifts[0] = 0;
    while(i <= text_length - pattern_length){
        j = pattern_length - 1;
        while(j >= 0 && ++comparison_bm && pattern[j] == text[i+j]){
            j--;
        }
        if(j < 0){
            return i;
        }
        else{
            uint8_t letter = (uint8_t) text[i+pattern_length];
            letter = letter>>1;
            letter = letter & 0b00000011;
            badShift = MAX(1, j-badChars[letter]);
            goodShift = shifts[0];
            i += MAX(badShift, goodShift);
        }
    }
    return -1;
}

void write_result(string method_name, int index, int comp_num, double runtime){
    printf("%s:\npattern was found in query file at position %d.\n%d  character comparisons performed .\nRun time was %f ms .\n\n", method_name.c_str(), index, comp_num, runtime);
}

char* fasta_reader(char* file_adr)
{
    FILE *in_file;
    in_file = fopen(file_adr, "r");
    struct stat sb;
    stat(file_adr, &sb);
    char* data = (char *) malloc(sb.st_size);
    char letter = 'a';
    int i, j;
    bool read = true;
    for(i = 0, j = 0; i < sb.st_size; i++, j++){
        fscanf(in_file, "%c", &letter);
        if(letter == '>'){
            read = false;
            j = j==0 ?-1 :j-1;
        }
        else if(letter == '\n'){
            read = true;
            j = j==0 ?-1 :j-1;
        }
        else if(read){
            data[j] = letter;
        }
        else{
            j = j==0 ?-1 :j-1;
        }
    }
    length = sb.st_size - (i - j);
    return data;
}

int main(int argc, char * argv[])
{
    typedef chrono::high_resolution_clock timer;
    using chrono::duration;
    char* text_file_adr = NULL;
    char* pattern_file_adr = NULL;
    char* method = NULL;
    int c;
    if(argc == 1){
        printf("This program requires arguments!\n");
        exit(-1);
    }
    while ((c = getopt(argc, argv, "i:o:a:")) != -1){
        switch (c){
            case 'i':
                text_file_adr = optarg;
                break;
            case 'o':
                pattern_file_adr = optarg;
                break;
            case 'a':
                method = optarg;
                break;
            default:
                exit(-1);
        }
    }
    char* pattern = fasta_reader(pattern_file_adr);
    size_t text_len = length;
    char* text = fasta_reader(text_file_adr);
    size_t pattern_len = length;

    if(method[0] == 'A'){
        auto t1 = timer::now();
        int index_bf = brute_force(pattern, text) + 1;
        auto t2 = timer::now();
        int index_kmp = KMP(pattern, text) + 1;
        auto t3 = timer::now();
        int index_bm = BM(pattern, text) + 1;
        auto t4 = timer::now();
        duration<double, milli> runtime_bf = t2 - t1;
        duration<double, milli> runtime_kmp = t3 - t2;
        duration<double, milli> runtime_bm = t4 - t3;
        write_result("Brute Force", index_bf, comparison_bf, runtime_bf.count());
        write_result("Knuth-Morris-Pratt", index_kmp, comparison_kmp, runtime_kmp.count());
        write_result("Boyer-Moore", index_bm, comparison_bm, runtime_bm.count());
        printf("The best algorithm was ");
        if(comparison_bf < comparison_kmp){
            if(comparison_bf < comparison_bm){
                printf("Brute Force.");
            }
            else{
                printf("Boyer-Moore.");
            }
        }
        else if(comparison_bm < comparison_kmp){
            printf("Boyer-Moore.");
        }
        else{
            printf("Knuth-Morris-Pratt.");
        }
    }

    if(method[0] == 'B' && method[1] == 'F'){
        auto t1 = timer::now();
        int index_bf = brute_force(pattern, text) + 1;
        auto t2 = timer::now();
        duration<double, milli> runtime_bf = t2 - t1;
        write_result("Brute Force", index_bf, comparison_bf, runtime_bf.count());
    }

    if(method[0] == 'K' && method[1] == 'M' && method[2] == 'P'){
        auto t1 = timer::now();
        int index_kmp = KMP(pattern, text) + 1;
        auto t2 = timer::now();
        duration<double, milli> runtime_kmp = t2 - t1;
        write_result("Knuth-Morris-Pratt", index_kmp, comparison_kmp, runtime_kmp.count());
    }

    if(method[0] == 'B' && method[1] == 'M'){
        auto t1 = timer::now();
        int index_bm = BM(pattern, text) + 1;
        auto t2 = timer::now();
        duration<double, milli> runtime_bm = t2 - t1;
        write_result("Boyer-Moore", index_bm, comparison_bm, runtime_bm.count());
    }
    
    cout<<endl;
    return 0;
}