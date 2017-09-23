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

//---------------------------------------------------------------------------
Implementation and assumptions:
----a GraphM holds an adjacency matrix, a number of nodes,
    a 2D TableType array, and an array of NodeData
----adjacency matrix save all the edge and their weight
----a initial adjacency matrix is 100*100 2D int array
    all the value in the matrix is an infinity int
    except when index of row == col, the value is 0
----2D TableType array 
    states attributes of each node to every other node
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

#include "graphm.h"

//---------------------------------------------------------------------------
//default constructor 
//initial all the value in the adjacency matrix with an infinity value
//except when index of row == col, the value in adjacency matrix is 0
//and when index of row == col, the dist in TableType matrix is 0          
GraphM::GraphM()
{
	for (int i = 0; i < MAXNODES; i++)
	{
		for (int j = 0; j < MAXNODES; j++)
		{
			if (i == j)
			{
				C[i][j] = 0;      //initial weight to this node itself 0 
				T[i][j].dist = 0; //initial dist to this node itself 0
			}
			else	       
				C[i][j] = INT_MAX;//initial other weight with inifinity, 
			                      //means this node has no edge
		}
	}
}

//---------------------------------------------------------------------------
//destructor
//set all the data to the initial value
GraphM::~GraphM()
{
	for (int i = 0; i < MAXNODES; i++)
	{
		data[i].setData("");    //set all descroption to empty string
		for (int j = 0; j < MAXNODES; j++)
		{
			if (i == j)
			{
				C[i][j] = 0;      //set weight to this node itself 0 
				T[i][j].dist = 0; //set dist to this node itself 0
			}
			else
			{
				//set all other weight to inifinity, 
				//means this node has no edge
				C[i][j] = INT_MAX; 
				//set all other nodes unvisited
				T[i][j].visited = false;
				//set all other dist to all other node infinity
				T[i][j].dist = INT_MAX;
				//set all other path to all other node 0
				//means no path to other node
				T[i][j].path = 0;  
			}
		}
	}
}



//---------------------------------------------------------------------------
//buildGraph
//read a data file consisting of many lines to build a graph
//read the first line to set the number of nodes
//read following line to set description of each node
//one description per line(50 chars max length).
//After that, each line consists of 3 ints representing an edge.
//ex: 1 2 10 means an edge from node 1 to node 2 with a weight of 10.
//Each GraphM is terminated with A zero for the first integer
//assume the data file is correct format
void GraphM::buildGraph(istream & infile)
{
	// fromNode, toNode and it's weight of each edge
	int fromNode, toNode, weight;

	infile >> size;         // read size is int

	if (infile.eof()) return;   

	//stop build the graph if the size is not in the range of 1 to 100
	if (size <= 0 || size > 100) return;
	
	//since read new data to build a graph, set changed to true
	changed = true;

	string s;               //use to read through the end of line
	getline(infile, s);

	//read graph node decription
	for (int i = 0; i < size; i++)
		data[i].setData(infile);// how to make sure only read 50 string?

	//read next three int
	infile >> fromNode >> toNode >> weight;
	//return when read an 0, which means the end of a graph
	while (fromNode != 0)
	{
		//check if read invalid data, ignore it, no type checking
		if (fromNode < 0 || toNode <= 0 || fromNode == toNode ||
			fromNode > size || toNode > size ||
			(weight <= 0 && fromNode != toNode))
			infile >> fromNode >> toNode;

		C[fromNode - 1][toNode - 1] = weight;
		infile >> fromNode >> toNode >> weight;       // read next edge 
	}
}


//---------------------------------------------------------------------------
//insert a new edge
//return false if try to insert invailid data
bool GraphM::insertEdge(int fromNode, int toNode, int weight)
{
	if (weight<=0 || fromNode <= 0 || fromNode>size|| toNode<=0
		|| toNode>size || fromNode==toNode)
    	return false;
	else
	{
		//build a new edge or change an edge to a new weight
		C[fromNode-1][toNode-1] = weight;
		//since build or change a new edge in graph, set changed to true
		changed = true;
		return true;
	}
}




//---------------------------------------------------------------------------
//remove a edge
//return false if try to remove invalid or inexist edge
bool GraphM::removeEdge(int fromNode, int toNode )
{
	//return false if parameter is invalidtry or try remove inexist edge
	if ( fromNode <= 0 || fromNode>size || toNode <= 0 || toNode>size 
		|| fromNode == toNode || C[fromNode - 1][toNode - 1] == INT_MAX)
		return false;
	else
	{
		//set the weight of that edge to infinity, which means no edge
		C[fromNode - 1][toNode - 1] = INT_MAX;
		//since remove an edge in graph, set changed to true
		changed = true;
		return true;
	}
}



//---------------------------------------------------------------------------
//find shortes distance and path for all Nodes
//after calling this method, all dist has been re-calculate 
//and set to the shortest path to every other node
//path saves the last visited node
void GraphM::findShortestPath()
{
	for (int i = 0; i < size; i++)// loop all source node
		OneNodeShortestPath(i);

	//after calculate the shortes path, set changed to false
	changed = false; 
}

//OneNodeShortestPath
//findShortestPath helper: find shortes distance and path for one node
void GraphM::OneNodeShortestPath(int source)
{
	for(int i=0; i<size-1;i++)// loop all the other node, check shortest path 
	{
		//minNotVisited() return the minimun distance and no-visited Node
		//return -1 if all unvisited node has distance of infinity
		int v = minNotVisited(source);
		if (v == -1) break;       //break the loop if not found

		T[source][v].visited = true;//set this node visited
		for (int w=0; w<size; w++)//loop all adjacency node
		{
			if (C[v][w] < INT_MAX && !T[source][w].visited)
				//check Dijkstra's algorithm
				//if going through v is shorter than source to w
				if ( T[source][v].dist + C[v][w] < T[source][w].dist)
				{
					//set the shortest dist to the dist through v
					T[source][w].dist = T[source][v].dist + C[v][w];
					//set the path through v
					T[source][w].path = (v==source? 0:v);
				}
		}
	}
}

//return the index of not yet visited Node with the minimun distance 
//return -1 if all unvisited node has distance of infinity 
//include distance is 0 eg:C[0][0], C[2][2]
int GraphM::minNotVisited( int from)
{
	int minNode = -1;
	int minValue = INT_MAX; //initial minValue to infinity
	for (int i = 0; i < size; i++)
	{
		//find the shorter distance node and not visited yet  
		if (!T[from][i].visited   && T[from][i].dist < minValue)
		{
			minNode = i;
			minValue = T[from][i].dist;
		}
	}
	return minNode;
}


//---------------------------------------------------------------------------
//display from all nodes' information
//include description, edge shorted distance and path
void GraphM::displayAll()const
{
	cout <<"Description"<< setw(20) << "From node" << setw(10) << "To node" 
		<< setw(15) << "Dijkstra's" <<setw(12)<<"Path"<< endl;
	for (int i = 0; i < size; i++)
	{
		cout << data[i] << endl; //display description
		for (int j = 0; j < size; j++)
		{
			if (i != j)
			{
				cout << setw(20)<<"";
				stack<int> path;
				displayPath(i, j,path);
			}
		}
	}
}


//display shortes distance and path between one pair of graph nodes
void GraphM::displayPath(int fromNode, int toNode, stack<int>& path)const
{
	displayDis(fromNode, toNode);
	if (T[fromNode][toNode].dist != INT_MAX) {
		cout << setw(16)<<fromNode + 1<<" " ;
		path.push(toNode);
		displayPathHelper(fromNode, toNode, path);
		cout << toNode + 1 << " ";
		path.push(fromNode);
	}
	cout << endl;
}

//displayPathHelper
//push the path nodein the stuck,  recursivly find the previous path node
//print the path node 
void GraphM::displayPathHelper(int fromNode, int toNode,
	stack<int>& path)const
{
	if (T[fromNode][toNode].path != 0)
	{
		path.push(T[fromNode][toNode].path);
		displayPathHelper(fromNode, T[fromNode][toNode].path, path);
		cout << T[fromNode][toNode].path + 1 << " ";
	}
}

//displayDisitance  Helper
//display from node, to node and the shortes distance
void GraphM::displayDis(int fromNode, int toNode)const
{
	cout << setw(5) << fromNode + 1;
	cout << setw(12) << toNode + 1;
	if (T[fromNode][toNode].dist == INT_MAX)
		cout << setw(12) << "---";
	else
		cout << setw(12) << T[fromNode][toNode].dist;
}




//---------------------------------------------------------------------------
//display shortes distance and path between one pair of graph nodes
//inculude the description of each path
//return false if parameter is invalid
bool GraphM::display(int fromNode, int toNode)const
{
	if ( fromNode <= 0 || fromNode>size || toNode <= 0 || toNode>size )
		return false;
	else
	{
		stack<int> path;
		displayPath(fromNode-1, toNode-1, path);
		displayPathDescription(path);
	}
}

//display the description of each path
void GraphM::displayPathDescription(stack<int>& path) const
{
	while (!path.empty())
	{
		cout << data[path.top()] << endl;
		path.pop();
	}
}