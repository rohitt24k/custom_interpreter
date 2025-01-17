#include "../header_files/Interpreter.h"

int Interpreter::_visitNum(Num *node)
{
    if (node->token().type() == Token::TokenType::INTEGER_CONST)
    {
        return stoi(node->value());
    }
}

int Interpreter::_visitBinOp(BinOp *node)
{

    int leftResult = visit(node->left());
    int rightResult = visit(node->right());

    // cout << "Binary left-> " << leftResult << " operator->" << node->op.getValue() << " right-> " << rightResult << endl;

    if (node->op().type() == Token::TokenType::PLUS)
    {
        return leftResult + rightResult;
    }
    if (node->op().type() == Token::TokenType::SUBTRACT)
    {
        return leftResult - rightResult;
    }
    if (node->op().type() == Token::TokenType::MULTIPLICATION)
    {
        return leftResult * rightResult;
    }
    if (node->op().type() == Token::TokenType::INTEGER_DIV)
    {
        if (rightResult == 0)
        {
            // Handle division by zero
            string errorMessage =
                "Division by zero encountered in binary operation at line " +
                to_string(node->op().line()) + ", column " +
                to_string(node->op().column()) + ".";
            Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->op().line(), node->op().column());
        }
        return leftResult / rightResult;
    }
    // if (node->op.getType() == TokenType::FLOAT_DIV)
    // {
    //     return leftResult.divide(REAL, rightResult);
    // }

    string errorMessage =
        "Unknown binary operator '" + node->op().value() +
        "' encountered at line " + to_string(node->op().line()) +
        ", column " + to_string(node->op().column()) + ".";
    Error::throwFatalError(Error::ErrorType::RuntimeError, errorMessage, node->op().line(), node->op().column());
}

int Interpreter::_visitUniaryOp(UniaryOp *node)
{
    int result = visit(node->expr());
    if (node->op().type() == Token::TokenType::PLUS)
    {
        return result;
    }
    else
    {
        return -1 * result;
    }
}

int Interpreter::interpret()
{
    AST *tree = _parser.parse();
    return visit(tree);
}