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
public:
    Expr *left;
    Token op;
    Expr *right;
    BinOp(Expr *left, Token token, Expr *right) : left(left), right(right), op(token) {}
};

#endif // AST_H