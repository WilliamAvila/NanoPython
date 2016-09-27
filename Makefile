all: nanop
 
nanop: nanop_parser.o nanop_lexer.o ast.o main.o
	g++ -g -o $@ $^
 
main.o: main.cpp
	g++ -g -c -o $@ $<
 
nanop_lexer.o: nanop_lexer.cpp
	g++ -g -c -o $@ $<

ast.o : ast.cpp
	g++ -std=c++11 -c -o $@ $<
 
nanop_lexer.cpp: nanop.l
	flex -o $@ $<
 
nanop_parser.cpp: nanop.y
	bison --defines=tokens.h -o $@ $<
 
nanop_parse.o: nanop_parser.cpp
	g++ -g -c -o $@ $<
 
clean:
	rm -f *.o
	rm -f nanop_lexer.cpp
	rm -f nanop_parser.cpp
	rm -f tokens.h
	rm -f nanop
