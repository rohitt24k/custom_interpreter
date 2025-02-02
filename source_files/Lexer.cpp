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
    if (_sourceCode[_cursor] == '.')
    {
        digit += '.';
        _cursor++;
        _column++;
        while (isdigit(_sourceCode[_cursor]))
        {
            digit += _sourceCode[_cursor];
            _cursor++;
        }
        return Token(Token::TokenType::REAL_CONST, digit, _line, column);
    }
    else
    {
        return Token(Token::TokenType::INTEGER_CONST, digit, _line, column);
    }
}

Token Lexer::_id()
{
    string str("");
    int column = _column;
    if (_sourceCode[_cursor] == '_')
    {
        str += '_';
        _cursor++;
        _column++;

        while (isalnum(_sourceCode[_cursor]))
        {
            str += _sourceCode[_cursor];
            _cursor++;
            _column++;
        }
    }
    else
    {
        while (isalnum(_sourceCode[_cursor]))
        {
            str += _sourceCode[_cursor];
            _cursor++;
            _column++;
        }

        transform(str.begin(), str.end(), str.begin(), ::toupper); // converting the str to uppercase since they are case insensitive in pascal

        if (RESERVED_KEYWORDS.find(str) != RESERVED_KEYWORDS.end())
        {
            return Token(RESERVED_KEYWORDS[str], str, _line, column);
        }
    }

    return Token(Token::TokenType::ID, str, _line, column);
}

Token Lexer::_readString()
{
    // Skip the opening quote.
    _cursor++;
    int column = _column++;
    string stringConst = "";

    while (_cursor < _sourceCode.size() && _sourceCode[_cursor] != '\'')
    {
        if (_sourceCode[_cursor] == '\\')
        {
            _cursor++;
            _column++;
            if (_cursor >= _sourceCode.size())
            {
                Error::throwFatalError(Error::ErrorType::SyntaxError,
                                       "Unterminated string literal", _line, _column);
            }
            stringConst.push_back(_sourceCode[_cursor]);
        }
        else
        {
            stringConst.push_back(_sourceCode[_cursor]);
        }
        _cursor++;
        _column++;
    }

    if (_cursor >= _sourceCode.size() || _sourceCode[_cursor] != '\'')
    {
        Error::throwFatalError(Error::ErrorType::SyntaxError,
                               "Unterminated string literal", _line, _column);
    }

    // Skip the closing quote.
    _cursor++;
    _column++;
    return Token(Token::TokenType::STRING_CONST, stringConst, _line, column);
}

void Lexer::_skipWhiteSpaceCommentNewLine()
{
    while (_cursor < _sourceCode.size() && (_sourceCode[_cursor] == ' ' || _sourceCode[_cursor] == '\n' || _sourceCode[_cursor] == '{'))
    {
        if (_sourceCode[_cursor] == ' ')
        {
            _cursor++;
            _column++;
        }

        if (_sourceCode[_cursor] == '{')
        {
            _cursor++;
            _column++;
            while (_sourceCode[_cursor] != '}')
            {
                _cursor++;
                _column++;
            }
            _cursor++;
            _column++;
        }

        if (_sourceCode[_cursor] == '\n')
        {
            // new line
            _line++;
            _column = 0;
            _cursor++;
        }
    }
}

const char Lexer::getNextChar()
{
    // skip any new lines, spaces or comments
    _skipWhiteSpaceCommentNewLine();
    return _sourceCode[_cursor];
}

Token Lexer::getNextToken()
{

    _skipWhiteSpaceCommentNewLine();

    if (_cursor < _sourceCode.size())
    {
        char currentChar = _sourceCode[_cursor];

        if (isdigit(_sourceCode[_cursor]))
        {
            return _integerConst();
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
            return Token(Token::TokenType::FLOAT_DIV, string(1, currentChar), _line, _column++);
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

        // ID and other string token
        if (isalpha(_sourceCode[_cursor]) || _sourceCode[_cursor] == '_')
        {
            return _id();
        }

        if (_sourceCode[_cursor] == ':' && _peek() == '=')
        {
            _cursor++;
            _cursor++;
            int column = _column;
            _column++;
            _column++;
            return Token(Token::TokenType::ASSIGN, ":=", _line, _column);
        }
        if (_sourceCode[_cursor] == ':')
        {
            _cursor++;
            return Token(Token::TokenType::COLON, ":", _line, _column++);
        }
        if (_sourceCode[_cursor] == ',')
        {
            _cursor++;
            return Token(Token::TokenType::COMMA, ",", _line, _column++);
        }
        if (_sourceCode[_cursor] == ';')
        {
            _cursor++;
            return Token(Token::TokenType::SEMI, ";", _line, _column++);
        }
        if (_sourceCode[_cursor] == '.')
        {
            _cursor++;
            return Token(Token::TokenType::DOT, ".", _line, _column++);
        }
        if (_sourceCode[_cursor] == '\'')
        {
            return _readString();
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
