#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include "Error.h"

class Lexer
{
private:
    int _line = 1;
    int _column = 0;
    int _cursor = 0;
    string _sourceCode;
    Token _integerConst();

public:
    Lexer(string sourceCode) : _sourceCode(sourceCode) {}
    Token getNextToken();
};

#endif // LEXER_H