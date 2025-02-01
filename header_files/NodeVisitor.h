#ifndef NODEVISITOR_H
#define NODEVISITOR_H

#include "AST.h"
#include "Error.h"

template <typename T>
class NodeVisitor
{
public:
    NodeVisitor(int log = 0) : _log(log) {};

    T visit(AST *node)
    {
        if (Num *numNode = dynamic_cast<Num *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitNum \n";
            return _visitNum(numNode);
        }
        if (UniaryOp *UniaryOpNode = dynamic_cast<UniaryOp *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitUniaryOp \n";
            return _visitUniaryOp(UniaryOpNode);
        }
        if (BinOp *binOpNode = dynamic_cast<BinOp *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitBinOp \n";
            return _visitBinOp(binOpNode);
        }
        if (CompoundStatement *compoundNode = dynamic_cast<CompoundStatement *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitCompoundStatement \n";
            return _visitCompoundStatement(compoundNode);
        }
        if (AssignmentStatement *assignmentNode = dynamic_cast<AssignmentStatement *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitAssignStatement \n";
            return _visitAssignStatement(assignmentNode);
        }
        if (ProcedureCallStatement *procedureCallNode = dynamic_cast<ProcedureCallStatement *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitProcedureCallStatement \n";
            return _visitProcedureCallStatement(procedureCallNode);
        }
        if (NoOp *noOpNode = dynamic_cast<NoOp *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitNoOP \n";
            return _visitNoOP(noOpNode);
        }
        if (Var *varNode = dynamic_cast<Var *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitVar \n";
            return _visitVar(varNode);
        }
        if (Program *programNode = dynamic_cast<Program *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitProgram \n";
            return _visitProgram(programNode);
        }
        if (Block *blockNode = dynamic_cast<Block *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitBlock \n";
            return _visitBlock(blockNode);
        }
        if (VarDecl *varDeclNode = dynamic_cast<VarDecl *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitVarDecl \n";
            return _visitVarDecl(varDeclNode);
        }
        if (Type *typeNode = dynamic_cast<Type *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitType \n";
            return _visitType(typeNode);
        }
        if (ProcedureDecl *procedureDeclNode = dynamic_cast<ProcedureDecl *>(node))
        {
            if (_log == 1)
                cout << "nodevisitor inside -> _visitProcedureDecl \n";
            return _visitProcedureDecl(procedureDeclNode);
        }
        string errorMessage = "Error in interpreting the tree";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, 0, 0);
    }

private:
    int _log = 0;
    virtual T _visitProgram(Program *node) = 0;
    virtual T _visitBlock(Block *node) = 0;
    virtual T _visitVarDecl(VarDecl *node) = 0;
    virtual T _visitProcedureDecl(ProcedureDecl *node) = 0;
    virtual T _visitType(Type *node) = 0;
    virtual T _visitBinOp(BinOp *node) = 0;
    virtual T _visitNum(Num *node) = 0;
    virtual T _visitUniaryOp(UniaryOp *node) = 0;
    virtual T _visitCompoundStatement(CompoundStatement *node) = 0;
    virtual T _visitAssignStatement(AssignmentStatement *node) = 0;
    virtual T _visitProcedureCallStatement(ProcedureCallStatement *node) = 0;
    virtual T _visitNoOP(NoOp *node) = 0;
    virtual T _visitVar(Var *node) = 0;
};

#endif // NODEVISITOR_H