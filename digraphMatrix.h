#ifndef __DIGRAPH_MATRIX_H
#define __DIGRAPH_MATRIX_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

enum mark_t { UNDISCOVERED, DISCOVERED, EXPLORED };
class DigraphMatrix
{
private:
  unordered_map<int, int> components; // stores component ID
  vector<int> first_DFS, sec_DFS;
  vector<vector<int>> adjGraph, pairs;
  string fileName;
  int n, m; // n = number of vertices, m = number of edges

public:
  // Calls the functions neccesary to read the file and store the adj matrix and create the reversed adj matrix
  DigraphMatrix(const string& fileName);
  // Reads in digraph from given file
  string readFile();
  // Uses pair to construct the adjacency matrix adjGraph
  void adjacentGraph();
  // Function that reverses (or transposes) the adjacency graph
  void reverseGraph();
  // Gets the neighbors of a given vertex
  vector<int> getNeighbors(int vertex);
  // Recursively marks this vertex then explores neighbors
  void rDFS1(unordered_map<int, mark_t>& marks, int v);
  // Creates first_DFS which is the resulting array-based vector created after the first Depth First Search
  void DFS1();
  // Recursively calls this vertex then explores neighboring vertices a second time to get the component ID
  void rDFS2(unordered_map<int, mark_t>& marks, int v, int b);
  // Performs the second depth first search to find the strongly connected components
  void DFS2();

  vector<int> convertNum(string str1);
  int findStartPoint(vector<int> vec6);

  //Returns number of vertices
  int getOrder() const { return n; }

  // Returns vector containing component ID for each vertex
  vector<int> stronglyConnectedComponents() const;
};

#endif