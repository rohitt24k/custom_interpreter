#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "NodeVisitor.h"
#include "CallStack.h"
#include <variant>

class Interpreter : public NodeVisitor<nodeVisitorResult>
{
private:
    AST *_tree;

    nodeVisitorResult _visitNum(Num *node);
    nodeVisitorResult _visitStringLiteral(StringLiteral *node);
    nodeVisitorResult _visitBinOp(BinOp *node);
    nodeVisitorResult _visitUniaryOp(UniaryOp *node);
    nodeVisitorResult _visitVar(Var *node);

    nodeVisitorResult _visitProgram(Program *node);

    nodeVisitorResult _visitBlock(Block *node);

    nodeVisitorResult _visitCompoundStatement(CompoundStatement *node);
    nodeVisitorResult _visitAssignStatement(AssignmentStatement *node);
    nodeVisitorResult _visitProcedureCallStatement(ProcedureCallStatement *node);

    nodeVisitorResult _visitNoOP(NoOp *node);

    nodeVisitorResult _visitProcedureDecl(ProcedureDecl *node);
    nodeVisitorResult _visitVarDecl(VarDecl *node);

    nodeVisitorResult _visitType(Type *node);

    // unordered_map<string, nodeVisitorResult> _GLOBAL_SCOPE;
    CallStack _callStack;
    short _logInterpreter = 0;

public:
    Interpreter(AST *tree, int log = 0) : NodeVisitor(log), _tree(tree), _logInterpreter(log) {}
    nodeVisitorResult interpret();
    // void printGlobalScope()
    // {
    //     // cout << _GLOBAL_SCOPE.size() << endl;
    //     for (auto row : _GLOBAL_SCOPE)
    //     {
    //         std::visit([&](const auto &second)
    //                    { cout << row.first << " : " << second << endl; }, row.second);
    //     }
    // }

    void printAR()
    {
        _callStack.printAR();
    }
};

#endif // INTERPRETER_H