run : exe
	mpirun -np $(n) ./exe

gdb : exe
	mpirun -np 1 xterm -e gdb ./exe

gdb2 : exe
	mpirun -np 2 xterm -e gdb ./exe
	
compileAll : merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o stopword_removal.o bfilter.o
	mpicc -O3 -g -o exe merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o stopword_removal.o bfilter.o

BloomAll : merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o bloom_driver.o serializer.o stopword_removal.o bfilter.o
	mpicc -O3 -g -o exe merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o bloom_driver.o serializer.o stopword_removal.o bfilter.o

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

driver.o : driver.c _LINKED_LIST.h _TOKENIZER.h _TRIE.h _BFILTER.h
	mpicc -c -g driver.c

bloom_driver.o : bloom_driver.c _LINKED_LIST.h _TOKENIZER.h _TRIE.h _BFILTER.h
	mpicc -c -g bloom_driver.c

bfilter.o : bfilter.c _BFILTER.h
	mpicc -c -g bfilter.c

stopword_removal.o: stopword_removal.c _TOKENIZER.h
	mpicc -g -c stopword_removal.c

clean : 
	rm *.o exe
