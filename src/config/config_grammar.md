```abnf

; Non-terminals
config		= *(directive / comment) EOF ;
directive   = name 1*WSP parameters *WSP SEMICOLON *WSP ;
			/ name *WSP LCURLY *WSP *(directive / comment) *WSP RCURLY *WSP ;
name		= IDENTIFIER ;
parameters  = 1*(value *WSP) ;
value		= WORD / number / string ;
string		= DQUOTE *CHAR DQUOTE ;
number		= 1*DIGIT ;
comment		= HASH *CHAR NEWLINE ;

; Terminals
IDENTIFIER  = ALPHA *(ALPHA / DIGIT / '_' / '-') ;
WORD		= ALPHA *(ALPHA / DIGIT / '_' / '-' / '=' /	'[' / ']') ;
DIGIT		= %x30-%x39 ;
ALPHA		= %x41-%x5A / %x61-%x7A ;
WSP			= ' ' / '\t' / NEWLINE ; lexer skips optional whitespaces
CHAR		= %x20-%x7E ;
DQUOTE		= '"' ;
LCURLY		= '{' ;
RCURLY		= '}' ;
SEMICOLON   = ';' ;
HASH		= '#' ;
NEWLINE		= '\n' ;
EOF			= ; end of input
```
