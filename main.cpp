#include <fstream>
#include "./tokenization.h"

//Prevede tokene v asm codo---------------------------------------------------------------------------------------------------------------------------
//asm. syscall sheat: https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md
string tokens_to_asm(const vector<Token>& tokens){
    stringstream out; 
    out << "global _start\n_start:\n"; //naredi start funkcijo in jo naredi dostopno do cpuja
    for(int i = 0; i < tokens.size() ;i++){
        const Token& token = tokens.at(i);
        if (token.type == TokenType::exit){ // pregleda ce ima exit token
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::integer){ // pregleda ce ima int token
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi){ // pregleda ce ima podpicje token
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

    if (argc != 2){ //Ends program if no arg is given
        cerr << "No file to compile \n Example 'rsharp <input.rsh>'\n";
        exit(EXIT_FAILURE);
    }

    string contents; //Grabs the text from arg file and  turns it into a string
    {
        stringstream contents_steam;
        fstream input(argv[1] , ios :: in);
        contents_steam << input.rdbuf();
        contents = contents_steam.str();
    }

    Tokenizer tokenizer(move(contents)); //purt the string into the tokenizer
    vector<Token> tokens = tokenizer.tokenize(); // Turns the string int a vector of tokens
    {
        fstream file("out.asm" , ios::out); 
        file << tokens_to_asm(tokens); // turns tokens to asm and puts them into out.asm
    }

    system("nasm -felf64 out.asm"); // turns .asm file into .o file
    system("ld -o out out.o"); //turns .o file into executable
    
    return EXIT_SUCCESS;
}
