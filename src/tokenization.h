#pragma once
#include <sstream>
#include <iostream>
#include <optional>
#include <vector>

using namespace std;

//Vse vrste tokenov
enum class TokenType {
    exit,
    integer,
    semi
};

//Struktura tokena
struct Token{
    TokenType type;
    optional<string> value {};
};



class Tokenizer{
public:
    inline Tokenizer(const string& src)
        : m_src(move(src)) // da string v m_src
    {        
    }

    inline vector<Token> tokenize(){

        vector<Token> tokens; // Vektor tokeno
        string buffer; // Buffer za vrednosti ki bodo dane v token

        while(peak().has_value()){
            if(isalpha(peak().value())){ // Pregled za text zazaj samo pregleda ce pise "exit"
                buffer.push_back(consume()); 
                while(peak().has_value() && isalpha(peak().value())){
                    buffer.push_back(consume());
                }
                if(buffer == "exit"){
                    tokens.push_back({.type = TokenType::exit });
                    buffer.clear();
                    continue;
                } else {
                    cerr << "ERROR!\n"; // Ce neje pise "exit" vrne ERROR
                    exit(EXIT_FAILURE);                  
                }
            }
            else if(isdigit(peak().value())){ // Pregled za stevilke
                buffer.push_back(consume());
                while(peak().has_value() && isdigit(peak().value())){
                    buffer.push_back(consume());
                }
                tokens.push_back({.type = TokenType::integer, .value = buffer});
                buffer.clear();
                
            }
            else if(peak().value() == ';'){ // Pregled za podpicje
                consume();
                tokens.push_back({.type = TokenType::semi,});
            }
            else if(isspace(peak().value())){ //space bar / drugi whitespace 
                consume();
                continue;
            }
            else{ // Ce nima vrednosti vrne ERROR
                cerr << "ERROR!\n";
                exit(EXIT_FAILURE);
            }
        }
    m_index = 0;
    return tokens;
    }


private:
    // Pogleda kje smo v stringu
    [[nodiscard]] optional<char> peak(int x = 1) const  
    {
        if(m_index + x > m_src.length()){ // Ce smo na koncu vrene NULL
            return {}; 
        }
        else{ // Drugace vrne trenutni znak
            return m_src.at(m_index);
        }
    }

    char consume(){ // Vrne trenutni znak in indexu doda 1
        return m_src.at(m_index++);
    }

    const string m_src;
    int m_index = 0;
};
