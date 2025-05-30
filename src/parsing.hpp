#pragma once
#include <vector>
#include <optional>
#include <variant>
#include "tokenization.hpp"

using namespace std;

struct N_Expr_int{
    Token integer;
};

struct N_Expr_name{
    Token name;
};

struct N_Expr{ //expresion
    variant<N_Expr_int , N_Expr_name> var;
};

struct N_Stam_exit{
    N_Expr expr;
};

struct N_Stam_int{
    Token name;
    N_Expr expr;
};

struct N_Statement{
    variant<N_Stam_exit , N_Stam_int> var;
};

struct N_Program{
    vector<N_Statement> statement;
};




class Parser{
public:

    inline explicit Parser(vector<Token> tokens)
        : m_tokens(move(tokens)){
        }

    optional<N_Expr> parse_expr(){
        
        if(peek().has_value() && peek().value().type == TokenType::integer){
            
            return N_Expr{.var = N_Expr_int{.integer = consume()}};
        }
        
        else if(peek().has_value() && peek().value().type == TokenType::name){
            
            return N_Expr{.var = N_Expr_name {.name = consume()} };
        }
        
        else{
            return {};
        }
    }

    optional<N_Statement> parse_stmt{
        
        if( peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_p){
            consume();
            consume();
            N_Stam_exit stat_exit;
            if(auto expr_n = parse_expr()){
                N_Stam_exit = {.expr = expr_n.value()};
            }
            
            else{
                cerr << "Exit nima vrednosti\n";
                exit(EXIT_FAILURE);
            }
                
            if(peek().has_value() && peek().value().type == TokenType::close_p){
                consume();
            }

            else{
                cerr << "Majnka ')'\n";
                exit(EXIT_FAILURE);
            }
                
            if(peek().has_value() && peek().value().type == TokenType::semi){
                consume();
            }
                
            else{
                cerr << "Ni podpicja\n";
                exit(EXIT_FAILURE);
            }
        return N_Statement{.var = stat_exit};    
        }
    }

    optional<N_Exit> parse(){
        optional<N_Exit> exit_n;
        while(peek().has_value()){
            if(peek().value().type == TokenType::exit && peek(1).has_value() && peek(1).value().type == TokenType::open_p){
                consume();
                consume();
                if(auto expr_n = parse_expr()){
                    exit_n = N_Exit{.expr = expr_n.value()};
                }
                
                else{
                    cerr << "Exit nima vrednosti\n";
                    exit(EXIT_FAILURE);
                }
                
                if(peek().has_value() && peek().value().type == TokenType::close_p){
                    consume();
                }

                else{
                    cerr << "Majnka ')'\n";
                    exit(EXIT_FAILURE);
                }
                
                if(peek().has_value() && peek().value().type == TokenType::semi){
                    consume();
                }
                
                else{
                    cerr << "Ni podpicja\n";
                    exit(EXIT_FAILURE);
                }
            }

        }
        m_index = 0;
        return exit_n;
    }

private:
    

    [[nodiscard]] inline optional<Token> peek(int x = 0) const{
        if(m_index + x >= m_tokens.size()){ // Ce smo na koncu vrene NULL
            return {}; 
        }
        else{ // Drugace vrne trenutni znak
            return m_tokens.at(m_index + x);
        }
    }

    Token inline consume(){ // Vrne trenutni znak in indexu doda 1
        return m_tokens.at(m_index++);
    }

    const vector<Token> m_tokens;
    size_t m_index = 0;
};