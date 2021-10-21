#include<iostream>
#include<getopt.h>
#include <chrono>
#include<sys/stat.h>

using namespace std;
size_t length = 0;
size_t comparison_bf;
size_t comparison_kmp;
size_t comparison_bm;

//g++ hw1.cpp && ./a.out -i text.fa -o pat.fa -a A

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
        comparison_kmp++;
        if(text[i] == pattern[j]){
            if(j == pattern_length-1){
                return i-j;
            }
            else{
                i++;
                j++;
            }
        }
        else if(j > 0){
            j = failure[j-1];
        }
        else{
            i++;
            j = 0;
        }
    }
    return -1;
}

void good_suffix_1(int *shift, int *bpos, string pat, int m)
{
    int i=m, j=m+1;
    bpos[i]=j;
  
    while(i>0)
    {
        while(j<=m && pat[i-1] != pat[j-1])
        {
            if (shift[j]==0)
                shift[j] = j-i;
            j = bpos[j];
        }
        i--;
        j--;
        bpos[i] = j; 
    }
}

void good_suffix_2(int *shift, int *bpos, string pat, int m)
{
    int i, j;
    j = bpos[0];
    for(i=0; i<=m; i++)
    {
        if(shift[i]==0)
            shift[i] = j;
        if (i==j)
            j = bpos[j];
    }
}

int BM(string pattern, string text)
{
    int s=0, j;
    int pattern_length = pattern.length();
    int text_length = text.length();
    comparison_bm = 0;
  
    int bpos[pattern_length+1], shift[pattern_length+1];
  
    for(int i=0;i<pattern_length+1;i++) 
        shift[i]=0;
  
    good_suffix_1(shift, bpos, pattern, pattern_length);
    good_suffix_2(shift, bpos, pattern, pattern_length);
  
    while(s <= text_length-pattern_length)
    {
        j = pattern_length-1;
        while(j >= 0 && pattern[j] == text[s+j] && ++comparison_bm){
            j--;
        }
        if (j<0)
        {
            return s;
        }
        else
            s += shift[j+1];
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
    for(i = 0, j = 0; i < sb.st_size; i++, j++){
        fscanf(in_file, "%c", &letter);
        if(letter != '\n')
            data[j] = letter;
        else
            j--;
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
        write_result("Knuth-Morris-Pratt", index_bf, comparison_kmp, runtime_kmp.count());
        write_result("Boyer-Moore", index_bf, comparison_bm, runtime_bm.count());
        printf("The best algorithm was ");
        if(runtime_bf < runtime_kmp){
            if(runtime_bf < runtime_bm){
                printf("Brute Force.");
            }
            else{
                printf("Boyer-Moore.");
            }
        }
        else if(runtime_bm < runtime_kmp){
            printf("Boyer-Moore.");
        }
        else{
            printf("Knuth-Morris-Pratt.");
        }
    }

    if(method[0] == 'B' && method[1] == 'F'){
        int index_bf = brute_force(pattern, text) + 1;
        write_result("Brute Force", index_bf, 0, 0);
    }

    if(method[0] == 'K' && method[1] == 'M' && method[2] == 'P'){
        int index_kmp = KMP(pattern, text) + 1;
        write_result("Knuth-Morris-Pratt", index_kmp, 0, 0);
    }

    if(method[0] == 'B' && method[1] == 'M'){
        int index_bm = BM(pattern, text) + 1;
        write_result("Boyer-Moore", index_bm, 0, 0);
    }
    
    cout<<endl<<endl;
    return 0;
}