#
# EECS 678
#

STUDENT_ID="2760730"

fptrs: main.c
	gcc -g main.c -lpthread -lm -o fptrs

test: function_ptrs_test

function_ptrs_test: fptrs
	./fptrs

.PHONY : clean
clean:
	rm -rf fptrs *.o

tar: 
	make clean
	mkdir $(STUDENT_ID)-function_ptrs-lab
	cp Makefile main.c process.txt $(STUDENT_ID)-function_ptrs-lab/
	tar cvzf $(STUDENT_ID)-function_ptrs-lab.tar.gz $(STUDENT_ID)-function_ptrs-lab
	rm -rf $(STUDENT_ID)-function_ptrs-lab