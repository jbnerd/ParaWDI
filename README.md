# Parallel WDI (Word Document Index)

- Computational Model : Message Passing on a Beowulf Style Cluster.

## [Centralized WDI](https://github.com/svaderia/ParaWDI/tree/master/Centralized)

- Construction of a Global word-document index in a distributed fashion.
	- [**NOTE** : The index itself is local to one node in the cluster. : **END OF NOTE**]
- Analysis of speedup and construction cost.

## [Decentralized WDI](https://github.com/svaderia/ParaWDI/tree/master/Decentralized)

- Construction of a completely distributed word-document index and Analysis of Query turn around time.
- Analysis of speedup by using Bloom Filters for each node.

# Authors

- [Abhishek V Joshi](https://github.com/jbnerd)
- [Shyamal Vaderia](https://github.com/svaderia)
- [Arka Talukdar](https://github.com/atalukdar)

# Dependencies

- MPICH2/OpenMPI
- gcc 5.4.0