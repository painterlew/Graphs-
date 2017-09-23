//file graphm.h
//502 Homework3
//Author: Chang Liu

//---------------------------------------------------------------------------
/*GraphM class: a directed weighted graph.
also includes additional features:
----holds a set of nodes and edges connecting them
----the edges are ordered pairs which has single direction
----each edge has specific cost and weight is bigger than 0
----Each graphs having at most 100 nodes. 
----Dijkstra's shortest path:
    minimum weight path between two nodess
----To build a GraphM, read a data file consisting of many lines
    In the data, the first line tells the number of nodes
	Following is a text description of each node,
	one description per line (50 chars max length).  
	After that, each line consists of 3 ints representing an edge.
	1 2 10 means an edge from node 1 to node 2 with a weight of 10.
	Each GraphM is terminated with A zero for the first integer

//------------------------------------------------------------------
Implementation and assumptions:
----a GraphM holds an adjacency matrix, a number of nodes, 
    a 2D TableType array, and an array of NodeData
----adjacency matrix save all the edge and their weight
----a initial adjacency matrix is 100*100 2D int array
    all the value in the matrix is an infinity int 
	except when index of row == col, the value is 0 
----2D TableType array states attributes of each node to every other node
----a initial TableType array is 100*100 2D TableType array
----The three attributes struct in TableType are: 
	visited: idetify if the node has been visited initialize with false
	dist: current best distance, initialize with infinity
	path: previous vertex in the path to this vertex initialize with 0
----array of NodeData save all descriptions of the nodes
----a NodeData, holding a string   

----the file to read for build a graphM must consisted of correct 
    type data, and there must be a 0 sign after each graph
*/
//---------------------------------------------------------------------------

#pragma once
#include <string>
#include<iostream>
#include"nodedata.h"
#include <climits>
#include<iomanip>
#include <algorithm>  
#include <stack>
using namespace std;

const int MAXNODES = 100;  // default max number of the nodes in a graph

struct TableType
{
	//idetify if the node has been visited, initial all node unvisited
	bool visited = false; 
    //current best distance
	//set dist to all other node infinity
	int dist = INT_MAX; 
    //prevvious vertex in the path to this vertex
	//initial path to all other node with 0, means no path to other node
	int path = 0;     
};



//---------------------------------------------------------------------------
class GraphM
{

//---------------------------------------------------------------------------
public:
	
	//---------------------------------------------------------------------------
	//default constructor 
	//initial all the value in the adjacency matrix with an infinity value
	//except when index of row == col, the value in adjacency matrix is 0
	//and when index of row == col, the dist in TableType matrix is 0  
	GraphM(); 
	
	// destructor
    //set all the data to the initial value
	~GraphM();

	//buildGraph
	//read a data file consisting of many lines to build a graph
	//read the first line to set the number of nodes
	//read following line to set description of each node
	//one description per line(50 chars max length).
	//After that, each line consists of 3 ints representing an edge.
	//ex: 1 2 10 means an edge from node 1 to node 2 with a weight of 10.
	//Each GraphM is terminated with A zero for the first integer
	//assume the data file is correct format
	void buildGraph(istream & infile);

	//insert a new edge
	//return false if try to insert invailid data
	bool insertEdge(int fromNode, int toNode, int weight);

	//remove a edge
	//return false if try to remove invalid or inexist edge
	bool removeEdge(int fromNode, int toNode);

	//find shortes distance and path for all Nodes
	//after calling this method, all dist has been re-calculate 
	//and set to the shortest path to every other node
	//path saves the last visited node
	void findShortestPath();


	//display from one node to all the other node information
	//include description, edge shorted distance and path
	void displayAll()const;

	//display shortes distance and path between one pair of graph nodes
	//inculude the description of each path
	//return false if parameter is invalid
	bool display(int fromNode, int toNode) const;

	void displayDis(int fromNode, int toNode)const;

	void displayPathHelper(int fromNode, int toNode, stack<int>& path)const;
	

	//display shortes distance and path between one pair of graph nodes
	//void displayPath(int fromNode, int toNode, int i, int* path)const;
	void displayPath(int fromNode, int toNode,
		stack<int>& path)const;

	//display the description of each path
	void displayPathDescription(stack<int>& path)const;


	//bool savePath(int fromNode, int toNode, , int& i, int path[] )const;



	  
private:
	//number of nodes in the graph
	int size = 0;                     
	//2D TableType array
	//states attributes of each node to every other node
	TableType T[MAXNODES][MAXNODES];  
	//adjacency matrix save all the edge and their weigh
	int C[MAXNODES][MAXNODES];   
	//array of NodeData save all descriptions of the nodes
	NodeData data[MAXNODES];          
	//keeps track of whether or not the graph has been changed
	bool changed = false;


//---------------------------------------------------------------------------
//private method

	//findShortestPath helper:

	//OneNodeShortestPath
    //find shortes distance and path for one Node
	void OneNodeShortestPath(int from);

	//minNotVisited
	//return the index of not yet visited Node with the minimun distance 
	//not include distance is 0
	int minNotVisited(int from);

	//bool allVisit(int from)const;




public:
	void testDisplay()
	{
		cout << "this GraphM, size: " << size << endl;

		for (int i = 0; i < size; i++)
		{
			cout << "discription:  " << data[i] << endl;

		}
		cout << "Cost matix like below: " << endl;
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					if(C[i][j]>0&& C[i][j]<INT_MAX)
					cout << "C[" << i + 1 << "][" << j + 1 << "] weight" << C[i][j] << endl;
				}

			}
		

			cout << "the Table like below: " << endl;
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					cout << "T[" << i + 1 << "][" << j + 1 << "].visited" << setw(1) << T[i][j].visited <<"       ";
					
				}
					cout << endl;
				for (int j = 0; j < size; j++)
				{
					if (T[i][j].dist>200 || T[i][j].dist<0)
						cout << "T[" << i + 1 << "][" << j + 1 << "].dist" << "INF        " ;
					else
					cout << "T[" << i + 1 << "][" << j + 1 << "].dist" << setw(3)<<T[i][j].dist << "        ";
				}
					cout << endl;
				for (int j = 0; j < size; j++)
				{
					cout << "T[" << i + 1 << "][" << j + 1 << "].path" << setw(3) << T[i][j].path << "        ";
				}
					cout << endl;
					cout << endl;
			}
			cout << endl;
			cout << endl;
	}
};