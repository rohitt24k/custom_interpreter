#include <iostream>
#include "../header_files/Interpreter.h"
#include "../header_files/Drawing.h"

int main()
{
    string code = R"(
BEGIN 
    BEGIN
        number := 2;
        a := number;
        b := 10 * a + 10 * number / 4;
        c := a - - b
    END;
    x := 11;
END.
    )";

    Lexer lexer(code);
    Parser parser(lexer);
    Interpreter interpreter(parser);
    DrawingInterpreter drawing(parser);

    cout << interpreter.interpret() << endl;
    cout << endl;
    drawing.interpret();

    cout << endl;
    cout << endl;
    cout << "SYMBOL TABLE" << endl;
    interpreter.printGlobalScope();

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