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
        // // if (Compound *compundNode = dynamic_cast<Compound *>(node))
        // // {
        // //     return visit_Compound(compundNode);
        // // }
        // if (Assign *assignNode = dynamic_cast<Assign *>(node))
        // {
        //     return visit_Assign(assignNode);
        // }
        // if (NoOp *noOpNode = dynamic_cast<NoOp *>(node))
        // {
        //     return visit_NoOP(noOpNode);
        // }
        // if (Var *varNode = dynamic_cast<Var *>(node))
        // {
        //     return visit_Var(varNode);
        // }
        // if (Program *programNode = dynamic_cast<Program *>(node))
        // {
        //     return visit_Program(programNode);
        // }
        // if (VarDecl *varDeclkNode = dynamic_cast<VarDecl *>(node))
        // {
        //     return visit_VarDecl(varDeclkNode);
        // }
        // if (Block *blockNode = dynamic_cast<Block *>(node))
        // {
        //     return visit_Block(blockNode);
        // }
        // if (Type *typeNode = dynamic_cast<Type *>(node))
        // {
        //     return visit_Type(typeNode);
        // }
        // if (ProcedureDecl *procedureDeclNode = dynamic_cast<ProcedureDecl *>(node))
        // {
        //     return visit_ProcedureDecl(procedureDeclNode);
        // }

        string errorMessage = "Error in interpreting the tree";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, 0, 0);
    }

private:
    // virtual T visit_Program(Program *node) = 0;
    // virtual T visit_Block(Block *node) = 0;
    // virtual T visit_VarDecl(VarDecl *node) = 0;
    // virtual T visit_ProcedureDecl(ProcedureDecl *node) = 0;
    // virtual T visit_Type(Type *node) = 0;
    virtual T _visitBinOp(BinOp *node) = 0;
    virtual T _visitNum(Num *node) = 0;
    virtual T _visitUniaryOp(UniaryOp *node) = 0;
    // virtual T visit_Compound(Compound *node) = 0;
    // virtual T visit_Assign(Assign *node) = 0;
    // virtual T visit_NoOP(NoOp *node) = 0;
    // virtual T visit_Var(Var *node) = 0;
};

#endif // NODEVISITOR_H