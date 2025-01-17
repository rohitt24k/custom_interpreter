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

    unordered_map<string, TokenType> RESERVED_KEYWORDS = {
        {"BEGIN", TokenType::BEGIN},
        {"END", TokenType::END},
        {"DIV", TokenType::INTEGER_DIV},
        {"INTEGER", TokenType::INTEGER},
        {"REAL", TokenType::REAL},
        {"PROGRAM", TokenType::PROGRAM},
        {"VAR", TokenType::VAR},
        {"PROCEDURE", TokenType::PROCEDURE},
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
    static string tokenTypeToString(TokenType type);
};

#endif // TOKEN_H