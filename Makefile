CFLAGS = -Wall -g -lm
CC = gcc
EXEC = pagerank
PETIT_GRAPH = GraphesWebTest/web1.txt
GRAPH_TEST = graph/Stanford/Stanford.txt
GRAPH_TEST2 = graph/wikipedia-20051105/wikipedia-20051105V2.txt #prend plus de temps

compil:
	$(CC) -o $(EXEC) pagerank.c $(CFLAGS)

run-little-graph: compil
	echo "--> Lancement de l'algo power sur " $(PETIT_GRAPH)
	./$(EXEC) $(PETIT_GRAPH)
	echo "\n--> Lancement de l'algo power Gauss-Seidel sur " $(PETIT_GRAPH)
	./$(EXEC) $(PETIT_GRAPH) seidel

run-test: compil
	./$(EXEC) $(GRAPH_TEST) seidel

clean:
	rm pagerank