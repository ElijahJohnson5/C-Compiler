#ifndef GENERATE_H
#define GENERATE_H

void generateFactor(struct ASTNode *factor, FILE *f, struct HashMap *map);
//Expressions
void generateTerm(struct ASTNode *term, FILE *f, struct HashMap *map);
//Expressions
void generateAdditiveExpr(struct ASTNode *addExpr, FILE *f, struct HashMap *map);
void generateBitwiseShiftExpr(struct ASTNode *bitwiseShiftExp, FILE *f, struct HashMap *map);
void generateRelationalExpr(struct ASTNode *relaExpr, FILE *f, struct HashMap *map);
void generateEqualityExpr(struct ASTNode *eqExpr, FILE *f, struct HashMap *map);
void generateBitwiseAndExpr(struct ASTNode *bitwiseAndExp, FILE *f, struct HashMap *map);
void generateBitwiseXorExpr(struct ASTNode *bitwiseXorExp, FILE *f, struct HashMap *map);
void generateBitwiseOrExpr(struct ASTNode *bitwiseOrExp, FILE *f, struct HashMap *map);
void generateLogicalAndExpr(struct ASTNode *logAndExpr, FILE *f, struct HashMap *map);
void generateLogicalOrExpr(struct ASTNode *logOrExpr, FILE *f, struct HashMap *map);
void generateExpr(struct ASTNode *expr, FILE *f, struct HashMap *map);
//End expression
void generateStatement(struct ASTNode *statement, FILE *f, struct HashMap *map);
void generateFunction(struct ASTNode *function, FILE *f, struct HashMap *map);
void generateAssembly(struct ASTNode *root, FILE *f);

#endif // !GENERATE_H
