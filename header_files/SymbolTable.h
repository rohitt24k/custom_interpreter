#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include "Symbol.h"
using namespace std;

class SymbolTable
{
private:
    unordered_map<string, Symbol *> _symbols;
    void _initBuiltIns();

public:
    SymbolTable() { _initBuiltIns(); }
    void define(Symbol *symbol) { _symbols.insert({symbol->name(), symbol}); }
    Symbol *lookup(string name)
    {
        if (_symbols.find(name) == _symbols.end())
            return NULL;
        return _symbols[name];
    }
};

#endif