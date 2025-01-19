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
    else if (_currentToken.type() == Token::TokenType::PLUS)
    {
        Token currentToken = _currentToken;
        _eat(Token::TokenType::PLUS);
        return new UniaryOp(currentToken, _factor());
    }
    else if (_currentToken.type() == Token::TokenType::SUBTRACT)
    {
        Token currentToken = _currentToken;
        _eat(Token::TokenType::SUBTRACT);
        return new UniaryOp(currentToken, _factor());
    }
    else if (_currentToken.type() == Token::TokenType::LPAREN)
    {
        _eat(Token::TokenType::LPAREN);
        Expr *node = _expr();
        _eat(Token::TokenType::RPAREN);
        return node;
    }
    else if (_currentToken.type() == Token::TokenType::ID)
    {

        Var *node = new Var(_currentToken, _currentToken.value());
        _eat(Token::TokenType::ID);
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

Var *Parser::_variable()
{
    Token currentToken = _currentToken;
    _eat(Token::TokenType::ID);
    return new Var(currentToken, currentToken.value());
}

Program *Parser::_program()
{
    CompoundStatement *compoundStatement = _compoundStatement();
    _eat(Token::TokenType::DOT);
    return new Program(compoundStatement);
}

CompoundStatement *Parser::_compoundStatement()
{
    _eat(Token::TokenType::BEGIN);
    vector<Statement *> statements = _statementList();
    _eat(Token::TokenType::END);
    return new CompoundStatement(statements);
}

vector<Statement *> Parser::_statementList()
{
    vector<Statement *> statements;

    statements.push_back(_statement());

    while (_currentToken.type() == Token::TokenType::SEMI)
    {
        _eat(Token::TokenType::SEMI);
        statements.push_back(_statement());
    }
    return statements;
}

Statement *Parser::_statement()
{

    if (_currentToken.type() == Token::TokenType::BEGIN)
    {
        return _compoundStatement();
    }
    if (_currentToken.type() == Token::TokenType::ID)
    {
        return _assignmentStatement();
    }

    return _empty();
}

AssignmentStatement *Parser::_assignmentStatement()
{
    Var *left = _variable();
    Token op = _currentToken;
    _eat(Token::TokenType::ASSIGN);
    Expr *right = _expr();
    return new AssignmentStatement(left, op, right);
}

NoOp *Parser::_empty()
{
    return new NoOp();
}

AST *Parser::parse()
{
    AST *node = _program();
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