#include <fstream>
#include <string>
#include "hash.h"
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <iomanip>

int spellcheck(hashTable hashtable, std::string input_document, std::string output_file); //spellcheck func declaration

int main(){
    std::string input_dictionary;
    std::string input_document;
    std::string output_file;
    hashTable hashtable(50000);


    clock_t start, end; //time function (returns clock ticks in elapsed time)

    printf("Input Dictionary File: ");
    std::cin>>input_dictionary;

    std::ifstream dictionary; //declare stream object
    dictionary.open(input_dictionary); //associate input_dictionary with dictionary object

    printf("Input Document File: ");
    std::cin>>input_document;

    printf("Output File: ");
    std::cin>>output_file;

    //load dictionary
    std::string line;
    if(dictionary.is_open()){

        start = clock();
        while(!dictionary.eof()){
            dictionary >> line;
            std::transform(line.begin(),line.end(), line.begin(),
                           [](unsigned char c){ return std::tolower(c); }); //turn string to lowercase
            hashtable.insert(line);
        }
        end = clock();

        std::cout<<"Dictionary loading time: " <<std::fixed<<double(end-start)/double(CLOCKS_PER_SEC)<<std::setprecision(5)<<"seconds"<<std::endl;
    }
    else{
        printf("The dictionary file cannot be opened.\n");
        return EXIT_FAILURE;
    }
    dictionary.close(); //close dictionary

    //spell check
    start = clock();
    spellcheck(hashtable, input_document, output_file);
    end = clock();

    std::cout<<"Spellcheck time: "<< std::fixed<< double(end-start)/ double(CLOCKS_PER_SEC)<< std::setprecision(5) <<"seconds"<<std::endl;
    return 0;
}

int spellcheck(hashTable hashtable, std::string input_document, std::string output_file) {
    std::string temp_string;
    std::string temp_word;
    int line_num = 0;
    bool has_num = false;

    std::ifstream document; //declare stream object
    document.open(input_document); //associate input_dictionary with dictionary object

    std::ofstream outputfile; //declare stream object
    outputfile.open(output_file); //associate input_dictionary with dictionary object

    if (document.is_open()) {
        while (std::getline(document, temp_string)) {
            line_num++;
            temp_word = "";
            has_num = false;

            for (int i = 0; i < temp_string.size() + 1; i++) {
                char c = tolower(temp_string[i]);

                if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
                    c == 39 || c == 45) { //compare ascii values
                    if (c >= '0' && c <= '9') { //spec. check if contains number
                        has_num = true;
                    }
                    temp_word += c;
                } else {
                    if (temp_word != "") {
                        if (temp_word.size() > 20) { //larger than 20 chars
                            outputfile << "Long word at line " << line_num << ", starts: " << temp_word.substr(0, 20)
                                       << std::endl;
                        } else if ((!hashtable.contains(temp_word)) && !has_num) {
                            outputfile << "Unknown word at line " << line_num << ": " << temp_word << std::endl;
                        }
                    }
                    temp_word = "";
                    has_num = false;
                }
            }
        }
    } else {
        printf("The document file cannot be opened.\n");
        return EXIT_FAILURE;
    }
    document.close(); //close dictionary
    outputfile.close();

    return 0;
}