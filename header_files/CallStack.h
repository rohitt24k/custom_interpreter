#ifndef CALLSTACK_H
#define CALLSTACK_H

#include "Error.h"

#include <stack>
#include <vector>
#include <variant>
#include <string>
#include <unordered_map>

using namespace std;

using nodeVisitorResult = variant<int, double, string>;

// callstack will contain activation record which is a vector<string, nodeVisitorResult>

class ActivationRecord
{
private:
    string _name;
    unordered_map<string, nodeVisitorResult> _members;
    ActivationRecord *_enclosingActivationRecord;

public:
    ActivationRecord(const string name, ActivationRecord *enclosingActivationRecord = NULL) : _name(name), _enclosingActivationRecord(enclosingActivationRecord) {}
    void set(string name, nodeVisitorResult value) { _members[name] = value; }
    nodeVisitorResult &get(const string name)
    {
        auto it = _members.find(name);
        if (it != _members.end())
            return it->second;

        if (_enclosingActivationRecord)
            return _enclosingActivationRecord->get(name);

        string errorMessage = "Variable '" + name + "' is not declared in the current activation record '" + _name + "'.";

        throw runtime_error(errorMessage);
    }

    friend ostream &operator<<(ostream &os, const ActivationRecord &activationRecord)
    {
        struct ValueVisitor
        {
            ostream &os;
            void operator()(int i) const { os << i; }
            void operator()(double d) const { os << d; }
            // void operator()(bool b) const { os << boolalpha << b; }
            void operator()(const string &s) const { os << "\"" << s << "\""; }
        };

        os << "=== Activation Record: " << activationRecord._name << " ===\n";

        if (activationRecord._enclosingActivationRecord != nullptr)
        {
            os << "Enclosing AR: " << activationRecord._enclosingActivationRecord->_name << "\n";
        }
        os << "Members:\n";

        for (const auto &[name, value] : activationRecord._members)
        {
            os << "  " << name << " = ";
            std::visit(ValueVisitor{os}, value);
            os << "\n";
        }

        return os << "========================" << endl;
    }
};

class CallStack
{

private:
    stack<ActivationRecord *> _records;

public:
    void push(ActivationRecord *activationRecord)
    {
        _records.push(activationRecord);
    }

    void pop()
    {
        if (!_records.empty())
            _records.pop();
    }

    ActivationRecord *top() const
    {
        // if (_records.empty())
        // {
        //     Error::throwFatalError(Error::ErrorType::RuntimeError, "Call stack is empty. No activation record available.");
        // }

        return _records.empty() ? NULL : _records.top();
    }

    nodeVisitorResult &get(const string &name)
    {
        if (_records.empty())
        {
            throw runtime_error("Attempted to access variable '" + name + "' but the call stack is empty.");
        }

        return top()->get(name);
    }

    void set(const string &name, const nodeVisitorResult &value)
    {
        if (_records.empty())
        {
            throw runtime_error("Attempted to assign value to '" + name + "' but the call stack is empty.");
        }

        top()->set(name, value);
    }

    void printAR()
    {
        cout << *top();
    }

    ~CallStack()
    {
        while (!_records.empty())
            pop();
    }
};

#endif