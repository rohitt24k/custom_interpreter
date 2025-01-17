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

public:
    Interpreter(Parser parser) : _parser(parser) {}
    int interpret();
};

#endif // INTERPRETER_H