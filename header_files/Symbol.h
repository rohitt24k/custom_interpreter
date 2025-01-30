#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>
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

    virtual string toString() const
    {
        return "<Symbol(name='" + _name + "')>";
    }

    friend ostream &operator<<(ostream &os, const Symbol &symbol)
    {
        os << symbol.toString();
        return os;
    }

    virtual ~Symbol() = default;
};

class BuiltInTypeSymbol : public Symbol
{
public:
    BuiltInTypeSymbol(string name) : Symbol(name) {}

    string toString() const override
    {
        return "<BuiltInTypeSymbol(name='" + this->name() + "')>";
    }
};

class VarSymbol : public Symbol
{
public:
    VarSymbol(string name, Symbol *type) : Symbol(name, type) {}

    string toString() const override
    {
        return "<VarSymbol(name='" + this->name() + "', type='" + this->type()->name() + "')>";
    }
};

class ProcedureSymbol : public Symbol
{
private:
    vector<Symbol *> _params;
    string toString() const override
    {
        string str;
        str = "<ProcedureSymbol(name='" + this->name() + "' params=[";
        for (auto params : this->params())
        {
            str += params->toString();
        }
        str += "])";
        return str;
    }

public:
    ProcedureSymbol(const string &name) : Symbol(name) {}

    void insertParams(Symbol *varSymbol) { _params.push_back(varSymbol); }
    const vector<Symbol *> &params() const { return _params; }
};

#endif // SYMBOL_H