#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include <vector>

#include "./tokenization.h"

using namespace std;


// PARSING


//PREVOD V ASM---------------------------------------------------------------------------------------------------------------------------
string tokens_to_asm(const vector<Token>& tokens){
    stringstream out;
    out << "global _start\n_start:\n";
    for(int i = 0; i < tokens.size() ;i++){
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit){
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::integer){
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi){
                    out << "    mov rax, 60\n";
                    out << "    mov rdi, " << tokens.at(i + 1).value.value() << endl;
                    out << "    syscall";
                }
            }
        }
    }
    return out.str();
}


int main(int argc , char * argv[]){

    if (argc != 2){
        cerr << "No file to compile \n Example 'rsharp <input.rsh>'\n";
        exit(EXIT_FAILURE);
    }

    string contents;
    {
        stringstream contents_steam;
        fstream input(argv[1] , ios :: in);
        contents_steam << input.rdbuf();
        contents = contents_steam.str();
    }

    Tokenizer tokenizer(move(contents));
    vector<Token> tokens = tokenizer.tokenize();
    {
        fstream file("out.asm" , ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    
    return EXIT_SUCCESS;
}