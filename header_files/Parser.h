#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"

class Parser
{
private:
    Lexer _lexer;
    Token _currentToken;
    void _eat(Token::TokenType type);
    Expr *_expr();
    Expr *_term();
    Expr *_factor();

public:
    Parser(Lexer &lexer) : _lexer(lexer), _currentToken(Token(Token::TokenType::EOD, "", 0, 0))
    {
        _currentToken = _lexer.getNextToken();
    }
    AST *parse();
};

#endif // PARSER_H
