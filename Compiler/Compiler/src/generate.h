#ifndef GENERATE_H
#define GENERATE_H

void generateFactor(struct ASTNode *factor, FILE *f, struct HashMap *map);
//Expressions
void generateOpAssembly(char *op, FILE *f, struct HashMap *map);
void generatePrecedenceExpr(struct ASTNode *precedenceExpr, FILE *f, struct HashMap *map);
void generateExpr(struct ASTNode *expr, FILE *f, struct HashMap *map);
//End expression
void generateStatement(struct ASTNode *statement, FILE *f, struct HashMap *map);
void generateFunction(struct ASTNode *function, FILE *f, struct HashMap *map);
void generateAssembly(struct ASTNode *root, FILE *f);

#endif // !GENERATE_H
