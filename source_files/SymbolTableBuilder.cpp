#include "../header_files/SymbolTableBuilder.h"

void SymbolTableBuilder::_visitProgram(Program *node)
{
    visit(node->block());
}

void SymbolTableBuilder::_visitBlock(Block *node)
{
    for (auto declaration : node->declarations())
        visit(declaration);

    visit(node->compoundStatement());
}

void SymbolTableBuilder::_visitVarDecl(VarDecl *node)
{
    _symboltable->define(new VarSymbol(node->var()->value(), _symboltable->lookup(node->type()->value())));
}

void SymbolTableBuilder::_visitBinOp(BinOp *node)
{
    visit(node->left());
    visit(node->right());
}

void SymbolTableBuilder::_visitUniaryOp(UniaryOp *node)
{
    visit(node->expr());
}

void SymbolTableBuilder::_visitCompoundStatement(CompoundStatement *node)
{
    for (auto statement : node->statements())
        visit(statement);
}

void SymbolTableBuilder::_visitAssignStatement(AssignmentStatement *node)
{
    Symbol *symbol = _symboltable->lookup(node->left()->value());
    if (symbol == NULL)
    {
        string errorMessage = "Assignment to an undeclared variable '" + node->left()->value() +
                              "' at line " + to_string(node->left()->token().line()) +
                              ", column " + to_string(node->left()->token().column()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->left()->token().line(), node->left()->token().column());
    }

    visit(node->right());
}

void SymbolTableBuilder::_visitVar(Var *node)
{
    Symbol *symbol = _symboltable->lookup(node->value());
    if (symbol == NULL)
    {
        string errorMessage = "Use of undeclared variable '" + node->value() +
                              "' at line " + to_string(node->token().line()) +
                              ", column " + to_string(node->token().column()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
    }
}