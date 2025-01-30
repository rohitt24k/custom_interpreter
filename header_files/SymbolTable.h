#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include <unordered_map>
#include "Symbol.h"

using namespace std;

class SymbolTable
{
private:
    unordered_map<string, Symbol *> _symbols;
    string _scopeName;
    int _scopeLevel;
    void _initBuiltIns();

public:
    SymbolTable *enclosingScope;
    void initBuiltIns();

    SymbolTable(string scopeName, int scopeLevel, SymbolTable *enclosingScope) : _scopeName(scopeName), _scopeLevel(scopeLevel), enclosingScope(enclosingScope) {}

    void define(Symbol *symbol);
    Symbol *lookup(string name, int currentScopeOnly = 0);

    const string scopeName() const { return _scopeName; }
    const int scopeLevel() const { return _scopeLevel; }

    friend ostream &operator<<(ostream &os, const SymbolTable &symbolTable)
    {
        os << "SYMBOL TABLE" << endl;

        os << "Scope Name \t: " << symbolTable._scopeName << endl;
        os << "Scope Level \t: " << symbolTable._scopeLevel << endl;
        if (symbolTable.enclosingScope != NULL)
            os << "Enclosing Scope : " << symbolTable.enclosingScope->_scopeName << endl;

        os << "-----------------------------------------------" << endl;

        for (auto row : symbolTable._symbols)
        {
            os << row.first << " : " << *row.second << endl;
        }
        return os;
    }
};

#endif