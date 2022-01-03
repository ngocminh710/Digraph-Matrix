#include "digraphMatrix.h"
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <vector>

// Gets called by the driver when reading a new file
// Calls the functions neccesary to read the file and store the adj matrix and create the reversed adj matrix
DigraphMatrix::DigraphMatrix(const string& fileName) : fileName(fileName) 
{ 
    readFile();
    adjacentGraph(); 
    DFS1();
    reverseGraph();
    DFS2();
}

// lambda expression with parameter str1
// Converts a string of digits into a vector<int> and returns the vector
vector<int> DigraphMatrix::convertNum(string str1)
{
    vector<int> v;

    int i = 0;
    while(i < str1.size()) 
    {
        if(isdigit(str1[i]))
        {
            int val = 0;
            while(isdigit(str1[i])) // converts a string of digits into an int val
            {
                // Converts string to digit using ASCII representation then appends that to the value of val
                val = val * 10 + (str1[i] - '0');
                i++;
            }
            v.push_back(val); // adds int to vector
        }
        else
            i+=1;
    }
    return v;
}

// File constructor
// Reads in digraph from given file line by line
// Uses file to construct vector<vector<int>> pairs
// File format:
// n = # vertices m = # edges
// Next m lines: v1 v2
string DigraphMatrix::readFile() 
{
    ifstream inFS;
    string str = "", data = "";

    inFS.open(fileName);

    int j = 0;
    while(!inFS.eof()) 
    {
        getline(inFS, str); 
        if(!inFS.fail()) 
        {
            if(j == 0)
            {
                vector<int> vec1 = convertNum(str);
                n = vec1[0];
                m = vec1[1];
            }
            else if(j > 0)
            {
                vector<int> vec2 = convertNum(str);
                int c = vec2[0];
                int d = vec2[1];
                vector<int> vec3;
                vec3.push_back(c);
                vec3.push_back(d);
                pairs.push_back(vec3);
            }
            j+=1;
        }
    }

    inFS.close();
    return data;
}

// Uses vector<vector<int>> pairs to construct an adjacency matrix of the same type
void DigraphMatrix::adjacentGraph()
{
    // Creates temporary nested vector filled with zeros
    vector<vector<int>> temp(n, vector<int> (n, 0));

    // For each pair of vectors in pair turn that edge in the adjacency matrix to one
    for(auto x : pairs)
        temp[x[0]][x[1]] = 1;

    adjGraph = temp;
}

// Function that reverses (or transposes) the adjacency graph
void DigraphMatrix::reverseGraph()
{
    // Creates temporary nested vector filled with zeros
    vector<vector<int>> temp(n, vector<int> (n, 0));

    // For each edge changes the direction of that edge
	// Essentially transposes the adjacency matrix
    for(int i = 0; i < pairs.size(); i++)
        temp[pairs[i][1]][pairs[i][0]] = 1;
    
    adjGraph = temp;
}

// Gets the neighbors of a given vertex
vector<int> DigraphMatrix::getNeighbors(int vertex)
{
    vector<int> vec4;

    int i = 0;
    while(i < n) // for every vertex
    {
        // Goes to row in adj matrix and saves all vetices with an edge between them to a vector
        if(adjGraph[vertex][i] == 1)
            vec4.push_back(i);
        i+=1;
    }
    return vec4;
}

// Called by driver
// Returns a vector created from an array-based (vector-based) map that associates each vertex with its component ID
vector<int> DigraphMatrix::stronglyConnectedComponents() const
{
    vector<int> vec5(n, 0);
    
    // Iterate through the unordered_map to create an array-based vector of integers
    for(auto it = components.begin(); it != components.end(); it++)
        vec5[it->first] = it->second;

    return vec5;
}

// Recursively marks this vertex then explores neighbors
void DigraphMatrix::rDFS1(unordered_map<int, mark_t>& mark, int v)
{
    mark[v] = DISCOVERED;

    //Recurse on undiscovered neighbors
    for(int a : getNeighbors(v))
    {
        if(mark[a] == UNDISCOVERED)
            rDFS1(mark, a);
    }

    mark[v] = EXPLORED;
    first_DFS.push_back(v); // adds explored neighbors to adj list
}

int DigraphMatrix::findStartPoint(vector<int> vec6)
{
    // Create an adjancecy list from vec5 store it in h_map
    vector<int> h_map(n, 0);
    for(int i = 0; i < vec6.size(); i++)
    {
        h_map[vec6[i]] = 1;
    }

    // Finds next zero entry in h_map 
    int j = 0;    
    while(j < h_map.size())
    {
        if(h_map[j] == 0)
            return j;

        j+=1;
    }
}

// Creates first_DFS which is the resulting array-based vector created after the first Depth First Search
void DigraphMatrix::DFS1()
{
    // For every vertex create a map entry and mark as undiscovered
    unordered_map<int, mark_t> marks;
    for(int i = 0; i < n; i++)
        marks.insert(make_pair(i, UNDISCOVERED));

    // Calls the recursive function rDFS1 to explore the findStartPoint() neighbor
    while(first_DFS.size() < n)
    {
        int new_start = findStartPoint(first_DFS);
        rDFS1(marks, new_start);
    }          
    reverse(first_DFS.begin(), first_DFS.end()); // reverses the entries in the vector first_DFS
}

// Recursively calls this vertex then explores neighboring vertices a second time to get the component ID
void DigraphMatrix::rDFS2(unordered_map<int, mark_t>& mark, int v, int b)
{
    mark[v] = DISCOVERED;

    //Recurse on undiscovered neighbors
    for(int a : getNeighbors(v))
    {
        if(mark[a] == UNDISCOVERED)
            rDFS2(mark, a, b);
    }
        
    mark[v] = EXPLORED;
    sec_DFS.push_back(v);// adds explored neighbors to adj list
    components.insert(make_pair(v, b));// adds explored vertex to component ID
}

// Performs the second depth first search to find the strongly connected components
void DigraphMatrix::DFS2()
{
    // Creates an unordered_map with entires for all the vertices initially marked undiscovered
    unordered_map<int, mark_t> marks;
    int j = 0;
    while(j < n)
    {
        marks.insert(make_pair(j, UNDISCOVERED));
        j+=1;
    }
        
    int c_num = 0; // keeps track of depth i.e. connected components
    while(sec_DFS.size() < n)
    {
        int i = 0;
        while(i<first_DFS.size())
        {
            // Mark those in first_DFS that are not in sec_DFS
            auto it = find(sec_DFS.begin(), sec_DFS.end(), first_DFS[i]);
            if(it != sec_DFS.end())
                first_DFS[i] = -5;
            i+=1;
        }

        int k = 0;
        while(k < first_DFS.size())
        {
            // If the element in first_DFS is not found in the sec_DFS
			// then run rDFS2 on that element
            if(first_DFS[k] != -5)
            {
                rDFS2(marks, first_DFS[k], c_num);
                break;
            }  
            k+=1;
        }

        c_num+=1;
    }
}

