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

Symbol *SymbolTable::lookup(string name)
{
    if (_symbols.find(name) == _symbols.end())
        return NULL;
    return _symbols[name];
}

void SymbolTable::printSymbolTable()
{
    for (auto row : _symbols)
    {
        string type;
        if (row.second->type() != NULL)
        {
            type = row.second->type()->name();
            cout << "[ " << row.first << " : " << type << " ]" << endl;
        }
        else
        {
            cout << row.first << endl;
        }
    }
}