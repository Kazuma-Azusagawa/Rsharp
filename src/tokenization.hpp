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
    semi,
    open_p,
    close_p,
    name
};

//Struktura tokena
struct Token{
    TokenType type;
    optional<string> value {};
};


// spremeni podan string v tokene
class Tokenizer{
public:
    inline Tokenizer(const string& src)
        : m_src(move(src)){ // da string v m_src
            
    }

    inline vector<Token> tokenize(){

        vector<Token> tokens; // Vektor tokeno
        string buffer; // Buffer za vrednosti ki bodo dane v token

        while(peek().has_value()){
            if(isalpha(peek().value())){ // Pregled za text zazaj samo pregleda ce pise "exit"
                buffer.push_back(consume()); 
                while(peek().has_value() && isalpha(peek().value())){
                    buffer.push_back(consume());
                }
                if(buffer == "exit"){
                    tokens.push_back({.type = TokenType::exit});
                    buffer.clear();
                    continue;
                } else {
                    tokens.push_back({.type = TokenType::name , .value = buffer});
                    buffer.clear();             
                }
            }
            else if(isdigit(peek().value())){ // Pregled za stevilke
                buffer.push_back(consume());
                while(peek().has_value() && isdigit(peek().value())){
                    buffer.push_back(consume());
                }
                tokens.push_back({.type = TokenType::integer, .value = buffer});
                buffer.clear();
                
            }
            else if(peek().value() == '('){
                consume();
                tokens.push_back({.type = TokenType::open_p});
            }
            else if(peek().value() == ')'){
                consume();
                tokens.push_back({.type = TokenType::close_p});
            }
            else if(peek().value() == ';'){ // Pregled za podpicje
                consume();
                tokens.push_back({.type = TokenType::semi});
            }
            else if(isspace(peek().value())){ //space bar / drugi whitespace 
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
    [[nodiscard]] inline optional<char> peek(int x = 0) const  
    {
        if(m_index + x >= m_src.length()){ // Ce smo na koncu vrene NULL
            return {}; 
        }
        else{ // Drugace vrne trenutni znak
            return m_src.at(m_index + x);
        }
    }

    char inline consume(){ // Vrne trenutni znak in indexu doda 1
        return m_src.at(m_index++);
    }

    const string m_src;
    int m_index = 0;
};
