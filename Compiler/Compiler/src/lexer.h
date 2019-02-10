#ifndef LEXER_H
#define LEXER_H

char *readFileToString(FILE *f);
struct TokenList* tokenizeFile(char *fileString, int *tokenCount);
void removeNewLinesTabs(char *source);
Token getNextToken(char **fileString);

#endif // !LEXER_H
