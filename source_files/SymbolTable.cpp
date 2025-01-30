#include "../header_files/SymbolTable.h"

void SymbolTable::initBuiltIns()
{
    define(new BuiltInTypeSymbol("INTEGER"));
    define(new BuiltInTypeSymbol("REAL"));
}

void SymbolTable::define(Symbol *symbol)
{
    _symbols.insert({symbol->name(), symbol});
}

Symbol *SymbolTable::lookup(string name, int currentScopeOnly)
{
    if (_symbols.find(name) != _symbols.end())
        return _symbols[name];

    if (!currentScopeOnly)
        if (enclosingScope != NULL)
            return enclosingScope->lookup(name);

    return NULL;
}