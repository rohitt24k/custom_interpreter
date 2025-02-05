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

nodeVisitorResult Interpreter::_visitStringLiteral(StringLiteral *node)
{
    return node->value();
}

// Helper function to convert any type to string
template <typename T>
string convertToString(const T &value)
{
    if constexpr (std::is_same_v<T, string>)
    {
        return value;
    }
    else
    {
        return std::to_string(value);
    }
}

// Helper function to check for division by zero
template <typename T>
void checkDivisionByZero(const T &value, BinOp *node)
{
    if (value == 0)
    {
        string errorMsg = "Division by zero at line " +
                          std::to_string(node->op().line()) + ", column " +
                          std::to_string(node->op().column());
        Error::throwFatalError(
            Error::ErrorType::RuntimeError,
            errorMsg,
            node->op().line(),
            node->op().column());
    }
}

// Helper function to throw errors with context
[[noreturn]] void throwError(const string &msg, BinOp *node)
{
    string fullMsg = msg + " at line " +
                     std::to_string(node->op().line()) + ", column " +
                     std::to_string(node->op().column());
    Error::throwFatalError(
        Error::ErrorType::RuntimeError,
        fullMsg,
        node->op().line(),
        node->op().column());
}

// Visitor struct for binary operations
struct BinOpVisitor
{
    Token::TokenType opType;
    BinOp *node;

    BinOpVisitor(Token::TokenType op, BinOp *n) : opType(op), node(n) {}

    // Handles all combinations except string-string
    template <typename T, typename U>
    nodeVisitorResult operator()(const T &left, const U &right)
    {
        if (opType == Token::TokenType::PLUS)
        {
            if constexpr (std::is_same_v<T, string> || std::is_same_v<U, string>)
            {
                return convertToString(left) + convertToString(right);
            }
            else
            {
                return left + right;
            }
        }
        else
        {
            if constexpr (std::is_same_v<T, string> || std::is_same_v<U, string>)
            {
                throwError("Operator '" + node->op().value() + "' cannot be used with strings", node);
            }
            else
            {
                return handleNumericOp(left, right);
            }
        }
    }

    // Special case for string-string operations
    nodeVisitorResult operator()(const string &left, const string &right)
    {
        if (opType == Token::TokenType::PLUS)
        {
            return left + right;
        }
        throwError("Operator '" + node->op().value() + "' cannot be used with strings", node);
    }

private:
    template <typename T, typename U>
    nodeVisitorResult handleNumericOp(const T &left, const U &right)
    {
        switch (opType)
        {
        case Token::TokenType::SUBTRACT:
            return left - right;
        case Token::TokenType::MULTIPLICATION:
            return left * right;
        case Token::TokenType::INTEGER_DIV:
        {
            checkDivisionByZero(right, node);
            return static_cast<int>(left) / static_cast<int>(right);
        }
        case Token::TokenType::FLOAT_DIV:
        {
            checkDivisionByZero(right, node);
            return static_cast<double>(left) / static_cast<double>(right);
        }
        default:
            throwError("Unsupported operator for numeric operands", node);
        }
    }
};

nodeVisitorResult Interpreter::_visitBinOp(BinOp *node)
{
    nodeVisitorResult leftResult = visit(node->left());
    nodeVisitorResult rightResult = visit(node->right());

    return std::visit(BinOpVisitor(node->op().type(), node), leftResult, rightResult);
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
    ActivationRecord *ar = new ActivationRecord("global");
    _callStack.push(ar);
    nodeVisitorResult result = visit(node->block());
    if (_logInterpreter)
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
        _callStack.set(node->left()->value(), visit(node->right()), node->left()->scopeName());
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
    if (node->procedureName() == "WRITELN")
    {
        for (auto param : node->actualParams())
        {
            nodeVisitorResult result = visit(param);
            std::visit([](auto &arg)
                       { cout << arg << " "; }, result);
        }
        cout << endl;
        return 0;
    }

    ProcedureSymbol *procedureSymbol = node->procedureSymbol();
    if (!procedureSymbol)
    {
        string errorMessage = "Procedure '" + node->procedureName() + "' is not defined.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                               node->token().line(), node->token().column());
    }

    vector<Symbol *> formalParams = procedureSymbol->formalParams();
    vector<Expr *> actualParams = node->actualParams();

    if (formalParams.size() != actualParams.size())
    {
        string errorMessage = "Procedure '" + node->procedureName() + "' expects " +
                              to_string(formalParams.size()) + " arguments but got " +
                              to_string(actualParams.size()) + ".";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                               node->token().line(), node->token().column());
    }

    ActivationRecord *ar = new ActivationRecord(node->procedureName(), _callStack.top());
    _callStack.push(ar);

    // now map the formalParams to the actual Params in the call stack of this procedure
    for (size_t i = 0; i < actualParams.size(); i++)
    {
        string name = formalParams[i]->name();
        nodeVisitorResult value = visit(actualParams[i]);
        _callStack.set(name, value, node->procedureName());
    }

    AST *blockAst = procedureSymbol->blockAst();
    if (!blockAst)
    {
        string errorMessage = "Procedure '" + node->procedureName() + "' has no body.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage,
                               node->token().line(), node->token().column());
    }
    visit(blockAst);

    if (_logInterpreter)
        _callStack.printAR();
    _callStack.pop();

    return 0;
}

nodeVisitorResult Interpreter::_visitIfelseStatement(IfelseStatement *node)
{
    if (_handleConditionNode(node->condition()))
    {
        for (auto &statement : node->thenBranch())
        {
            visit(statement);
        }
    }
    else
    {
        for (auto &statement : node->elseBranch())
        {
            visit(statement);
        }
    }
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