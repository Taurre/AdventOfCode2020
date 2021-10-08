#ifndef LEXER_H
#define LEXER_H

enum {
	LX_INGREDIENT = 1,
	LX_ALLERGEN,
	LX_EOL
};

extern int yylex(void);
extern char *yytext;

#endif /* LEXER_H */
