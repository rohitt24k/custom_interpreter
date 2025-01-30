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
    else if (_currentToken.type() == Token::TokenType::REAL_CONST)
    {
        Token currentToken = _currentToken;
        _eat(Token::TokenType::REAL_CONST);
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
    else
    {
        // If none of the expected token types match, throw a syntax error.
        string errorMessage = "Unexpected token '" + _currentToken.value() +
                              "' of type '" + Token::tokenTypeToString(_currentToken.type()) +
                              "' found while parsing a factor.";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _currentToken.line(), _currentToken.column());
        // The program will exit, so no need for a return statement.
    }
}

Expr *Parser::_term()
{
    Expr *node = _factor();

    while (_currentToken.type() == Token::TokenType::MULTIPLICATION || _currentToken.type() == Token::TokenType::INTEGER_DIV || _currentToken.type() == Token::TokenType::FLOAT_DIV)
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
        else if (_currentToken.type() == Token::TokenType::FLOAT_DIV)
        {
            _eat(Token::TokenType::FLOAT_DIV);
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
    _eat(Token::TokenType::PROGRAM);
    Var *var = _variable();
    _eat(Token::TokenType::SEMI);
    Block *block = _block();
    _eat(Token::TokenType::DOT);

    string programName = var->value();

    return new Program(programName, block);
}

Block *Parser::_block()
{
    vector<Declarations *> declarations = _declarations();
    CompoundStatement *compoundStatement = _compoundStatement();
    return new Block(declarations, compoundStatement);
}

vector<Param *> Parser::_formalPamaters()
{
    vector<Var *> variables;

    variables.push_back(_variable());

    while (_currentToken.type() == Token::TokenType::COMMA)
    {
        _eat(Token::TokenType::COMMA);
        variables.push_back(_variable());
    }

    _eat(Token::TokenType::COLON);
    Type *type = _typeSpec();
    vector<Param *> params;

    for (auto var : variables)
    {
        params.push_back(new Param(var, type));
    }

    return params;
}

vector<Param *> Parser::_formalParameterList()
{
    vector<Param *> params;

    for (auto p : _formalPamaters())
    {
        params.push_back(p);
    }

    if (_currentToken.type() == Token::TokenType::SEMI)
    {
        _eat(Token::TokenType::SEMI);
        for (auto p : _formalParameterList())
        {
            params.push_back(p);
        }
    }

    return params;
}

vector<Declarations *> Parser::_declarations()
{
    vector<Declarations *> declarations;

    if (_currentToken.type() == Token::TokenType::VAR)
    {
        _eat(Token::TokenType::VAR);
        for (auto declaration : _variableDeclarations())
        {
            declarations.push_back(declaration);
        }
        _eat(Token::TokenType::SEMI);

        while (_currentToken.type() == Token::TokenType::ID)
        {
            for (auto declaration : _variableDeclarations())
            {

                declarations.push_back(declaration);
            }
            _eat(Token::TokenType::SEMI);
        }
    }

    while (_currentToken.type() == Token::TokenType::PROCEDURE)
    {
        _eat(Token::TokenType::PROCEDURE);
        string procedureName = _currentToken.value();
        _eat(Token::TokenType::ID);

        vector<Param *> params;

        if (_currentToken.type() == Token::TokenType::LPAREN)
        {
            _eat(Token::TokenType::LPAREN);
            params = _formalParameterList();
            _eat(Token::TokenType::RPAREN);
        }

        _eat(Token::TokenType::SEMI);
        Block *procedureBlock = _block();
        _eat(Token::TokenType::SEMI);
        declarations.push_back(new ProcedureDecl(procedureName, params, procedureBlock));
    }

    return declarations;
}

vector<VarDecl *> Parser::_variableDeclarations()
{
    vector<Var *> variables;

    variables.push_back(_variable());

    while (_currentToken.type() == Token::TokenType::COMMA)
    {
        _eat(Token::TokenType::COMMA);
        variables.push_back(_variable());
    }

    _eat(Token::TokenType::COLON);
    Type *type = _typeSpec();
    vector<VarDecl *> declarations;

    for (auto var : variables)
    {
        declarations.push_back(new VarDecl(var, type));
    }
    return declarations;
}

Type *Parser::_typeSpec()
{
    Token token = _currentToken;
    if (token.type() == Token::TokenType::INTEGER)
    {
        _eat(Token::TokenType::INTEGER);
    }
    else if (token.type() == Token::TokenType::REAL)
    {
        _eat(Token::TokenType::REAL);
    }
    else
    {
        string errorMessage = "Unexpected token '" + _currentToken.value() +
                              "' of type '" + Token::tokenTypeToString(_currentToken.type()) +
                              "' found while parsing a typeSpec.";
        Error::throwFatalError(Error::ErrorType::SyntaxError, errorMessage, _currentToken.line(), _currentToken.column());
    }
    Type *node = new Type(token, token.value());
    return node;
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
        if (_lexer.currentChar() == '(')
        {
            return _procedureCallStatement();
        }
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

ProcedureCallStatement *Parser::_procedureCallStatement()
{
    Token token = _currentToken;
    string procedureName = _currentToken.value();
    _eat(Token::TokenType::ID);
    _eat(Token::TokenType::LPAREN);

    vector<Expr *> actualParams;

    if (_currentToken.type() != Token::TokenType::RPAREN)
    {
        actualParams.push_back(_expr());
        while (_currentToken.type() == Token::TokenType::COMMA)
        {
            _eat(Token::TokenType::COMMA);
            actualParams.push_back(_expr());
        }
    }

    _eat(Token::TokenType::RPAREN);

    return new ProcedureCallStatement(procedureName, actualParams, token);
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