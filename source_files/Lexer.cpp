#include "../header_files/Lexer.h"

Token Lexer::_integerConst()
{
    string digit = "";
    int column = _column;
    while (isdigit(_sourceCode[_cursor]))
    {
        _column++;
        digit += _sourceCode[_cursor++];
    }
    return Token(Token::TokenType::INTEGER_CONST, digit, _line, column);
}

Token Lexer::getNextToken()
{

    while (_cursor < _sourceCode.size() && _sourceCode[_cursor] == ' ')
    {
        if (_sourceCode[_cursor] == ' ')
        {
            _cursor++;
            _column++;
        }
    }

    if (_cursor < _sourceCode.size())
    {
        char currentChar = _sourceCode[_cursor];

        if (isdigit(_sourceCode[_cursor]))
        {
            return _integerConst();
        }

        if (currentChar == '\n')
        {
            // new line
            _line++;
            _column = 0;
        }

        if (currentChar == '+')
        {
            _cursor++;
            return Token(Token::TokenType::PLUS, string(1, currentChar), _line, _column++);
        }

        if (currentChar == '-')
        {
            _cursor++;
            return Token(Token::TokenType::SUBTRACT, string(1, currentChar), _line, _column++);
        }

        if (currentChar == '*')
        {
            _cursor++;
            return Token(Token::TokenType::MULTIPLICATION, string(1, currentChar), _line, _column++);
        }

        if (currentChar == '/')
        {
            _cursor++;
            return Token(Token::TokenType::INTEGER_DIV, string(1, currentChar), _line, _column++);
        }

        if (currentChar == '(')
        {
            _cursor++;
            return Token(Token::TokenType::LPAREN, string(1, currentChar), _line, _column++);
        }
        if (currentChar == ')')
        {
            _cursor++;
            return Token(Token::TokenType::RPAREN, string(1, currentChar), _line, _column++);
        }

        string errorMessage = "Unexpected character '" + string(1, currentChar) + "' at line " +
                              to_string(_line) + ", column " + to_string(_column) + ".";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _line, _column);
    }
    else
    {
        return Token(Token::TokenType::EOD, "", _line, _column);
    }
}
