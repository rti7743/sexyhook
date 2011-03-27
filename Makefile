# Makefile

a.out: main.cpp
## for gcc3,x ?
##	g++ -g -O0 main.cpp
	g++ -g -O0 --input-charset=cp932 main.cpp

clean:
	rm -f a.out

