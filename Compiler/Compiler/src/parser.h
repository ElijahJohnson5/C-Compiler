#ifndef PARSER_H
#define PARSER_H

struct ASTNode *parseProgram(struct TokenList** tokens);
struct ASTNode *parseFunction(struct TokenList** tokens);
struct ASTNode *parseStatement(struct TokenList** tokens);
struct ASTNode *parseExpr(struct TokenList** tokens);
struct ASTNode *parseEqualityExpr(struct TokenList** tokens);
struct ASTNode *parseLogicalAndExpr(struct TokenList** tokens);
struct ASTNode *parseRelationalExpr(struct TokenList** tokens);
struct ASTNode *parseAdditiveExpr(struct TokenList** tokens);
struct ASTNode *parseTerm(struct TokenList** tokens);
struct ASTNode *parseFactor(struct TokenList** tokens);

void printFactor(struct ASTNode *factor);
void printTerm(struct ASTNode *term);
//Expressions
void printAdditiveExpr(struct ASTNode *addExpr);
void printRelationalExpr(struct ASTNode *relaExpr);
void printEqualityExpr(struct ASTNode *eqExpr);
void printLogicalAndExpr(struct ASTNode *logAndExpr);
void printExpr(struct ASTNode *expr);
//End expression
void printStatement(struct ASTNode* statement);
void printFunction(struct ASTNode* function);
void prettyPrintAST(struct ASTNode *root);

#endif