YACC_OUT=y.tab.c
YACC_HEADER=y.tab.h
LEX_OUT=lex.yy.c
SOURCES=$(shell find -iname "*.c" -not -path ${YACC_OUT} -not -path ${LEX_OUT}) ${YACC_OUT} ${LEX_OUT}
BUILD_DIR=build
OBJECTS=$(SOURCES:%.c=${BUILD_DIR}/%.o)

golfit: ${OBJECTS}
	gcc -o $@ $^

${BUILD_DIR}/%.o: %.c
	mkdir -p ${BUILD_DIR}
	gcc -c -o $@ $^

${LEX_OUT}: golfit.l
	lex $^

${YACC_HEADER} ${YACC_OUT}: golfit.y
	yacc -H $^

clean:
	rm -rf golfit build ${YACC_HEADER} ${YACC_OUT} ${LEX_OUT}

.PHONY: clean
