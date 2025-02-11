#include "../header_files/SymbolTableBuilder.h"

void SymbolTableBuilder::_visitProgram(Program *node)
{
    SymbolTable *globalScope = new SymbolTable("global", 1, NULL);
    if (_logSymbolTable)
        cout << "ENTER scope: global" << endl;

    _currentScope = globalScope;
    _currentScope->initBuiltIns();

    visit(node->block());

    if (_logSymbolTable)
    {

        cout << endl;
        printSymbolTable();

        cout << "LEAVE scope: global" << endl;
    }

    _currentScope = globalScope->enclosingScope;
}

void SymbolTableBuilder::_visitBlock(Block *node)
{
    for (auto declaration : node->declarations())
        visit(declaration);

    visit(node->compoundStatement());
}

void SymbolTableBuilder::_visitVarDecl(VarDecl *node)
{
    Symbol *symbol = _currentScope->lookup(node->type()->value());

    Symbol *variableSymbol = new VarSymbol(node->var()->value(), symbol, _currentScope->scopeName());

    if (_currentScope->lookup(node->var()->value(), 1) != NULL)
    {
        string errorMessage =
            "Semantic Error: Variable '" + node->var()->value() +
            "' has already been declared. Redeclaration found at line " +
            to_string(node->var()->token().line()) +
            ", column " + to_string(node->var()->token().column()) +
            ". Ensure each variable is declared only once in the current scope.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->var()->token().line(), node->var()->token().column());
    }

    _currentScope->define(variableSymbol);
}

void SymbolTableBuilder::_visitProcedureDecl(ProcedureDecl *node)
{
    const string procedureName = node->procedureName();
    ProcedureSymbol *procedureSymbol = new UserDefinedProcedureSymbol(procedureName);

    _currentScope->define(procedureSymbol);

    procedureSymbol->setBlockAst(node->block());

    if (_logSymbolTable)
        cout << "ENTER scope: " << procedureName << endl;

    SymbolTable *procedureSymbolTable = new SymbolTable(procedureName, _currentScope->scopeLevel() + 1, _currentScope);

    _currentScope = procedureSymbolTable;

    for (auto param : node->params())
    {
        Symbol *symbol = _currentScope->lookup(param->type()->value());
        Symbol *variableSymbol = new VarSymbol(param->var()->value(), symbol, _currentScope->scopeName());
        _currentScope->define(variableSymbol);
        procedureSymbol->insertParams(variableSymbol);
    }

    visit(node->block());

    if (_logSymbolTable)
    {
        cout << endl;
        printSymbolTable();
        cout << "Leave scope: " << procedureName << endl;
    }

    _currentScope = _currentScope->enclosingScope;

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
    Symbol *symbol = _currentScope->lookup(node->left()->value());

    if (symbol == nullptr)
    {
        string errorMessage = "Assignment to an undeclared variable '" + node->left()->value() +
                              "' at line " + to_string(node->left()->token().line()) +
                              ", column " + to_string(node->left()->token().column()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->left()->token().line(), node->left()->token().column());
    }

    VarSymbol *varSymbol = dynamic_cast<VarSymbol *>(symbol);

    if (varSymbol == nullptr)
    {
        string errorMessage = "Cannot assign to '" + node->left()->value() + "' because it is not a variable.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->left()->token().line(), node->left()->token().column());
    }
    node->left()->setScopeName(varSymbol->scopeName());

    visit(node->right());
}

void SymbolTableBuilder::_visitProcedureCallStatement(ProcedureCallStatement *node)
{
    Symbol *symbol = _currentScope->lookup(node->procedureName());

    if (symbol == NULL)
    {
        string errorMessage = "Procedure '" + node->procedureName() +
                              "' is not declared before usage.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                               node->token().line(), node->token().column());
    }

    if (UserDefinedProcedureSymbol *procedureSymbol = dynamic_cast<UserDefinedProcedureSymbol *>(symbol))
    {
        node->setProcedureSymbol(procedureSymbol);

        if (procedureSymbol->formalParams().size() != node->actualParams().size())
        {
            string errorMessage = "Procedure '" + node->procedureName() +
                                  "' expects " + to_string(procedureSymbol->formalParams().size()) +
                                  " arguments, but " + to_string(node->actualParams().size()) +
                                  " were provided.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                                   node->token().line(), node->token().column());
        }
    } // skip the formalparams and actualparams size checking for the builtinfunction for now

    for (auto params : node->actualParams())
        visit(params);
}

void SymbolTableBuilder::_visitIfelseStatement(IfelseStatement *node)
{

    for (auto &statement : node->thenBranch())
    {
        visit(statement);
    }
    for (auto &statement : node->elseBranch())
    {
        visit(statement);
    }

    return;
}

void SymbolTableBuilder::_visitVar(Var *node)
{
    Symbol *symbol = _currentScope->lookup(node->value());
    if (symbol == NULL)
    {
        string errorMessage = "Use of undeclared variable '" + node->value() +
                              "' at line " + to_string(node->token().line()) +
                              ", column " + to_string(node->token().column()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
    }
}