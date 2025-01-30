#include "../header_files/Drawing.h"

void DrawingInterpreter::printHierarchy(Node *node, string prefix)
{
    prefix = prefix + "|";
    cout << prefix << "-->" << node->name << endl;
    string childPrefix = prefix + "    "; // Increase indentation for children

    for (size_t i = 0; i < node->children.size(); ++i)
    {
        printHierarchy(node->children[i], childPrefix);
    }
}

Node *DrawingInterpreter::_visitBinOp(BinOp *node)
{
    Node *newNode = new Node("Binary Operator (" + node->op().value() + ")");

    Node *leftResult = visit(node->left());
    Node *rightResult = visit(node->right());

    newNode->children.push_back(leftResult);
    newNode->children.push_back(rightResult);

    return newNode;
}

Node *DrawingInterpreter::_visitNum(Num *node)
{
    Node *newNode = new Node(node->value());
    return newNode;
}

Node *DrawingInterpreter::_visitUniaryOp(UniaryOp *node)
{
    Node *newNode = new Node("Uniary Operator (" + node->op().value() + ")");

    newNode->children.push_back(visit(node->expr()));

    return newNode;
}

Node *DrawingInterpreter::_visitVar(Var *node)
{
    return new Node(node->value());
}

Node *DrawingInterpreter::_visitProgram(Program *node)
{
    Node *newNode = new Node("Program");
    newNode->children.push_back(visit(node->block()));
    return newNode;
}

Node *DrawingInterpreter::_visitBlock(Block *node)
{
    Node *newNode = new Node("Block");
    for (auto declaration : node->declarations())
    {
        newNode->children.push_back(visit(declaration));
    }
    newNode->children.push_back(visit(node->compoundStatement()));
    return newNode;
}

Node *DrawingInterpreter::_visitCompoundStatement(CompoundStatement *node)
{
    Node *newNode = new Node("Compound");
    for (auto statement : node->statements())
        newNode->children.push_back(visit(statement));
    return newNode;
}

Node *DrawingInterpreter::_visitAssignStatement(AssignmentStatement *node)
{
    Node *newNode = new Node(node->op().value());
    newNode->children.push_back(visit(node->left()));
    newNode->children.push_back(visit(node->right()));
    return newNode;
}

Node *DrawingInterpreter::_visitProcedureCallStatement(ProcedureCallStatement *node)
{
    Node *newNode = new Node("ProcedureCall:" + node->procedureName());
    for (auto params : node->actualParams())
    {
        newNode->children.push_back(visit(params));
    }
    return newNode;
}

Node *DrawingInterpreter::_visitNoOP(NoOp *node)
{
    return new Node("NoOp");
}

// Node *visit_Program(Program *node)
// {
//     Node *newNode = new Node("program");
//     newNode->children.push_back(visit(node->block));
//     return newNode;
// }

// Node *visit_Block(Block *node)
// {
//     Node *newNode = new Node("Block");
//     vector<Node *> v;
//     for (auto declaration : node->declarations)
//     {
//         v.push_back(visit(declaration));
//     }
//     if (v.size() > 0)
//     {
//         newNode->children = v;
//     }

//     newNode->children.push_back(visit(node->compoundStatement));
//     return newNode;
// }

Node *DrawingInterpreter::_visitVarDecl(VarDecl *node)
{
    Node *newNode = new Node("VarDecl");
    newNode->children.push_back(visit(node->var()));
    newNode->children.push_back(visit(node->type()));
    return newNode;
}

Node *DrawingInterpreter::_visitProcedureDecl(ProcedureDecl *node)
{
    Node *newNode = new Node("Procedure -> " + node->procedureName());
    newNode->children.push_back(visit(node->block()));
    return newNode;
}

Node *DrawingInterpreter::_visitType(Type *node)
{
    Node *newNode = new Node(node->value());
    return newNode;
}

// Node *visit_Compound(Compound *node)
// {
//     Node *newNode = new Node("Compound");
//     vector<Node *> v;
//     for (auto child : node->children)
//     {
//         v.push_back(visit(child));
//     }

//     newNode->children = v;
//     return newNode;
// }

// Node *visit_Assign(Assign *node)
// {
//     Node *newNode = new Node("Assignment Operator (" + node->op.getValue() + ")");
//     newNode->children.push_back(visit(node->left));
//     newNode->children.push_back(visit(node->right));
//     return newNode;
// }

// Node *visit_NoOP(NoOp *node)
// {
//     return new Node("NoOp");
// }

// // Visiting a variable (look it up in the symbol table)
// Node *visit_Var(Var *node)
// {
//     return new Node(node->value);
// }