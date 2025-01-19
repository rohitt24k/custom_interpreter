#ifndef AST_H
#define AST_H

#include "Token.h"
#include "vector"

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

public:
    Var(Token token, string value) : _token(token), _value(value) {}
    Token token() const { return _token; }
    string value() const { return _value; }
};

class Statement : public AST
{
public:
    virtual ~Statement() {};
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

class Program : public AST
{
    // string _name;
    CompoundStatement *_compoundStatement;

public:
    Program(CompoundStatement *compoundStatement) : _compoundStatement(compoundStatement) {};
    // string name() const { return _name; }
    Statement *compoundStatement() const { return _compoundStatement; }
};

#endif // AST_H