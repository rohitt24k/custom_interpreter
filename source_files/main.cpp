#include <iostream>
#include "../header_files/Interpreter.h"

int main()
{

    string code = "3+4-5+1";
    Interpreter interpreter(code);

    cout << interpreter.expr() << endl;

    // cout << "Hello World" << endl;
    return 0;
}