run : exe
	mpirun -np 1 ./exe

run2 : exe
	mpirun -np 2 ./exe

run4 : exe
	mpirun -np 4 ./exe

run8 : exe
	mpirun -np 8 ./exe

gdb : exe
	mpirun -np 1 xterm -e gdb ./exe

gdb2 : exe
	mpirun -np 2 xterm -e gdb ./exe
	
compileAll : merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o
	mpicc -O3 -g -o exe merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o

serializer.o : serializer.c _TRIE.h _LINKED_LIST.h
	mpicc -c -g serializer.c
	
merge_trie.o : merge_trie.c _TRIE.h _LINKED_LIST.h
	mpicc -c -g merge_trie.c

my_clus_trie.o : my_clus_trie.c _TRIE.h _LINKED_LIST.h
	mpicc -c -g my_clus_trie.c

my_doc_trie.o : my_doc_trie.c _TRIE.h _LINKED_LIST.h
	mpicc -c -g my_doc_trie.c

my_linked_list.o : my_linked_list.c _LINKED_LIST.h
	mpicc -c -g my_linked_list.c

word_token.o : word_token.c _TOKENIZER.h
	mpicc -c -g word_token.c

driver.o : driver.c _LINKED_LIST.h _TOKENIZER.h _TRIE.h
	mpicc -c -g driver.c

clean : 
	rm *.o exe
