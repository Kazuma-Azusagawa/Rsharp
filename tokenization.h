#pragma once
#include <sstream>
#include <iostream>
#include <optional>
#include <vector>

using namespace std;

//VSE VRSTE TOKENOV
enum class TokenType {
    exit,
    integer,
    semi
};

//TOKEN
struct Token{
    TokenType type;
    optional<string> value {};
};

class Tokenizer{
public:
    inline Tokenizer(const string& src)
        : m_src(move(src))
    {        
    }

    inline vector<Token> tokenize(){

        vector<Token> tokens;
        string buffer;

        while(peak().has_value()){
            if(isalpha(peak().value())){
                buffer.push_back(consume());
                while(peak().has_value() && isalpha(peak().value())){
                    buffer.push_back(consume());
                }
                if(buffer == "exit"){
                    tokens.push_back({.type = TokenType::exit });
                    buffer.clear();
                    continue;
                } else {
                    cerr << "ERROR!\n";
                    exit(EXIT_FAILURE);                  
                }
            }
            else if(isdigit(peak().value())){
                buffer.push_back(consume());
                while(peak().has_value() && isdigit(peak().value())){
                    buffer.push_back(consume());
                }
                tokens.push_back({.type = TokenType::integer, .value = buffer});
                buffer.clear();
                
            }
            else if(peak().value() == ';'){
                consume();
                tokens.push_back({.type = TokenType::semi,});
            }
            else if(isspace(peak().value())){
                consume();
                continue;
            }
            else{
                cerr << "ERROR!\n";
                exit(EXIT_FAILURE);
            }
        }
    m_index = 0;
    return tokens;
    }


private:

    [[nodiscard]] optional<char> peak(int x = 1) const 
    {
        if(m_index + x > m_src.length()){
            return {};
        }
        else{
            return m_src.at(m_index);
        }
    }

    char consume(){
        return m_src.at(m_index++);
    }

    const string m_src;
    int m_index = 0;
};