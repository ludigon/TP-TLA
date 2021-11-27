YACC_OUT=y.tab.c
YACC_HEADER=y.tab.h
LEX_OUT=lex.yy.c
SOURCES=$(shell find -iname "*.c" -not -name ${YACC_OUT} -not -name ${LEX_OUT})

golfit: ${YACC_OUT} ${LEX_OUT} ${SOURCES}
	gcc $^ -o $@

${LEX_OUT}: golfit.l
	lex $^

${YACC_HEADER} ${YACC_OUT}: golfit.y
	yacc -H $^

clean:
	rm -rf golfit ${YACC_HEADER} ${YACC_OUT} ${LEX_OUT}

.PHONY: clean golfit
