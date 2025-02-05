#include <iostream>
#include "../header_files/Interpreter.h"
#include "../header_files/Drawing.h"
#include "../header_files/SymbolTableBuilder.h"

int main()
{
  string code = R"(
program FactorialUsingRecursion;
var
  result : Integer;  { Global variable to store the factorial }

procedure Factorial(n: Integer);
begin
  if n = 0 then
    result := 1  { Base case: 0! = 1 }
  else
  begin
    Factorial(n - 1);  { Recursive call }
    result := result * n;  { Multiply result by n }
  end;
end;

begin
  Factorial(6); { Call the recursive procedure }
  writeln('Factorial of 5 is: ', result);  { Print the result }
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
  // cout << endl;
  interpreter.interpret();

  // cout << get<int>() << endl;

  // drawing.interpret();

  // cout << endl;
  // cout << endl;
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