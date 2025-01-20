#ifndef DRAWING_H
#define DRAWING_H

#include "Parser.h"
#include "NodeVisitor.h"
#include <vector>

class Node
{
public:
    string name;
    vector<Node *> children;

    Node(const string &name) : name(name) {}
};

class DrawingInterpreter : public NodeVisitor<Node *>
{
private:
    Parser parser;

    Node *_visitNum(Num *node);
    Node *_visitBinOp(BinOp *node);
    Node *_visitUniaryOp(UniaryOp *node);
    Node *_visitVar(Var *node);

    Node *_visitProgram(Program *node);

    Node *_visitBlock(Block *node);

    Node *_visitCompoundStatement(CompoundStatement *node);
    Node *_visitAssignStatement(AssignmentStatement *node);
    Node *_visitNoOP(NoOp *node);

    Node *_visitVarDecl(VarDecl *node);

    Node *_visitType(Type *node);

    void printHierarchy(Node *node, string prefix);

public:
    DrawingInterpreter(Parser parser) : parser(parser) {}

    void interpret()
    {
        AST *tree = parser.parse();
        printHierarchy(visit(tree), "");
    }
};

#endif // DRAWING_H