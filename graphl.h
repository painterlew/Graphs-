//file graphl.h
//502 Homework3
//Author: Chang Liu

//---------------------------------------------------------------------------
/*Graphl class: a directed graph.
also includes additional features:
----holds a set of nodes and edges connecting them
----the edges are ordered pairs which has single direction
----Each graphs having at most 100 nodes. 
----depth-first-serch
    a generalization of preordertraversal, visit adjaceny nodes then visit
	their adjacency node
----To build a Graphl, read a data file consisting of many lines
    In the data, the first line tells the number of nodes
    Following is a text description of each node,
    one description per line (50 chars max length).  
    After that, each line consists of a ints representing an edge.
    1 2 means an edge from node 1 to node 2
    Each GraphM is terminated with a zero for the first integer

//------------------------------------------------------------------
Implementation and assumptions:
----a GraphL holds an array of GraphNode
----a GraphNode saves a NodeData pointer, 
    a bool indetify if this node has been visited, 
	and a pointer to a EdgeNode.
	NodeData holds a string which saves the decription of this node
----the adjacency Nodes is a EdgeNode type link list
----a EdgeNode holds an integer and a pointer to next EdgeNode
    the integer is subscript of the adjacent graph node 
----the file to read for build a graphM must consisted of correct
    type data, and there must be a 0 sign after each graph
*/
//---------------------------------------------------------------------------


#pragma once
#include<string>
#include <iostream>
#include "nodedata.h"
using namespace std;


//---------------------------------------------------------------------------
//a GraphNode saves a NodeData pointer, a bool indetify if 
//this node has been visited, and a pointer to a EdgeNode.
//NodeData holds a string which saves the locate/decription of this node

struct EdgeNode;
struct GraphNode {
	NodeData * locate = nullptr;//the locate/decription of this node
	bool visited = false;       //indetify if this node has been visited
	EdgeNode* edgeHead = nullptr;// head of the list of edges
};

struct EdgeNode {
	int adjGraphNode = 0;        //subscript of the adjacent graph node 
	EdgeNode* nextEdge = nullptr;//pointer to next EdgeNode
};



//---------------------------------------------------------------------------
class GraphL
{
public:
	GraphL();								  //defalt constructor
	GraphL(const GraphL& other);              //copy constructor
	GraphL operator=(const GraphL& other);    //operator==
	~GraphL();                                //destrucor

	// build a Graph 
	//read from istream passed from parameter, each line is 
	//determian the number of node, description of node and all edges
	//terminating when "0 0" is encountered.
	void buildGraph(istream& infile);

	// print the GraphNode, description and all edges on the console
	void displayGraph()const;
	
	//traverse the graph in a depthFirst order
	void depthFirstSearch() const;
	


private:
	int size = 0;           // number of GraphNode in this Graph


	// dynamic allocated array that saves all the GraphNode*
	GraphNode ** gndArray = nullptr;

	//initial array to specific size and full fill all nullptr
	void initialArray();

	//deep copy the GraphL object pass from parameter
	void copy(const GraphL& other);

	// copy() helper
	// deep copy the link list of edgeNode in the GraphNode
	void copyEdges( EdgeNode * other, EdgeNode*& cur);


	//deallocate the whole GraphL
	void makeEmpty();

	//makeEmpty() helper
	//help to delete adjacency list first
	void  makeEmptyHelper(EdgeNode*& temp);

	//buildGraph() helper
	//insert a new EdgeNode at the end of adjacency link list
	void insertEdegeNode(EdgeNode*& temp, int toNode);


	// displayGraph() helper
	//recursively call this method to display all edges in reverse order
	void displayEdgeHelper(EdgeNode* temp, int nodeNumber) const;

	//depthFirstSearch() helper
	//displaying the GraphNode haven't been visit
	void depthFirstHelper(int i) const;

	////depthFirstSearch() helper
	//recursively find the EdgeNode in a reverse order
	//and call depthFirstHelper() to traverse the GraphNode havn't been visited
	void lastAjaNode(EdgeNode * temp) const;
};

