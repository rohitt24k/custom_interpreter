#ifndef NODEVISITOR_H
#define NODEVISITOR_H

#include "AST.h"
#include "Error.h"

template <typename T>
class NodeVisitor
{
public:
    T visit(AST *node)
    {
        if (Num *numNode = dynamic_cast<Num *>(node))
        {
            return _visitNum(numNode);
        }
        if (UniaryOp *UniaryOpNode = dynamic_cast<UniaryOp *>(node))
        {
            return _visitUniaryOp(UniaryOpNode);
        }
        if (BinOp *binOpNode = dynamic_cast<BinOp *>(node))
        {
            return _visitBinOp(binOpNode);
        }
        if (CompoundStatement *compoundNode = dynamic_cast<CompoundStatement *>(node))
        {
            return _visitCompoundStatement(compoundNode);
        }
        if (AssignmentStatement *assignmentNode = dynamic_cast<AssignmentStatement *>(node))
        {
            return _visitAssignStatement(assignmentNode);
        }
        if (NoOp *noOpNode = dynamic_cast<NoOp *>(node))
        {
            return _visitNoOP(noOpNode);
        }
        if (Var *varNode = dynamic_cast<Var *>(node))
        {
            return _visitVar(varNode);
        }
        if (Program *programNode = dynamic_cast<Program *>(node))
        {
            return _visitProgram(programNode);
        }
        if (Block *blockNode = dynamic_cast<Block *>(node))
        {
            return _visitBlock(blockNode);
        }
        if (VarDecl *varDeclNode = dynamic_cast<VarDecl *>(node))
        {
            return _visitVarDecl(varDeclNode);
        }
        if (Type *typeNode = dynamic_cast<Type *>(node))
        {
            return _visitType(typeNode);
        }
        if (ProcedureDecl *procedureDeclNode = dynamic_cast<ProcedureDecl *>(node))
        {
            return _visitProcedureDecl(procedureDeclNode);
        }
        string errorMessage = "Error in interpreting the tree";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, 0, 0);
    }

private:
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
    virtual T _visitNoOP(NoOp *node) = 0;
    virtual T _visitVar(Var *node) = 0;
};

#endif // NODEVISITOR_H