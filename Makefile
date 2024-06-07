all:
	gcc main.cpp -o main

test:
	sh run_tests.sh	

clean:
	rm -rf *.o main out