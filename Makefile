YACC_OUT=y.tab.c
YACC_HEADER=y.tab.h
LEX_OUT=lex.yy.c
SOURCES=$(shell find -iname "*.c" -not -name ${YACC_OUT} -not -name ${LEX_OUT}) ${YACC_OUT} ${LEX_OUT}
BUILD_DIR=build
OBJECTS=$(SOURCES:%.c=${BUILD_DIR}/%.o)
CC=gcc
CFLAGS=-g

golfit: ${OBJECTS}
	$(CC) -o $@ $^ $(CFLAGS)

${BUILD_DIR}/%.o: %.c
	mkdir -p ${BUILD_DIR}
	$(CC) -c -o $@ $^ $(CFLAGS)

${LEX_OUT}: golfit.l
	lex $^

${YACC_HEADER} ${YACC_OUT}: golfit.y
	yacc -H $^

clean:
	rm -rf golfit build ${YACC_HEADER} ${YACC_OUT} ${LEX_OUT}

.PHONY: clean
