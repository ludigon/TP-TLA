golfit: lex.yy.c y.tab.c
	gcc $^ -o $@

lex.yy.c: golfit.l
	lex $^

y.tab.h y.tab.c: golfit.y
	yacc -H $^

clean:
	rm -rf golfit y.tab.h y.tab.c lex.yy.c

.PHONY: clean golfit
