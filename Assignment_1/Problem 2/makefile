run : exe
	./exe

compileAll : merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o
	gcc -g -o exe merge_trie.o my_clus_trie.o my_doc_trie.o my_linked_list.o word_token.o driver.o serializer.o

serializer.o : serializer.c _TRIE.h _LINKED_LIST.h
	gcc -c -g serializer.c

merge_trie.o : merge_trie.c _TRIE.h _LINKED_LIST.h
	gcc -c -g merge_trie.c

my_clus_trie.o : my_clus_trie.c _TRIE.h _LINKED_LIST.h
	gcc -c -g my_clus_trie.c

my_doc_trie.o : my_doc_trie.c _TRIE.h _LINKED_LIST.h
	gcc -c -g my_doc_trie.c

my_linked_list.o : my_linked_list.c _LINKED_LIST.h
	gcc -c -g my_linked_list.c

word_token.o : word_token.c _TOKENIZER.h
	gcc -c -g word_token.c

driver.o : driver.c _LINKED_LIST.h _TOKENIZER.h _TRIE.h
	gcc -c -g driver.c

clean : 
	rm *.o *.txt exe
