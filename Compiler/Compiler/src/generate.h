#ifndef GENERATE_H
#define GENERATE_H

void generateFactor(struct ASTNode *factor, FILE *f);
void generateTerm(struct ASTNode *term, FILE *f);
void generateExpr(struct ASTNode *expr, FILE *f);
void generateStatement(struct ASTNode *statement, FILE *f);
void generateFunction(struct ASTNode *function, FILE *f);
void generateAssembly(struct ASTNode *root, FILE *f);

#endif // !GENERATE_H
