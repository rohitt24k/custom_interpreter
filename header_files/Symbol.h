#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <vector>
using namespace std;

class Block;

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
    string _scopeName;

public:
    VarSymbol(string name, Symbol *type, const string &scopeName) : Symbol(name, type), _scopeName(scopeName) {}

    string &scopeName() { return _scopeName; }

    string toString() const override
    {
        return "<VarSymbol(name='" + this->name() + "', type='" + this->type()->name() + "')>";
    }
};

class ProcedureSymbol : public Symbol
{
private:
    vector<Symbol *> _formalParams;
    Block *_blockAst;

public:
    string toString() const override
    {
        string str;
        str = "<ProcedureSymbol(name='" + this->name() + "' params=[";
        for (auto params : this->formalParams())
        {
            str += params->toString();
        }
        str += "])";
        return str;
    }
    ProcedureSymbol(const string &name) : Symbol(name), _blockAst(NULL) {}

    void insertParams(Symbol *varSymbol) { _formalParams.push_back(varSymbol); }
    vector<Symbol *> formalParams() const { return _formalParams; }
    void setBlockAst(Block *blockAst) { _blockAst = blockAst; }
    Block *blockAst() const { return _blockAst; }
};

class BuiltInProcedureSymbol : public ProcedureSymbol
{ // for now the only builtin symbol is writeln so no need of block or params
public:
    string toString() const override
    {
        string str;
        str = "<BuiltInProcedureSymbol(name='" + this->name() + "')";
        return str;
    }
    BuiltInProcedureSymbol(const string &name) : ProcedureSymbol(name) {}
};

class UserDefinedProcedureSymbol : public ProcedureSymbol
{
public:
    string toString() const override
    {
        string str;
        str = "<UserDefinedProcedureSymbol(name='" + this->name() + "' params=[";
        for (auto params : this->formalParams())
        {
            str += params->toString();
        }
        str += "])";
        return str;
    }
    UserDefinedProcedureSymbol(const string &name) : ProcedureSymbol(name) {}
};

#endif // SYMBOL_H