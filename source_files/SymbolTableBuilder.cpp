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
    Symbol *symbol = _symboltable.lookup(node->type()->value());

    Symbol *variableSymbol = new VarSymbol(node->var()->value(), symbol);

    if (_symboltable.lookup(node->var()->value()) != NULL)
    {
        string errorMessage =
            "Semantic Error: Variable '" + node->var()->value() +
            "' has already been declared. Redeclaration found at line " +
            to_string(node->var()->token().line()) +
            ", column " + to_string(node->var()->token().column()) +
            ". Ensure each variable is declared only once in the current scope.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->var()->token().line(), node->var()->token().column());
    }

    _symboltable.define(variableSymbol);
}

void SymbolTableBuilder::_visitProcedureDecl(ProcedureDecl *node)
{
    return;
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
    Symbol *symbol = _symboltable.lookup(node->left()->value());
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
    Symbol *symbol = _symboltable.lookup(node->value());
    if (symbol == NULL)
    {
        string errorMessage = "Use of undeclared variable '" + node->value() +
                              "' at line " + to_string(node->token().line()) +
                              ", column " + to_string(node->token().column()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
    }
}