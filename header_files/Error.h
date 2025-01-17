#pragma once

#include <iostream>
#include <string>
#include <exception>

using namespace std;

class Error : public exception
{
public:
    // Enum for error types
    enum class ErrorType
    {
        SyntaxError,
        RuntimeError,
        SemanticError,
        IOError,
        UnknownError
    };

    // Enum for severity levels
    enum class Severity
    {
        Warning, // Non-critical, execution can continue
        Fatal    // Critical, execution must stop
    };

private:
    ErrorType _errorType; // Type of the error
    Severity _severity;   // Severity of the error
    string _message;      // Human-readable error message
    int _line;            // Line number of the error
    int _column;          // Column position of the error

public:
    // Constructor
    Error(ErrorType type, Severity sev, const string &msg, int lineNum = -1, int colNum = -1)
        : _errorType(type), _severity(sev), _message(msg), _line(lineNum), _column(colNum) {}

    // Getters
    ErrorType errorType() const { return _errorType; }
    Severity severity() const { return _severity; }
    const char *what() const noexcept override { return _message.c_str(); }
    int line() const { return _line; }
    int column() const { return _column; }

    // Display error information
    void showError() const
    {
        cerr << "Error [" << errorTypeToString(_errorType) << "]"
             << " at line " << (_line >= 0 ? to_string(_line) : "unknown")
             << ", column " << (_column >= 0 ? to_string(_column) : "unknown") << ": "
             << _message << endl;

        if (_severity == Severity::Fatal)
        {
            cerr << "Execution stopped due to a fatal error." << endl;
        }
    }

    // Static helper to convert error type to string
    static string errorTypeToString(ErrorType type)
    {
        switch (type)
        {
        case ErrorType::SyntaxError:
            return "Syntax Error";
        case ErrorType::RuntimeError:
            return "Runtime Error";
        case ErrorType::SemanticError:
            return "Semantic Error";
        case ErrorType::IOError:
            return "IO Error";
        default:
            return "Unknown Error";
        }
    }

    // Check if the error is fatal
    bool isFatal() const { return _severity == Severity::Fatal; }

    // Static function to throw a fatal error
    static void throwFatalError(ErrorType type, const string &msg, int lineNum = -1, int colNum = -1)
    {
        Error error(type, Severity::Fatal, msg, lineNum, colNum);
        error.showError();
        exit(1);
    }

    // Static function to throw a warning (non-fatal error)
    static void throwWarning(ErrorType type, const string &msg, int lineNum = -1, int colNum = -1)
    {
        Error error(type, Severity::Warning, msg, lineNum, colNum);
        error.showError();
    }
};
