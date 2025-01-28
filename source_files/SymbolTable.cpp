#include "../header_files/SymbolTable.h"

void SymbolTable::_initBuiltIns()
{
    define(new BuiltInTypeSymbol("INTEGER"));
    define(new BuiltInTypeSymbol("REAL"));
}
