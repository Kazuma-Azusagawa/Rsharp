#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include <vector>
#include "./tokenization.hpp"
#include "./parsing.hpp"
#include "./generation.hpp"

/*

  RRRRRRRRRRRRRRRRR                                                               
  R::::::::::::::::R                                                               
  R::::::RRRRRR:::::R                                                             
  RR:::::R     R:::::R                                                            
    R::::R     R:::::R   ooooooooooo xxxxxxx      xxxxxxxyyyyyyy           yyyyyyy
    R::::R     R:::::R oo:::::::::::oox:::::x    x:::::x  y:::::y         y:::::y 
    R::::RRRRRR:::::R o:::::::::::::::ox:::::x  x:::::x    y:::::y       y:::::y  
    R:::::::::::::RR  o:::::ooooo:::::o x:::::xx:::::x      y:::::y     y:::::y   
    R::::RRRRRR:::::R o::::o     o::::o  x::::::::::x        y:::::y   y:::::y    
    R::::R     R:::::Ro::::o     o::::o   x::::::::x          y:::::y y:::::y     
    R::::R     R:::::Ro::::o     o::::o   x::::::::x           y:::::y:::::y      
    R::::R     R:::::Ro::::o     o::::o  x::::::::::x           y:::::::::y       
  RR:::::R     R:::::Ro:::::ooooo:::::o x:::::xx:::::x           y:::::::y        
  R::::::R     R:::::Ro:::::::::::::::ox:::::x  x:::::x           y:::::y         
  R::::::R     R:::::R oo:::::::::::oox:::::x    x:::::x         y:::::y          
  RRRRRRRR     RRRRRRR   ooooooooooo xxxxxxx      xxxxxxx       y:::::y           
                                                               y:::::y            
                                                              y:::::y             
                                                             y:::::y              
                                                            y:::::y               
                                                           yyyyyyy     

*/

int main(int argc , char * argv[]){

    if (argc != 2){ //Ends program if no arg is given
        cerr << "No file to compile \n Example 'roxy <input.rsh> /*<output>*/'\n";
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
    
    Parser parser(move(tokens));
    optional<N_Exit> tree = parser.parse();

    if(!tree.has_value()){
        cerr << "Naredil si retard napako\n";
        exit(EXIT_FAILURE);
    }
    
    Generator generator(tree.value());

    {
        fstream file("out.asm" , ios::out); 
        file << generator.generate();// turns tokens to asm and puts them into out.asm
    }
    // Linux Commands ---------------------------------------------------
    system("nasm -felf64 out.asm"); // turns .asm file into .o file
    system("ld -o out out.o"); //turns .o file into executable
    
    return EXIT_SUCCESS;
}
