#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "NodeVisitor.h"
#include "CallStack.h"
#include <variant>

// struct overloaded
// {
//     Token op;

//     overloaded(Token operation) : op(std::move(operation)) {}

//     template <typename T>
//     bool operator()(const string &left, T &right)
//     {
//         string errorMessage = "Invalid comparison between string and non-string type.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }

//     template <typename T>
//     bool operator()(T &left, const string &right)
//     {
//         string errorMessage = "Invalid comparison between non-string type and string.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }

//     template <typename T, typename U>
//     bool operator()(T &left, U &right) const
//     {
//         if (op.type() == Token::TokenType::GREATER_THAN)
//             return left > right;
//         if (op.type() == Token::TokenType::LESS_THAN)
//             return left < right;
//         if (op.type() == Token::TokenType::GREATER_THAN_OR_EQUAL)
//             return left >= right;
//         if (op.type() == Token::TokenType::LESS_THAN_OR_EQUAL)
//             return left <= right;
//         if (op.type() == Token::TokenType::EQUAL_TO)
//             return left == right;
//         if (op.type() == Token::TokenType::NOT_EQUAL_TO)
//             return left != right;

//         string errorMessage = "Unsupported operation in comparison.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }

//     bool operator()(const string &left, const string &right) const
//     {
//         if (op.type() == Token::TokenType::EQUAL_TO)
//             return left == right;
//         if (op.type() == Token::TokenType::NOT_EQUAL_TO)
//             return left != right;

//         string errorMessage = "Invalid comparison operator for strings.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }
// };

// struct ConditionVisitor
// {
//     Token op;

//     ConditionVisitor(Token op) : op(op) {}

//     template <typename T, typename U>
//     bool operator()(const T &left, const U &right)
//     {
//         if constexpr (std::is_same_v<T, string> || std::is_same_v<U, string>)
//         {
//             string errorMessage = "Invalid comparison between non-string type and string.";
//             Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//             return false;
//         }

//         if (op.type() == Token::TokenType::GREATER_THAN)
//             return left > right;
//         if (op.type() == Token::TokenType::LESS_THAN)
//             return left < right;
//         if (op.type() == Token::TokenType::GREATER_THAN_OR_EQUAL)
//             return left >= right;
//         if (op.type() == Token::TokenType::LESS_THAN_OR_EQUAL)
//             return left <= right;
//         if (op.type() == Token::TokenType::EQUAL_TO)
//             return left == right;
//         if (op.type() == Token::TokenType::NOT_EQUAL_TO)
//             return left != right;

//         string errorMessage = "Unsupported operation in comparison.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }

//     bool operator()(const string &left, const string &right)
//     {
//         if (op.type() == Token::TokenType::EQUAL_TO)
//             return left == right;
//         if (op.type() == Token::TokenType::NOT_EQUAL_TO)
//             return left != right;

//         string errorMessage = "Invalid comparison operator for strings. Only == and != are allowed.";
//         Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
//         return false;
//     }
// };

struct ConditionVisitor
{
    Token op;

    ConditionVisitor(Token op) : op(op) {}

    template <typename T, typename U>
    bool operator()(const T &left, const U &right)
    {
        if constexpr (std::is_same_v<T, string> || std::is_same_v<U, string>)
        {
            string errorMessage = "Invalid comparison between non-string type and string.";
            Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
            return false;
        }
        else
        {
            switch (op.type())
            {
            case Token::TokenType::EQUAL_TO:
                return left == right;
            case Token::TokenType::NOT_EQUAL_TO:
                return left != right;
            case Token::TokenType::GREATER_THAN:
                return left > right;
            case Token::TokenType::LESS_THAN:
                return left < right;
            case Token::TokenType::GREATER_THAN_OR_EQUAL:
                return left >= right;
            case Token::TokenType::LESS_THAN_OR_EQUAL:
                return left <= right;
            default:
                string errorMessage = "Invalid comparison operator.";
                Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
                return false;
            }
        }
    }

    bool operator()(const string &left, const string &right)
    {
        if (op.type() == Token::TokenType::EQUAL_TO)
            return left == right;
        if (op.type() == Token::TokenType::NOT_EQUAL_TO)
            return left != right;

        string errorMessage = "Invalid comparison operator for strings. Only == and != are allowed.";
        Error::throwFatalError(Error::ErrorType::SemanticError, errorMessage, op.line(), op.column());
        return false;
    }
};

class Interpreter : public NodeVisitor<nodeVisitorResult>
{
private:
    AST *_tree;

    nodeVisitorResult _visitNum(Num *node);
    nodeVisitorResult _visitStringLiteral(StringLiteral *node);
    nodeVisitorResult _visitBinOp(BinOp *node);
    nodeVisitorResult _visitUniaryOp(UniaryOp *node);
    nodeVisitorResult _visitVar(Var *node);

    // nodeVisitorResult _visitCondition(Condition *node);

    nodeVisitorResult _visitProgram(Program *node);

    nodeVisitorResult _visitBlock(Block *node);

    nodeVisitorResult _visitCompoundStatement(CompoundStatement *node);
    nodeVisitorResult _visitAssignStatement(AssignmentStatement *node);
    nodeVisitorResult _visitProcedureCallStatement(ProcedureCallStatement *node);
    nodeVisitorResult _visitIfelseStatement(IfelseStatement *node);

    nodeVisitorResult _visitNoOP(NoOp *node);

    nodeVisitorResult _visitProcedureDecl(ProcedureDecl *node);
    nodeVisitorResult _visitVarDecl(VarDecl *node);

    nodeVisitorResult _visitType(Type *node);

    bool _handleConditionNode(const Condition *node)
    {
        nodeVisitorResult left = visit(node->left());
        nodeVisitorResult right = visit(node->right());
        Token op = node->op();

        return std::visit(ConditionVisitor(op), left, right);
    }

    // unordered_map<string, nodeVisitorResult> _GLOBAL_SCOPE;
    CallStack _callStack;
    short _logInterpreter = 0;

public:
    Interpreter(AST *tree, int log = 0) : NodeVisitor(0), _tree(tree), _logInterpreter(log) {}
    nodeVisitorResult interpret();
    // void printGlobalScope()
    // {
    //     // cout << _GLOBAL_SCOPE.size() << endl;
    //     for (auto row : _GLOBAL_SCOPE)
    //     {
    //         std::visit([&](const auto &second)
    //                    { cout << row.first << " : " << second << endl; }, row.second);
    //     }
    // }

    void printAR()
    {
        _callStack.printAR();
    }
};

#endif // INTERPRETER_H