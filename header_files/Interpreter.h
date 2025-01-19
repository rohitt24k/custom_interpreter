#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "NodeVisitor.h"

class Interpreter : public NodeVisitor<int>
{
private:
    Parser _parser;

    int _visitNum(Num *node);
    int _visitBinOp(BinOp *node);
    int _visitUniaryOp(UniaryOp *node);
    int _visitVar(Var *node);

    int _visitProgram(Program *node);

    int _visitCompoundStatement(CompoundStatement *node);
    int _visitAssignStatement(AssignmentStatement *node);
    int _visitNoOP(NoOp *node);

    unordered_map<string, int> _GLOBAL_SCOPE;

public:
    Interpreter(Parser parser) : _parser(parser) {}
    int interpret();
    void printGlobalScope()
    {
        for (auto row : _GLOBAL_SCOPE)
        {
            cout << row.first << " : " << row.second << endl;
        }
    }
};

#endif // INTERPRETER_H