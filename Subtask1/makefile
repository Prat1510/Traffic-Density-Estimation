all:
	ml-lex lexer.lex
	ml-yacc parser.yacc
	sml
	use "loader.sml"

clean:
	rm lexer.lex.sml
	rm parser.yacc.desc
	rm parser.yacc.sig
	rm parser.yacc.sml