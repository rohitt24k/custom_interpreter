#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Error.h"

class Interpreter
{
private:
    Token _currentToken;
    string _sourceCode;
    int _line = 0;
    int _column = 0;
    int _cursor = 0;
    Token _number();
    Token _getNextToken();
    int _term();

    void _eat(Token::TokenType type);

public:
    Interpreter(string sourceCode) : _sourceCode(sourceCode), _currentToken(Token(Token::TokenType::EOD, "", 0, 0))
    {
        _currentToken = _getNextToken();
    }

    int expr();
};

#endif // PARSER_H