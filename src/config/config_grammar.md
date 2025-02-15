```abnf

; Non-terminals
config				= *( directive / comment ) EOF ;
directive			= simple-directive / block-directive ;
simple-directive	= name [ parameters ] *WSP SEMICOLON ;
block-directive		= name [ parameters ] *WSP LCURLY *directive RCURLY ;
name				= ALPHA *( ALPHA / DIGIT / "_" ) ;
parameters			= 1*( 1*WSP parameter ) ;
parameter			= value / quoted_value ;
value				= uri / boolean / number / size ; Parsing this is left to each validator
quoted-value		= DQUOTE *( CHAR / escape DQUOTE ) DQUOTE ;
escape				= "\" ;
uri					= 1*( ALPHA / DIGIT / "-" / "_" / "/" / "." / "?" / "=" / "&" / "%" ); simple URI
boolean				= "on" / "off" ;
number				= 1*DIGIT ;
size				= 1*DIGIT [ "k" / "m" ] ;
comment				= "#" *( WSP / CHAR ) NEWLINE ;

; Terminals
LCURLY				= "{" ;
RCURLY				= "}" ;
SEMICOLON			= ";" ;
NEWLINE				= "\n" ;
EOF					= ; end of input

; Standard rules
ALPHA				= %x41-5A / %x61-7A ; A-Z/a-z
DIGIT				= %x30-39 ; 0-9
CHAR				= %x20-7E ; printable ASCII character
DQUOTE				= %x22 ; "
WSP					= SP / HTAB ;
SP					= %x20 ; ' '
HTAB				= %x09 ; '\t'

```
