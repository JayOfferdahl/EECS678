STUDENT_ID=2760730

all: producer_consumer

producer_consumer: producer_consumer.c
	gcc -g -o producer_consumer producer_consumer.c -lpthread -lm 

tar: clean
	make clean
	mkdir $(STUDENT_ID)-pthreads_pc-lab
	cp producer_consumer.c Makefile $(STUDENT_ID)-pthreads_pc-lab/
	tar cvzf $(STUDENT_ID)-pthreads_pc-lab.tar.gz $(STUDENT_ID)-pthreads_pc-lab
	rm -rf $(STUDENT_ID)-pthreads_pc-lab

all-tests: test1 test2 test3 test4 test5 test6 test7 test8

test1: producer_consumer
	@echo "  P-1 C-1   " > narrative1.raw
	@echo "************" >> narrative1.raw
	@echo "    RAW     " >> narrative1.raw
	@echo "************" >> narrative1.raw
	@./producer_consumer 1 1 >> narrative1.raw
	@echo "  P-1 C-1   " > narrative1.sorted
	@echo "************" >> narrative1.sorted
	@echo "  Sorted    " >> narrative1.sorted
	@echo "************" >> narrative1.sorted
	@grep "con 0"  narrative1.raw >> narrative1.sorted
	@echo "    " >> narrative1.sorted
	@grep "prod 0" narrative1.raw >> narrative1.sorted

test2: producer_consumer
	@echo "  P-1 C-5   " > narrative2.raw
	@echo "************" >> narrative2.raw
	@echo "    RAW     " >> narrative2.raw
	@echo "************" >> narrative2.raw
	@./producer_consumer 1 5 >> narrative2.raw
	@echo "  P-1 C-5   " > narrative2.sorted
	@echo "************" >> narrative2.sorted
	@echo "  Sorted    " >> narrative2.sorted
	@echo "************" >> narrative2.sorted
	@grep "con 0"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 1"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 2"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 3"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "con 4"  narrative2.raw >> narrative2.sorted
	@echo "    " >> narrative2.sorted
	@grep "prod 0" narrative2.raw >> narrative2.sorted

test3: producer_consumer
	@echo "  P-5 C-1   " > narrative3.raw
	@echo "************" >> narrative3.raw
	@echo "    RAW     " >> narrative3.raw
	@echo "************" >> narrative3.raw
	@./producer_consumer 5 1 >> narrative3.raw
	@echo "  P-5 C-1   " > narrative3.sorted
	@echo "************" >> narrative3.sorted
	@echo "  Sorted    " >> narrative3.sorted
	@echo "************" >> narrative3.sorted
	@grep "con 0"  narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 0" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 1" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 2" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 3" narrative3.raw >> narrative3.sorted
	@echo "    " >> narrative3.sorted
	@grep "prod 4" narrative3.raw >> narrative3.sorted

test4: producer_consumer
	@echo "  P-5 C-5   " > narrative4.raw
	@echo "************" >> narrative4.raw
	@echo "    RAW     " >> narrative4.raw
	@echo "************" >> narrative4.raw
	@./producer_consumer 5 5 >> narrative4.raw
	@echo "  P-5 C-5   " > narrative4.sorted
	@echo "************" >> narrative4.sorted
	@echo "  Sorted    " >> narrative4.sorted
	@echo "************" >> narrative4.sorted
	@grep "con 0"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 1"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 2"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 3"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "con 4"  narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 0" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 1" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 2" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 3" narrative4.raw >> narrative4.sorted
	@echo "    " >> narrative4.sorted
	@grep "prod 4" narrative4.raw >> narrative4.sorted

test5: producer_consumer
	@echo "  P-10 C-1  " > narrative5.raw
	@echo "************" >> narrative5.raw
	@echo "    RAW     " >> narrative5.raw
	@echo "************" >> narrative5.raw
	@./producer_consumer 10 1 >> narrative5.raw
	@echo "  P-10 C-1  " > narrative5.sorted
	@echo "************" >> narrative5.sorted
	@echo "  Sorted    " >> narrative5.sorted
	@echo "************" >> narrative5.sorted
	@grep "con 0"  narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 0" narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 1"  narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 2"  narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 3"  narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 4"  narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 5" narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 6" narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 7" narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 8" narrative5.raw >> narrative5.sorted
	@echo "    " >> narrative5.sorted
	@grep "prod 9" narrative5.raw >> narrative5.sorted

test6: producer_consumer
	@echo "  P-1 C-10  " > narrative6.raw
	@echo "************" >> narrative6.raw
	@echo "    RAW     " >> narrative6.raw
	@echo "************" >> narrative6.raw
	@./producer_consumer 1 10 >> narrative6.raw
	@echo "  P-1 C-10  " > narrative6.sorted
	@echo "************" >> narrative6.sorted
	@echo "   Sorted   " >> narrative6.sorted
	@echo "************" >> narrative6.sorted
	@grep "con 0"  narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 1"  narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 2"  narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 3"  narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 4"  narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 5" narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 6" narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 7" narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 8" narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "con 9" narrative6.raw >> narrative6.sorted
	@echo "    " >> narrative6.sorted
	@grep "prod 0" narrative6.raw >> narrative6.sorted

test7: producer_consumer
	@echo "  P-2 C-2   " > narrative7.raw
	@echo "************" >> narrative7.raw
	@echo "    RAW     " >> narrative7.raw
	@echo "************" >> narrative7.raw
	@./producer_consumer 2 2 >> narrative7.raw
	@echo "  P-2 C-2   " > narrative7.sorted
	@echo "************" >> narrative7.sorted
	@echo "  Sorted    " >> narrative7.sorted
	@echo "************" >> narrative7.sorted
	@grep "con 0"  narrative7.raw >> narrative7.sorted
	@echo "    " >> narrative7.sorted
	@grep "con 1"  narrative7.raw >> narrative7.sorted
	@echo "    " >> narrative7.sorted
	@grep "prod 0"  narrative7.raw >> narrative7.sorted
	@echo "    " >> narrative7.sorted
	@grep "prod 1" narrative7.raw >> narrative7.sorted

test8: producer_consumer
	@echo " P-10 C-10  " > narrative8.raw
	@echo "************" >> narrative8.raw
	@echo "    RAW     " >> narrative8.raw
	@echo "************" >> narrative8.raw
	@./producer_consumer 10 10 >> narrative8.raw
	@echo " P-10 C-10  " > narrative8.sorted
	@echo "************" >> narrative8.sorted
	@echo "  Sorted    " >> narrative8.sorted
	@echo "************" >> narrative8.sorted
	@grep "con 0"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 1"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 2"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 3"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 4"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 5"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 6"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 7"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 8"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "con 9"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 0"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 1"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 2"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 3"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 4"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 5"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 6"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 7"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 8"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted
	@grep "prod 9"  narrative8.raw >> narrative8.sorted
	@echo "    " >> narrative8.sorted

clean:
	rm -f *~ *.raw *.sorted producer_consumer

distclean: clean
