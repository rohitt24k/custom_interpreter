#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "NodeVisitor.h"
#include <variant>

using nodeVisitorResult = variant<int, double>;

class Interpreter : public NodeVisitor<nodeVisitorResult>
{
private:
    Parser _parser;

    nodeVisitorResult _visitNum(Num *node);
    nodeVisitorResult _visitBinOp(BinOp *node);
    nodeVisitorResult _visitUniaryOp(UniaryOp *node);
    nodeVisitorResult _visitVar(Var *node);

    nodeVisitorResult _visitProgram(Program *node);

    nodeVisitorResult _visitBlock(Block *node);

    nodeVisitorResult _visitCompoundStatement(CompoundStatement *node);
    nodeVisitorResult _visitAssignStatement(AssignmentStatement *node);
    nodeVisitorResult _visitNoOP(NoOp *node);

    nodeVisitorResult _visitProcedureDecl(ProcedureDecl *node);
    nodeVisitorResult _visitVarDecl(VarDecl *node);

    nodeVisitorResult _visitType(Type *node);

    unordered_map<string, nodeVisitorResult> _GLOBAL_SCOPE;

public:
    Interpreter(Parser parser) : _parser(parser) {}
    nodeVisitorResult interpret();
    void printGlobalScope()
    {
        // cout << _GLOBAL_SCOPE.size() << endl;
        for (auto row : _GLOBAL_SCOPE)
        {
            std::visit([&](const auto &second)
                       { cout << row.first << " : " << second << endl; }, row.second);
        }
    }
};

#endif // INTERPRETER_H