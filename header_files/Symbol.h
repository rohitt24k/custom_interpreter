#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
using namespace std;

class Symbol
{
private:
    string _name;
    Symbol *_type;

public:
    Symbol(string name) : _name(name), _type(NULL) {}
    Symbol(string name, Symbol *type) : _name(name), _type(type) {}
    string name() const { return _name; }
    Symbol *type() const { return _type; }
};

class BuiltInTypeSymbol : public Symbol
{
public:
    BuiltInTypeSymbol(string name) : Symbol(name) {}
};

class VarSymbol : public Symbol
{
public:
    VarSymbol(string name, Symbol *type) : Symbol(name, type) {}
};

#endif // SYMBOL_H