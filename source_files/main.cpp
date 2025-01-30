#include <iostream>
#include "../header_files/Interpreter.h"
#include "../header_files/Drawing.h"
#include "../header_files/SymbolTableBuilder.h"

int main()
{
   string code = R"(
program Main;

procedure Alpha(a : integer; b : integer);
var x : integer;
begin
   x := (a + b ) * 2;
end;

begin { Main }

   Alpha(3 + 5, 7);  { procedure call }

end.  { Main }
    )";

   Lexer lexer(code);

   // Token token = lexer.getNextToken();
   // while (token.type() != Token::TokenType::EOD)
   // {
   //    cout << token.tokenTypeToString(token.type()) << " - " << token.value() << endl;
   //    token = lexer.getNextToken();
   // }
   Parser parser(lexer);
   // Interpreter interpreter(parser);
   DrawingInterpreter drawing(parser);
   SymbolTableBuilder stb(parser);

   stb.buildSymbolTable();
   cout << endl;

   // cout << get<int>(interpreter.interpret()) << endl;
   cout << endl;
   drawing.interpret();

   cout << endl;
   cout << endl;
   // cout << "GLOBAL TABLE" << endl;
   // interpreter.printGlobalScope();

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