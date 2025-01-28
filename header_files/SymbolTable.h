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
    void _initBuiltIns();

public:
    void initBuiltIns();

    SymbolTable()
    {
        initBuiltIns();
    }

    void define(Symbol *symbol);

    Symbol *lookup(string name);

    void printSymbolTable();
};

#endif