#include<iostream>
#include <getopt.h>
#include <sys/stat.h>
// #include<string>

using namespace std;

int main(int argc, char* argv[])
{
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
    cout<<text_file_adr<<endl<<pattern_file_adr<<endl<<method<<endl;
    FILE *pattern_file, *text_file;
    pattern_file = fopen(pattern_file_adr, "r");
    struct stat sb;
    stat(pattern_file_adr, &sb);
    char* pattern = (char *) malloc(sb.st_size);
    char letter = 'a';
    int i, j;
    for(i = 0, j = 0; i < sb.st_size; i++, j++){
        fscanf(pattern_file, "%c", &letter);
        if(letter != '\n')
            pattern[j] = letter;
        else
            j--;
    }
    int pattern_length = sb.st_size - (i - j);
    cout<<pattern_length<<endl;
    
    fclose(pattern_file);
    return 0;
}