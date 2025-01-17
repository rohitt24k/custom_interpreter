#include "../header_files/Parser.h"

void Parser::_eat(Token::TokenType type)
{
    if (_currentToken.type() == type)
    {
        _currentToken = _lexer.getNextToken();
    }
    else
    {
        string errorMessage = "Expected token of type '" + Token::tokenTypeToString(type) + "' but found '" +
                              Token::tokenTypeToString(_currentToken.type()) + "' with value '" +
                              _currentToken.value() + "'.";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _currentToken.line(), _currentToken.column());
    }
}

Expr *Parser::_factor()
{
    if (_currentToken.type() == Token::TokenType::INTEGER_CONST)
    {
        Token currentToken = _currentToken;
        _eat(Token::TokenType::INTEGER_CONST);
        return new Num(currentToken, currentToken.value());
    }
    else
    {
        _eat(Token::TokenType::LPAREN);
        Expr *node = _expr();
        _eat(Token::TokenType::RPAREN);
        return node;
    }
}

Expr *Parser::_term()
{
    Expr *node = _factor();

    while (_currentToken.type() == Token::TokenType::MULTIPLICATION || _currentToken.type() == Token::TokenType::INTEGER_DIV)
    {
        Token currentToken = _currentToken;
        if (_currentToken.type() == Token::TokenType::MULTIPLICATION)
        {
            _eat(Token::TokenType::MULTIPLICATION);
        }
        else if (_currentToken.type() == Token::TokenType::INTEGER_DIV)
        {
            _eat(Token::TokenType::INTEGER_DIV);
        }
        node = new BinOp(node, currentToken, _factor());
    }

    return node;
}

Expr *Parser::_expr()
{
    Expr *node = _term();

    while (_currentToken.type() == Token::TokenType::PLUS || _currentToken.type() == Token::TokenType::SUBTRACT)
    {
        Token currentToken = _currentToken;
        if (_currentToken.type() == Token::TokenType::PLUS)
        {
            _eat(Token::TokenType::PLUS);
        }
        else if (_currentToken.type() == Token::TokenType::SUBTRACT)
        {
            _eat(Token::TokenType::SUBTRACT);
        }
        node = new BinOp(node, currentToken, _term());
    }

    return node;
}

AST *Parser::parse()
{
    AST *node = _expr();
    if (_currentToken.type() != Token::TokenType::EOD)
    {
        string errorMessage =
            "Unexpected token '" + _currentToken.value() +
            "' at line " + std::to_string(_currentToken.line()) +
            ", column " + std::to_string(_currentToken.column()) +
            ". Expected end of data (EOD).";

        // Throw a fatal error using the Error class
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _currentToken.line(), _currentToken.column());
    }
    return node;
}