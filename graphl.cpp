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


#include "graphl.h"

//---------------------------------------------------------------------------
GraphL::GraphL()
{
	// already initialize everything when struct node
}

//---------------------------------------------------------------------------
//copy constructor
GraphL::GraphL(const GraphL & other)
{
	copy(other);
}

//deep copy the GraphL object pass from parameter
void GraphL::copy(const GraphL& other)
{
	this->size = other.size;

	//initial array to the new size and full fill with empty GraphNode
	initialArray();

	//deep copy all data from other
	for (int i = 0; i < size; i++)
	{
		if (other.gndArray[i]->locate == nullptr)	return;
		else
		{
    		gndArray[i]->locate = new NodeData();
	    	*(gndArray[i]->locate) = *(other.gndArray[i]->locate);
		}		

		gndArray[i]->visited = other.gndArray[i]->visited;
		copyEdges(other.gndArray[i]->edgeHead, gndArray[i]->edgeHead);
	}
}


// copyEdges:   copy() helper
// deep copy the link list of edgeNode in the GraphNode
void GraphL::copyEdges(EdgeNode * other, EdgeNode *& cur)
{
	if (other == nullptr)		return;
	cur = new EdgeNode();
	cur->adjGraphNode = other->adjGraphNode;
	copyEdges(other->nextEdge, cur->nextEdge);
}


//---------------------------------------------------------------------------
// operator=
GraphL GraphL::operator=(const GraphL & other)
{
	if (this != &other)
	{
		makeEmpty();
		copy(other);
	}
	
	return *this;
}


//---------------------------------------------------------------------------
GraphL::~GraphL()
{
	makeEmpty();
}

//deallocate the whole graph
void GraphL::makeEmpty()
{
	for (int i = 0; i < size; i++)
	{
		EdgeNode*& temp = gndArray[i]->edgeHead;
		makeEmptyHelper(temp);
		delete gndArray[i]->locate;
		gndArray[i]->locate = nullptr;
		delete	gndArray[i];
		gndArray[i] = nullptr;
	}
	delete [] gndArray;
	gndArray = nullptr;
}

//makeEmpty() helper
//help to delete adjacency list first
void GraphL::makeEmptyHelper(EdgeNode*& temp) {
	if (temp == nullptr) return;
	makeEmptyHelper(temp->nextEdge);
	delete  temp;
	temp = nullptr;
}



//---------------------------------------------------------------------------
//create specific size of GraphNode* array, each item point to a GraphNode object
//with locate = nullptr, visited = false, edgeHead = nullptr;
void GraphL::initialArray()
{
	if (size == 0) return;
	gndArray = new GraphNode*[size](); //does this work to a pointer to array point????? is it necessary?because I dynamiclly allocate the array

	for (int i = 0; i < size; i++)
	{
		gndArray[i] = new GraphNode();
	}
}



//---------------------------------------------------------------------------
// build a Graph 
//read from istream passed from parameter, each line is 
//determian the number of node, description of node and all edges
//terminating when "0 0" is encountered.
void GraphL::buildGraph(istream & infile)
{
	int fromNode, toNode;      // from and to node of each edge
	makeEmpty();               // deallocate the tree before build
	infile >> size;             // size is int, can read directly???????????

	if (infile.eof()) return;   // what if size read eof?????????????????
	
    //stop build the graph if the size is not in the range of 1 to 100
	if (size <= 0 || size > 100) return;

	//initial array to the read size 
	//and full fill all points with empty GraphNode
	initialArray();
	
	string s;                    //use to read through the end of line
	getline(infile,s);

	//read graph node decription
	for (int i = 0; i < size; i++)
	{
		gndArray[i]->locate = new NodeData();
		gndArray[i]->locate->setData(infile);
	}

	//read the edge data and add to the adjacency list
	infile >> fromNode >> toNode;
    //return when read an 0, which means the end of a graph
    while(fromNode != 0)
	{
		//check if read invalid data, ignore it, no type checking
		if (fromNode < 0 || toNode <= 0 || fromNode > size || toNode > size)
			infile >> fromNode >> toNode;

		//set a pointer to walk the EdgeNode link list
		EdgeNode*& temp = gndArray[fromNode - 1]->edgeHead;
		//insert a new EdgeNode at the end of link list
		insertEdegeNode(temp, toNode);

		infile >> fromNode >> toNode;  // read next fromNode and to Node pair
	}
}


//buildGraph() helper
//insert a new EdgeNode at the end of adjacency link list
void GraphL::insertEdegeNode(EdgeNode*& temp, int toNode) {
	if (temp == nullptr) 
	{
		temp = new EdgeNode();
		temp->adjGraphNode = toNode;
	}
	else insertEdegeNode(temp->nextEdge, toNode);
}



//---------------------------------------------------------------------------
// print the GraphNode, description and all edges on the console
void GraphL::displayGraph() const
{
	cout << "Graph:" << endl;
	for (int i = 0; i< this->size; i++)
	{
		cout << "Node " << i + 1 << "       " << *gndArray[i]->locate << endl;
		EdgeNode* temp = gndArray[i]->edgeHead;
		displayEdgeHelper(temp, i + 1);
	}
	cout<< endl;
}

// displayGraph() helper
//recursively call this method to display all edges in reverse order
void GraphL::displayEdgeHelper(EdgeNode* temp, int nodeNumber) const
{
	if (temp == nullptr) return;
	displayEdgeHelper( temp->nextEdge, nodeNumber);
	cout << "  dege " << nodeNumber<< " " << temp->adjGraphNode << endl;
}



//---------------------------------------------------------------------------
//traverse the graph in a depthFirst order
void GraphL::depthFirstSearch() const
{
	cout << "Depthh-fist ordering:";
	for (int i = 1; i <= size; i++)
		depthFirstHelper(i);

	//change all visited in array back to false
	for (int i = 1; i <= size; i++)
		gndArray[i - 1]->visited = false;
}

//depthFirstSearch() helper
//displaying the GraphNode haven't been visit
void GraphL::depthFirstHelper(int i) const
{
	if (gndArray[i-1]->visited) return;
	gndArray[i-1]->visited = true;
	cout << " "<< i;
	EdgeNode* temp = gndArray[i-1]->edgeHead;
	lastAjaNode(temp);
}

//depthFirstSearch() helper
//recursively find the EdgeNode in a reverse order
//and call depthFirstHelper() to traverse the GraphNode havn't been visited
void GraphL::lastAjaNode(EdgeNode * temp ) const
{
	if (temp == nullptr) return;
	if (temp->nextEdge == nullptr)
	{
		depthFirstHelper(temp->adjGraphNode);
		return;
	}
	lastAjaNode(temp->nextEdge);
	depthFirstHelper(temp->adjGraphNode);
}





