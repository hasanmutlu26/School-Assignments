part1:
	yacc -d gpp_interpreter.y
	lex gpp_interpreter.l
	gcc y.tab.c lex.yy.c -o gpp_interpreter