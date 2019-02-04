#ifndef PARSER_H
#define PARSER_H
//Program function and statement
struct ASTNode *parseProgram(struct TokenList** tokens);
struct ASTNode *parseFunction(struct TokenList** tokens);
struct ASTNode *parseStatement(struct TokenList** tokens);
//Expressions
struct ASTNode *parseExpr(struct TokenList** tokens);
struct ASTNode *parseLogicalOrExpr(struct TokenList** tokens);
struct ASTNode *parseLogicalAndExpr(struct TokenList** tokens);
struct ASTNode *parseBitwiseOrExpr(struct TokenList** tokens);
struct ASTNode *parseBitwiseXorExpr(struct TokenList** tokens);
struct ASTNode *parseBitwiseAndExpr(struct TokenList** tokens);
struct ASTNode *parseEqualityExpr(struct TokenList** tokens);
struct ASTNode *parseRelationalExpr(struct TokenList** tokens);
struct ASTNode *parseBitwiseShiftExpr(struct TokenList** tokens);
struct ASTNode *parseAdditiveExpr(struct TokenList** tokens);
//Term and factor
struct ASTNode *parseTerm(struct TokenList** tokens);
struct ASTNode *parseFactor(struct TokenList** tokens);

//Function and statement
void prettyPrintAST(struct ASTNode *root);
void printFunction(struct ASTNode* function);
void printStatement(struct ASTNode* statement);
//Expressions
void printExpr(struct ASTNode *expr);
void printLogicalOrExpr(struct ASTNode *logOrExpr);
void printLogicalAndExpr(struct ASTNode *logAndExpr);
void printBitwiseOrExpr(struct ASTNode *bitwiseOrExpr);
void printBitwiseXorExpr(struct ASTNode *bitwiseXorExpr);
void printBitwiseAndExpr(struct ASTNode *bitwiseAndExpr);
void printEqualityExpr(struct ASTNode *eqExpr);
void printRelationalExpr(struct ASTNode *relaExpr);
void printBitwiseShiftExpr(struct ASTNode *bitwiseShiftExpr);
void printAdditiveExpr(struct ASTNode *addExpr);
//Term and factor
void printTerm(struct ASTNode *term);
void printFactor(struct ASTNode *factor);

#endif