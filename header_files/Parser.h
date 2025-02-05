#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"
#include <unordered_set>

class Parser
{
private:
    Lexer _lexer;
    Token _currentToken;
    void _eat(Token::TokenType type);

    Expr *_expr();
    Expr *_term();
    Expr *_factor();
    Var *_variable();

    Condition *_condition();

    Program *_program();

    Block *_block();

    vector<Declarations *> _declarations();
    vector<Param *> _formalParameterList();
    vector<Param *> _formalPamaters();
    vector<VarDecl *> _variableDeclarations();

    Type *_typeSpec();

    CompoundStatement *_compoundStatement();
    vector<Statement *> _statementList();
    Statement *_statement();
    AssignmentStatement *_assignmentStatement();
    ProcedureCallStatement *_procedureCallStatement();
    IfelseStatement *_ifelseStatement();
    NoOp *_empty();

public:
    Parser(Lexer &lexer) : _lexer(lexer), _currentToken(Token(Token::TokenType::EOD, "", 0, 0))
    {
        _currentToken = _lexer.getNextToken();
    }
    AST *parse();
};

#endif // PARSER_H
