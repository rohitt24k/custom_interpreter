#include <iostream>
#include "../header_files/Interpreter.h"

int main()
{
    string code = "7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)";

    Lexer lexer(code);
    Parser parser(lexer);
    Interpreter interpreter(parser);

    cout << interpreter.interpret() << endl;

    // int userInput = 0;
    // cout << "For next type 1 ";
    // cin >> userInput;

    // while (userInput == 1)
    // {
    //     Token currentToken = lexer.getNextToken();
    //     cout << "type -> " << Token::tokenTypeToString(currentToken.type()) << endl;
    //     cout << "value -> " << currentToken.value() << endl;
    //     cout << "For next type 1 ";
    //     cin >> userInput;
    // }

    // cout << "Hello World" << endl;
    return 0;
}