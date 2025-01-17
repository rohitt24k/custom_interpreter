#include <iostream>
#include "../header_files/Interpreter.h"

int main()
{

    string code = "2+(4+4)/2*3-1*2";
    Interpreter interpreter(code);

    cout << interpreter.expr() << endl;

    // cout << "Hello World" << endl;
    return 0;
}