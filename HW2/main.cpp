#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    FILE *in; 
    char* file_adr;
    file_adr = argv[1];
    in = fopen(file_adr, "r");
    
    vector<string> patterns;
    vector<string> words;
    string word;
    bool text_started = false;
    char temp;
    int i = 0;

    while(fscanf(in, "%c", &temp) != EOF){
        if(temp == '\n'){
            text_started = true;
            i = 0;
        }
        else if(temp == ' '){
            i++;
            if(text_started)
                words.push_back(word);
            else
                patterns.push_back(word);
            word.clear();
        }
        else{
            word.push_back(temp);
        }
    }
    
    

    cout<<endl;
    return 0;
}