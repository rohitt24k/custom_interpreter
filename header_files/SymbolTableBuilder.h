#ifndef SYMBOLTABLEBUILDER_H
#define SYMBOLTABLEBUILDER_H

#include "Parser.h"
#include "NodeVisitor.h"
#include "SymbolTable.h"

class SymbolTableBuilder : public NodeVisitor<void>
{

private:
    SymbolTable *_currentScope;
    AST *_tree;

    void _visitNum(Num *node)
    {
        return;
    };
    void _visitBinOp(BinOp *node);
    void _visitUniaryOp(UniaryOp *node);
    void _visitVar(Var *node);

    void _visitProgram(Program *node);

    void _visitBlock(Block *node);

    void _visitCompoundStatement(CompoundStatement *node);
    void _visitAssignStatement(AssignmentStatement *node);
    void _visitProcedureCallStatement(ProcedureCallStatement *node);
    void _visitNoOP(NoOp *node)
    {
        return;
    };

    void _visitProcedureDecl(ProcedureDecl *node);
    void _visitVarDecl(VarDecl *node);

    void _visitType(Type *node)
    {
        return;
    };

    int _logSymbolTable = 0;

public:
    SymbolTableBuilder(AST *tree, int log = 0) : NodeVisitor(log), _tree(tree), _currentScope(NULL), _logSymbolTable(log) {}

    void buildSymbolTable()
    {
        visit(_tree);
        return;
    };

    void printSymbolTable()
    {
        cout << *_currentScope;
    }
};

#endif // SYMBOLTABLEBUILDER_H