#ifndef LEXER_H
#define LEXER_H

char *readFileToString(FILE *f);
struct TokenList* tokenizeFile(char *fileString, int *tokenCount);
void removeSpaces(char *source);

#endif // !LEXER_H
