#ifndef __DIGRAPH_MATRIX_H
#define __DIGRAPH_MATRIX_H

#include <string>
#include <vector>
using namespace std;

enum mark_t { UNDISCOVERED, DISCOVERED, EXPLORED };

class DigraphMatrix
{
private:
  int n;
  vector<bool> A;
public:
  //Reads in digraph from given file
  //Stores graph as an adjacency matrix
  DigraphMatrix(const string&);

  //Returns number of vertices
  int getOrder() const { return n; }

  //Returns vector containing component ID for each vertex
  vector<int> stronglyConnectedComponents() const;
};

#endif