#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include "Error.h"
#include <algorithm>

class Lexer
{
private:
    int _line = 1;
    int _column = 0;
    int _cursor = 0;
    string _sourceCode;
    Token _integerConst();
    Token _id();
    char _peek()
    {
        int peekCursor = _cursor + 1;
        if (peekCursor >= _sourceCode.size())
        {
            return '\0';
        }

        return _sourceCode[peekCursor];
    }

public:
    Lexer(string sourceCode) : _sourceCode(sourceCode) {}
    Token getNextToken();
    const char currentChar() const { return _sourceCode[_cursor]; }
};

#endif // LEXER_H