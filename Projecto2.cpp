// Diogo Freitas 81586
// Francisco Mira 80888
// Projecto 2 - Analise e Sintese de Algoritmos
 
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <set>

#define NIL -1
#define INF 2147483647

typedef struct{
	unsigned int vertex;
	int cost;
} Edge;

//General

int nVer, nBran, nCon, vBran, w;
unsigned int v1, v2;

std::vector< std::vector<Edge> > graph;
std::vector<int> compBranches;

//Bellman-Ford

std::vector<int> vWeight;

//Dijkstra

std::vector<int> d;
std::vector<int> dijkstraResult;

//Result

int PE = NIL, totalCost = INF;

void bellmanFordAlgorithm(int u){
	vWeight[u] = 0;
	for (unsigned int i = 1; i < graph.size(); i++){
		for (unsigned int e = 0; e < graph[i].size(); e++){
			int v = graph[i][e].vertex;
			int eCost = graph[i][e].cost;
			if (vWeight[i] + eCost < vWeight[v]){
				vWeight[v] = vWeight[i] + eCost;
			}
		}
	}
}

void reweightGraph(int u){
	bellmanFordAlgorithm(u);

	for (unsigned int i = u; i < graph.size(); i++){
		for (unsigned int e = 0; e < graph[i].size(); e++){
			int v = graph[i][e].vertex;
			int eCost = graph[i][e].cost;
			int cost = vWeight[i] + eCost - vWeight[v];
			graph[i][e].cost = cost;
		}
	}
}

void dijkstraAlgorithm(unsigned int u){
	if (d[u] == INF)
		d[u] = 0;
	for (unsigned int i = 0; i < graph[u].size(); i++){
		unsigned int v = graph[u][i].vertex;
		int eCost = graph[u][i].cost;
		if ((d[u] + eCost) < d[v]){
			d[v] = d[u] + eCost;
			dijkstraAlgorithm(v);
		}	
	}
}

void johnsonAlgorithm(){
	for (unsigned int i  = 1; i < graph.size(); i++){			//Adds zero-cost edge from vertex 0 to all other existing edges
		Edge e = {i, 0};
		graph[0].push_back(e);
	}

	reweightGraph(1);
	for (unsigned int i = 0; i < compBranches.size(); i++){
		int bran = compBranches[i];
		dijkstraAlgorithm(bran);
		for (unsigned int e = 1; e < graph.size(); e++){
			if (d[e] != INF)
				d[e] = d[e] - vWeight[bran] + vWeight[e];
		}
		for(unsigned int e = 1; e < dijkstraResult.size(); e++){
			if (d[e] == INF)
				dijkstraResult[e] = INF;
			if (d[e] != INF && dijkstraResult[e] != INF)
				dijkstraResult[e] += d[e];
		}
		d = std::vector<int>(nVer + 1, INF);
	}
	for (unsigned int i = 1; i < dijkstraResult.size(); i++){
		if (dijkstraResult[i] < totalCost){
			totalCost = dijkstraResult[i];
			PE = i;
		}
	}
	if (PE == NIL)
		printf("N\n");

	else{
		printf("%d %d\n", PE, totalCost);
		for (unsigned int i = 0; i < compBranches.size(); i++){
			int bran = compBranches[i];
			dijkstraAlgorithm(bran);
			for (unsigned int e = 1; e < graph.size(); e++){
				if (d[e] != INF)
					d[e] = d[e] - vWeight[bran] + vWeight[e];
			}
			printf("%d ", d[PE]);
			d = std::vector<int>(nVer + 1, INF);
		}
		printf("\n");
	}
}

int main() {
	scanf("%d %d %d", &nVer, &nBran, &nCon);

	graph = std::vector< std::vector<Edge> >(nVer + 1);

	vWeight = std::vector<int>(nVer + 1, 0);

	d = std::vector<int>(nVer + 1, INF);
	dijkstraResult = std::vector<int>(nVer + 1, 0);
	
	for (int b = 0; b < nBran; b++) {
		scanf("%d", &vBran);
		compBranches.push_back(vBran);
	}
	for (int c = 0; c < nCon; c++) {
		scanf("%u %u %d", &v1, &v2, &w);
		Edge e = {v2, w};
		graph[v1].push_back(e);
	}

	johnsonAlgorithm();

	return 0;
}