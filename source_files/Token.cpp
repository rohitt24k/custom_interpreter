#include "../header_files/Token.h"

string Token::tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::INTEGER:
        return "Integer";
    case TokenType::REAL:
        return "Real";
    case TokenType::PLUS:
        return "Plus";
    case TokenType::SUBTRACT:
        return "Subtract";
    case TokenType::MULTIPLICATION:
        return "Multiplication";
    case TokenType::OPENBRACKET:
        return "Open Bracket";
    case TokenType::CLOSEBRACKET:
        return "Close Bracket";
    case TokenType::BEGIN:
        return "Begin";
    case TokenType::END:
        return "End";
    case TokenType::DOT:
        return "Dot";
    case TokenType::ID:
        return "Id";
    case TokenType::ASSIGN:
        return "Assign";
    case TokenType::SEMI:
        return "Semi";
    case TokenType::PROGRAM:
        return "Program";
    case TokenType::VAR:
        return "Var";
    case TokenType::COLON:
        return "Colon";
    case TokenType::COMMA:
        return "Comma";
    case TokenType::INTEGER_CONST:
        return "Integer_const";
    case TokenType::REAL_CONST:
        return "Real_const";
    case TokenType::INTEGER_DIV:
        return "Integer_div";
    case TokenType::FLOAT_DIV:
        return "Float_div";
    case TokenType::PROCEDURE:
        return "Procedure";
    case TokenType::EOD:
        return "EOD";
    default:
        return "Unknown";
    }
}