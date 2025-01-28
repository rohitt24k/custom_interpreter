#include <iostream>
#include "../header_files/Interpreter.h"
#include "../header_files/Drawing.h"
#include "../header_files/SymbolTableBuilder.h"

int main()
{
   string code = R"(
PROGRAM Part12;
VAR
   a : INTEGER;

PROCEDURE P1;
VAR
   a : REAL;
   k : INTEGER;

   PROCEDURE P2;
   VAR
      a, z : INTEGER;
   BEGIN {P2}
      z := 777;
   END;  {P2}

BEGIN {P1}

END;  {P1}

BEGIN {Part12}
   a := 10;
END.  {Part12}
    )";

   Lexer lexer(code);

   // Token token = lexer.getNextToken();
   // while (token.type() != Token::TokenType::EOD)
   // {
   //    cout << token.tokenTypeToString(token.type()) << " - " << token.value() << endl;
   //    token = lexer.getNextToken();
   // }
   Parser parser(lexer);
   Interpreter interpreter(parser);
   DrawingInterpreter drawing(parser);
   SymbolTableBuilder stb(parser);

   cout << "SYMBOL TABLE" << endl;
   stb.buildSymbolTable();
   stb.printSymbolTable();
   cout << endl;
   cout << get<int>(interpreter.interpret()) << endl;
   cout << endl;
   drawing.interpret();

   cout << endl;
   cout << endl;
   cout << "GLOBAL TABLE" << endl;
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