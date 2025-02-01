#include <iostream>
#include "../header_files/Interpreter.h"
#include "../header_files/Drawing.h"
#include "../header_files/SymbolTableBuilder.h"

int main()
{
   string code = R"(
program Main;

procedure Outer(a: integer);
var
  x: integer;
  
  procedure Inner(b: integer);
  var
    y: integer;
  begin
    y := a + b;  { 'a' is accessible here because of the nested scope }
    {writeln('Result from Inner:', y);}
  end;

begin
  x := a * 2;
  {writeln('Result from Outer:', x);}
  Inner(x);  { Calling the nested procedure }
end;

begin
  Outer(5);
end.

    )";

   Lexer lexer(code);

   // Token token = lexer.getNextToken();
   // while (token.type() != Token::TokenType::EOD)
   // {
   //    cout << token.tokenTypeToString(token.type()) << " - " << token.value() << endl;
   //    token = lexer.getNextToken();
   // }
   Parser parser(lexer);
   AST *tree = parser.parse();
   // DrawingInterpreter drawing(parser);
   SymbolTableBuilder stb(tree);

   Interpreter interpreter(tree);

   stb.buildSymbolTable();
   cout << endl;

   cout << get<int>(interpreter.interpret()) << endl;
   cout << endl;
   // drawing.interpret();

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