#ifndef GENERATE_H
#define GENERATE_H

void generateFactor(struct ASTNode *factor, FILE *f);
void generateTerm(struct ASTNode *term, FILE *f);
//Expressions
void generateAdditiveExpr(struct ASTNode *addExpr, FILE *f);
void generateRelationalExpr(struct ASTNode *relaExpr, FILE *f);
void generateEqualityExpr(struct ASTNode *eqExpr, FILE *f);
void generateLogicalAndExpr(struct ASTNode *logAndExpr, FILE *f);
void generateExpr(struct ASTNode *expr, FILE *f);
//End expression
void generateStatement(struct ASTNode *statement, FILE *f);
void generateFunction(struct ASTNode *function, FILE *f);
void generateAssembly(struct ASTNode *root, FILE *f);

#endif // !GENERATE_H
