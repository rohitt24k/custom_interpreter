#ifndef AST_H
#define AST_H

#include "Token.h"
#include "vector"
#include "Symbol.h"

class AST
{
public:
    virtual ~AST() {}
};

class Expr : public AST
{
public:
    virtual ~Expr() {};
};

class Num : public Expr
{
private:
    Token _token;
    string _value;

public:
    Num(Token token, string value) : _token(token), _value(value) {}
    Token token() const { return _token; }
    string value() const { return _value; }
};

class StringLiteral : public Expr
{
private:
    Token _token;
    string _value;

public:
    StringLiteral(Token token, string value) : _token(token), _value(value) {}
    Token token() const { return _token; }
    string value() const { return _value; }
};

class BinOp : public Expr
{
private:
    Expr *_left;
    Token _op;
    Expr *_right;

public:
    BinOp(Expr *left, Token token, Expr *right) : _left(left), _right(right), _op(token) {}
    Expr *left() const { return _left; }
    Expr *right() const { return _right; }
    Token op() const { return _op; }
};

class UniaryOp : public Expr
{
private:
    Token _op;
    Expr *_expr;

public:
    UniaryOp(Token op, Expr *expr) : _op(op), _expr(expr) {}
    Token op() const { return _op; }
    Expr *expr() const { return _expr; }
};

class Var : public Expr
{
private:
    Token _token;
    string _value;
    string _scopeName;

public:
    Var(Token token, string value) : _token(token), _value(value) {}
    Token token() const { return _token; }
    string &value() { return _value; }
    void setScopeName(const string &scopeName)
    {
        _scopeName = scopeName;
    }
    string &scopeName() { return _scopeName; }
};

class Condition : public AST
{
private:
    Expr *_left;
    Token _op;
    Expr *_right;

public:
    Condition(Expr *left, Token token, Expr *right) : _left(left), _right(right), _op(token) {}
    Expr *left() const { return _left; }
    Expr *right() const { return _right; }
    Token op() const { return _op; }
};

class Statement : public AST
{
public:
    virtual ~Statement() {};
};

class ProcedureCallStatement : public Statement
{
private:
    Token _token;
    string _procedureName;
    vector<Expr *> _actualParams;
    ProcedureSymbol *_procedureSymbol;

public:
    ProcedureCallStatement(string procedureName, vector<Expr *> actualParams, Token token) : _procedureName(procedureName), _actualParams(actualParams), _token(token), _procedureSymbol(NULL) {}
    string &procedureName() { return _procedureName; }
    const vector<Expr *> &actualParams() const { return _actualParams; }
    Token token() const { return _token; }
    void setProcedureSymbol(ProcedureSymbol *procedureSymbol) { _procedureSymbol = procedureSymbol; }
    ProcedureSymbol *procedureSymbol() const { return _procedureSymbol; }
};

class CompoundStatement : public Statement
{
private:
    vector<Statement *> _statements; // array of statement
public:
    CompoundStatement(vector<Statement *> statements) : _statements(statements) {}
    const vector<Statement *> &statements() const { return _statements; }
    // void setChildrens(vector<Statement *> children) { _childrens = children; }
    // void addChild(Statement *child) { _childrens.push_back(child); }
};

class AssignmentStatement : public Statement
{
private:
    Var *_left;
    Token _op;
    Expr *_right;

public:
    AssignmentStatement(Var *left, Token op, Expr *right) : _left(left), _op(op), _right(right) {}
    Var *left() const { return _left; }
    Token op() const { return _op; }
    Expr *right() const { return _right; }
};

class NoOp : public Statement
{
};

class IfelseStatement : public Statement
{
private:
    Condition *_condition;
    vector<Statement *> _thenBranch;
    vector<Statement *> _elseBranch;

public:
    IfelseStatement(Condition *condition, const vector<Statement *> &thenBranch, const vector<Statement *> &elseBranch) : _condition(condition), _thenBranch(thenBranch), _elseBranch(elseBranch) {}

    const Condition *condition() const { return _condition; }
    const vector<Statement *> &thenBranch() const { return _thenBranch; }
    const vector<Statement *> &elseBranch() const { return _elseBranch; }
};

class Type : public AST
{
private:
    Token _token;
    string _value;

public:
    Type(Token token, string value) : _token(token), _value(value) {}
    Token token() const { return _token; }
    string value() const { return _value; }
};

class Declarations : public AST
{
public:
    virtual ~Declarations() {};
};

class VarDecl : public Declarations
{
private:
    Var *_var;
    Type *_type;

public:
    VarDecl(Var *var, Type *type) : _var(var), _type(type) {};
    Var *var() const { return _var; }
    Type *type() const { return _type; }
};

class Block : public AST
{
private:
    // the array can have VarDecl and ProcedureDecl
    vector<Declarations *> _declarations; // array of VarDecl and ProcedureDecl
    CompoundStatement *_compoundStatement;

public:
    Block(vector<Declarations *> declarations, CompoundStatement *compoundStatement) : _declarations(declarations), _compoundStatement(compoundStatement) {}
    const vector<Declarations *> &declarations() const { return _declarations; }
    CompoundStatement *compoundStatement() const { return _compoundStatement; }
};

class Param : public AST // is same as VarDecl but just a separate Node
{
private:
    Var *_var;
    Type *_type;

public:
    Param(Var *var, Type *type) : _var(var), _type(type) {};
    Var *var() const { return _var; }
    Type *type() const { return _type; }
};

class ProcedureDecl : public Declarations
{
private:
    string _procedureName;
    vector<Param *> _params;
    Block *_block;

public:
    ProcedureDecl(string procedureName, vector<Param *> params, Block *block) : _procedureName(procedureName), _params(params), _block(block) {}
    string procedureName() const { return _procedureName; }
    Block *block() const { return _block; }
    const vector<Param *> params() const { return _params; }
};

class Program : public AST
{
    string _name;
    Block *_block;

public:
    Program(string name, Block *block) : _name(name), _block(block) {};
    string name() const { return _name; }
    Block *block() const { return _block; }
};

#endif // AST_H