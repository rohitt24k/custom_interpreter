#ifndef SYMBOLTABLEBUILDER_H
#define SYMBOLTABLEBUILDER_H

#include "NodeVisitor.h"
#include "SymbolTable.h"

class SymbolTableBuilder : public NodeVisitor<void>
{

private:
    SymbolTable *_symboltable;
    void _visitProgram(Program *node);
    void _visitBlock(Block *node);
    void _visitVarDecl(VarDecl *node);
    void _visitType(Type *node) = 0;
    void _visitBinOp(BinOp *node);
    void _visitNum(Num *node) = 0;
    void _visitUniaryOp(UniaryOp *node);
    void _visitCompoundStatement(CompoundStatement *node);
    void _visitAssignStatement(AssignmentStatement *node);
    void _visitNoOP(NoOp *node) = 0;
    void _visitVar(Var *node);

public:
    SymbolTableBuilder() {}
};

#endif // SYMBOLTABLEBUILDER_H