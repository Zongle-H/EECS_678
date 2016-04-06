MY_LAST_NAME = "Nichols"
STUDENT_ID = 2374294

qsort: qsort.c
	gcc -g qsort.c -o qsort

test: qsort
	./qsort

clean:
	rm -f qsort

tar:
	make clean
	mkdir $(MY_LAST_NAME)-$(STUDENT_ID)-qsort_comparer-lab
	cp Makefile qsort.c $(MY_LAST_NAME)-$(STUDENT_ID)-qsort_comparer-lab/
	tar cvzf $(MY_LAST_NAME)-$(STUDENT_ID)-qsort_comparer-lab.tar.gz $(MY_LAST_NAME)-$(STUDENT_ID)-qsort_comparer-lab
	rm -rf $(MY_LAST_NAME)-$(STUDENT_ID)-qsort_comparer-lab
