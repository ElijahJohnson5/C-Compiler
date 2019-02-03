#ifndef PARSER_H
#define PARSER_H
//Program function and statement
struct ASTNode *parseProgram(struct TokenList** tokens);
struct ASTNode *parseFunction(struct TokenList** tokens);
struct ASTNode *parseStatement(struct TokenList** tokens);
//Expressions
struct ASTNode *parseExpr(struct TokenList** tokens);
struct ASTNode *parsePrecedenceExpr(struct TokenList** tokens, int precedenceLevel);
struct ASTNode *parseFactor(struct TokenList** tokens);

//Function and statement
void prettyPrintAST(struct ASTNode *root);
void printFunction(struct ASTNode* function);
void printStatement(struct ASTNode* statement);
//Expressions
void printExpr(struct ASTNode *expr);
void printPrecedenceExpr(struct ASTNode *precedenceExp);
void printFactor(struct ASTNode *factor);

#endif