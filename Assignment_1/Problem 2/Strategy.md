Exercise 2 and 3 
Consider a large collection of documents distributed in the secondary
memories (i.e. hard disks) of multiple nodes in a cluster:
- a) Design a program that **extracts words and their frequency of occurrence from each document and create a word-document index** 
in each node ranked on the frequency: 

i.e. for each word, a list of documents is associated with it and the list is ordered by decreasing frequency (of occurrence of that word in the document). 


Each index is local to a node a word is associated with only those documents in that node. 

Stop words i.e. frequently occurring words (e.g. a, an, the, for, if, to, then, on, etc.) need
not be indexed. Assume that a list of stop words is available. Implement this program using MPI in C.


- b) Design a program to merge all indices into one large word-document index ranked by decreasing frequency to be stored in node. Implement this
program using MPI in C.

- c) Measure the time taken for a) and b) for different numbers and sizes of documents and independently varying the number of processors used. For
each input (of a given number of documents of certain sizes), plot a curve of time taken against number of processors used.

Points to note:

a. Files are distributed.

b. Create a word list where each word points to a list of docs sorted acc to freq [local]

c. memerge such lists


Strategy:

- Start of with files dist across system [Joshi is correct in this regard]
- Give each file a global identifier(DocID). Create a table that allows O(1) or O(lg n) lookup to it.
- Create tokenizer [i have one frm NLP, no worries]
- Read files sequentially at each node. Create a dynamic array for tokens and each token points to a sorted linked list of docs (decreasing freq).
- https://nlp.stanford.edu/IR-book/html/htmledition/img194.png follow this algo.

2.b
- Once done we collect statistics from each node and then decide how to assign partitions for optimal distribution of workload of merging indexes.
(we can do this once before hand and hard code it into machine )
- Then each of the merged indexes are recieved by root node.

3.b  **response will be the collection of all documents associated with it** 
Do we have to give doc name or documents themselves are to be rendered in response?


- Create bloom filter of words in each node and store it in root.
- 













