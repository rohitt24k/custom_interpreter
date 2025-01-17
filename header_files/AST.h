#ifndef AST_H
#define AST_H

#include "Token.h"

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

#endif // AST_H