#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>
using namespace std;

class Token
{
public:
    enum class TokenType
    {
        INTEGER, // THIS IS TO MEAN INTEGET TYPE AND NOT INTERGER CONSTANT
        REAL,    // THIS IS LIKE FLOAT IN PASCAL
        STRING,
        PLUS,
        SUBTRACT,
        MULTIPLICATION,
        LPAREN,
        RPAREN,
        BEGIN,
        END,
        DOT,
        ID,
        ASSIGN,
        SEMI,
        PROGRAM,
        VAR,
        COLON,
        COMMA,
        INTEGER_CONST,
        REAL_CONST,
        INTEGER_DIV,
        FLOAT_DIV,
        PROCEDURE,
        EOD, // end of document
    };

private:
    TokenType _type;
    string _value;
    int _line;
    int _column;

public:
    Token(TokenType type, string value, int line, int column) : _type(type), _value(value), _line(line), _column(column) {};
    TokenType type() const { return _type; };
    string value() const { return _value; };
    int line() const { return _line; };
    int column() const { return _column; };
    string typeAsString()
    {
        return tokenTypeToString(_type);
    }
    static string tokenTypeToString(TokenType type);
};

extern unordered_map<string, Token::TokenType> RESERVED_KEYWORDS;

#endif // TOKEN_H