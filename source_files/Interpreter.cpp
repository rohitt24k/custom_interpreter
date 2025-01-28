#include "../header_files/Interpreter.h"

using nodeVisitorResult = variant<int, double>;

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
    if (_GLOBAL_SCOPE.find(node->value()) != _GLOBAL_SCOPE.end())
    {
        return _GLOBAL_SCOPE[node->value()];
    }
    else
    {
        string errorMessage = "Variable '" + node->value() + "' is not defined.";
        Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->token().line(), node->token().column());
    }
}

nodeVisitorResult Interpreter::_visitProgram(Program *node)
{
    return visit(node->block());
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
    return visit(node->block());
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
    _GLOBAL_SCOPE.insert({node->left()->value(), visit(node->right())});
    return 0;
}

nodeVisitorResult Interpreter::_visitNoOP(NoOp *node)
{
    return 0;
}

nodeVisitorResult Interpreter::interpret()
{
    AST *tree = _parser.parse();
    return visit(tree);
}