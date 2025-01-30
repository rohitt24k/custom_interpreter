#ifndef SYMBOLTABLEBUILDER_H
#define SYMBOLTABLEBUILDER_H

#include "Parser.h"
#include "NodeVisitor.h"
#include "SymbolTable.h"

class SymbolTableBuilder : public NodeVisitor<void>
{

private:
    SymbolTable *_currentScope;
    Parser _parser;

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

public:
    SymbolTableBuilder(Parser parser) : _parser(parser), _currentScope(NULL) {}

    void buildSymbolTable()
    {
        AST *tree = _parser.parse();
        visit(tree);
        return;
    };

    void printSymbolTable()
    {
        cout << *_currentScope;
    }
};

#endif // SYMBOLTABLEBUILDER_H