#ifndef GENERATE_H
#define GENERATE_H

char *generateReturnExpr(struct ASTNode *statement);
char *generateFunction(struct ASTNode *function);
char *generateAssembly(struct ASTNode *root);

#endif // !GENERATE_H
