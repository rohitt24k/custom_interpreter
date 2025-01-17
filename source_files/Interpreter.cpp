#include "../header_files/Interpreter.h"

Token Interpreter::_number()
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

Token Interpreter::_getNextToken()
{

    while (_cursor < _sourceCode.size() && _sourceCode[_cursor] == ' ')
    {
        if (_sourceCode[_cursor] == ' ')
            _cursor++;
    }

    if (_cursor < _sourceCode.size())
    {
        char currentChar = _sourceCode[_cursor];

        if (isdigit(_sourceCode[_cursor]))
        {
            return _number();
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

        string errorMessage = "Unexpected character '" + string(1, currentChar) + "' at line " +
                              to_string(_line) + ", column " + to_string(_column) + ".";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _line, _column);
    }
    else
    {
        return Token(Token::TokenType::EOD, "", _line, _column);
    }
}

void Interpreter::_eat(Token::TokenType type)
{
    if (_currentToken.type() == type)
    {
        _currentToken = _getNextToken();
    }
    else
    {
        string errorMessage = "Expected token of type '" + Token::tokenTypeToString(type) + "' but found '" +
                              Token::tokenTypeToString(_currentToken.type()) + "' with value '" +
                              _currentToken.value() + "'.";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _currentToken.line(), _currentToken.column());
    }
}

int Interpreter::_term()
{
    Token currentToken = _currentToken;
    _eat(Token::TokenType::INTEGER_CONST);
    return stoi(currentToken.value());
}

int Interpreter::expr()
{

    int result = _term();

    while (_currentToken.type() == Token::TokenType::PLUS || _currentToken.type() == Token::TokenType::SUBTRACT)
    {
        if (_currentToken.type() == Token::TokenType::PLUS)
        {
            _eat(Token::TokenType::PLUS);
            result = result + _term();
        }
        else if (_currentToken.type() == Token::TokenType::SUBTRACT)
        {
            _eat(Token::TokenType::SUBTRACT);
            result = result - _term();
        }
    }

    return result;
}