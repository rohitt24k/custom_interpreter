#include "../header_files/Interpreter.h"

nodeVisitorResult Interpreter::_visitNum(Num *node)
{
    if (node->token().type() == Token::TokenType::INTEGER_CONST)
    {
        try
        {
            // Convert the value to an integer
            return stoi(node->value());
        }
        catch (const std::invalid_argument &e)
        {
            // Handle invalid integer value
            string errorMessage = "Invalid integer value: '" + node->value() + "'.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
        }
        catch (const std::out_of_range &e)
        {
            // Handle integer value out of range
            string errorMessage = "Integer value out of range: '" + node->value() + "'.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
        }
    }
    else if (node->token().type() == Token::TokenType::REAL_CONST)
    {
        try
        {
            // Convert the value to a floating-point number
            return stod(node->value());
        }
        catch (const std::invalid_argument &e)
        {
            // Handle invalid floating-point value
            string errorMessage = "Invalid real value: '" + node->value() + "'.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
        }
        catch (const std::out_of_range &e)
        {
            // Handle floating-point value out of range
            string errorMessage = "Real value out of range: '" + node->value() + "'.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
        }
    }
    else
    {
        // Handle unexpected token type
        string errorMessage = "Expected a numeric constant but found token of type '" +
                              node->token().typeAsString() + "' with value '" +
                              node->value() + "'.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, node->token().line(), node->token().column());
    }
}

nodeVisitorResult Interpreter::_visitBinOp(BinOp *node)
{
    nodeVisitorResult leftResult = visit(node->left());
    nodeVisitorResult rightResult = visit(node->right());

    auto checkDivisonByZero = [&](const auto &rightValue)
    {
        if (rightValue == 0)
        {
            // Handle division by zero
            string errorMessage =
                "Division by zero encountered in binary operation at line " +
                to_string(node->op().line()) + ", column " +
                to_string(node->op().column()) + ".";
            Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->op().line(), node->op().column());
        }
    };

    auto result = std::visit(
        [&](const auto &leftValue, const auto &rightValue) -> nodeVisitorResult
        {
            if (node->op().type() == Token::TokenType::PLUS)
            {
                return leftValue + rightValue;
            }
            if (node->op().type() == Token::TokenType::SUBTRACT)
            {
                return leftValue - rightValue;
            }
            if (node->op().type() == Token::TokenType::MULTIPLICATION)
            {
                return leftValue * rightValue;
            }
            if (node->op().type() == Token::TokenType::INTEGER_DIV)
            {
                checkDivisonByZero(rightValue);
                return static_cast<int>(leftValue) / static_cast<int>(rightValue);
            }
            if (node->op().type() == Token::TokenType::FLOAT_DIV)
            {
                checkDivisonByZero(rightValue);
                // Perform floating-point division and return a double
                return static_cast<double>(leftValue) / static_cast<double>(rightValue);
            }
            string errorMessage =
                "Unknown binary operator '" + node->op().value() +
                "' encountered at line " + to_string(node->op().line()) +
                ", column " + to_string(node->op().column()) + ".";
            Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->op().line(), node->op().column());
        },
        leftResult, rightResult);
    return result;
}

nodeVisitorResult Interpreter::_visitUniaryOp(UniaryOp *node)
{
    int result = get<int>(visit(node->expr()));
    if (node->op().type() == Token::TokenType::PLUS)
    {
        return result;
    }
    else
    {
        return -1 * result;
    }
}

nodeVisitorResult Interpreter::_visitVar(Var *node)
{
    try
    {
        return _callStack.get(node->value());
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
        string errorMessage = "Variable '" + node->value() + "' is not defined.";
        Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->token().line(), node->token().column());
    }
}

nodeVisitorResult Interpreter::_visitProgram(Program *node)
{
    ActivationRecord *ar = new ActivationRecord("Program");
    _callStack.push(ar);
    nodeVisitorResult result = visit(node->block());
    _callStack.printAR();
    _callStack.pop();

    return result;
}

nodeVisitorResult Interpreter::_visitBlock(Block *node)
{
    for (auto declaration : node->declarations())
    {
        visit(declaration);
    }
    return visit(node->compoundStatement());
}

nodeVisitorResult Interpreter::_visitVarDecl(VarDecl *node)
{
    return 0; // do nothing fn
}

nodeVisitorResult Interpreter::_visitProcedureDecl(ProcedureDecl *node)
{
    // ActivationRecord *ar = new ActivationRecord(node->procedureName(), _callStack.top());
    // _callStack.push(ar);
    // return visit(node->block());

    // _callStack.printAR();
    // _callStack.pop();

    return 0;
}

nodeVisitorResult Interpreter::_visitType(Type *node)
{
    return 0; // do nothing fn
}

nodeVisitorResult Interpreter::_visitCompoundStatement(CompoundStatement *node)
{
    for (auto statement : node->statements())
    {
        visit(statement);
    }
    return 0;
}

nodeVisitorResult Interpreter::_visitAssignStatement(AssignmentStatement *node)
{
    try
    {
        _callStack.set(node->left()->value(), visit(node->right()));
        return 0;
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';

        string errorMessage = "Assignment failed for variable '" + node->left()->value() +
                              "'. Possible reasons: Variable is not declared, or the call stack is empty.";

        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                               node->left()->token().line(), node->left()->token().column());
    }

    return 0;
}

nodeVisitorResult Interpreter::_visitProcedureCallStatement(ProcedureCallStatement *node)
{
    ActivationRecord *ar = new ActivationRecord(node->procedureName(), _callStack.top());
    _callStack.push(ar);

    vector<Expr *> actualParams = node->actualParams();
    ProcedureSymbol *procedureSymbol = node->procedureSymbol();
    vector<Symbol *> formalParams = procedureSymbol->formalParams();

    // now map the formalParams to the actual Params in the call stack of this procedure
    for (int i = 0; i < actualParams.size(); i++)
    {
        string name = formalParams[i]->name();
        nodeVisitorResult value = visit(actualParams[i]);

        _callStack.set(name, value);
    }

    AST *blockAst = procedureSymbol->blockAst();

    visit(blockAst);

    _callStack.printAR();
    _callStack.pop();

    return 0;
}

nodeVisitorResult Interpreter::_visitNoOP(NoOp *node)
{
    return 0;
}

nodeVisitorResult Interpreter::interpret()
{
    return visit(_tree);
}