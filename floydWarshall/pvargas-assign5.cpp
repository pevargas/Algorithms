////////////////////////////////////////////////////////////////////////////////
// File:   pvargas-assign5.cpp          14 Apr 2012
// Author: Patrick Vargas               (patrick.vargas@colorado.edu)
//
// Description
//------------------------------------------------------------------------------
//   This program implements both Floyd's and Warshall's algorithm for a
// weighted, directed graph. Floyd's algorithm tells us the shortest distance 
// between two given vertices (Levitin, pp.308-311). We calculate this as
// follows: for a givin edge between vertices i and j, if the path from i to
// some vertex k to j is less than the path from i to j, then return the
// distance i->k->j, else return path i->j.
//
//   This algorithm then tests each point in the graph to every pair of points. 
// Loops, or an edge with only one vertex, counts as a 0. For non-existant
// edges, so the case where there is no edge between two given vertices, we
// begin the algorithm with the weight of these edges at infinity. The
// algorithm will work and slowly replace the infinities. At the end, if there
// still is any infinities, we will replace with an 'X' to signify there is no
// possible path between these two edges.
//
//   To calculate the transitive matrix, or Warshall's algorithm, we can use the
// matrix given from Floyd's algorithm. We simply mark with a one if there is
// some distance between two points and if no distance or no possible path, a
// zero.
//
// Invariant
//------------------------------------------------------------------------------
//   The input file is formated as follows: The first line should be a
// non-negative integer representing the total number of vertices in the graph.
// Following this are the vertices and their weighted edges, one per line. The
// format is u,v,w where there is an edge of weight w from u to v, such that u,
// v, and w are all single digit, non-negative integers.
//
// References
//------------------------------------------------------------------------------
// Levitin, A. (2012) Introduction to the Design and Analysis of Algorithms,
//   3rd Ed. Upper Saddle River, NJ: Pearson Education, Inc.
//
// I/O file implementatoin from Savitch, W. (2009) Problem Solving with C++,
//   7th Ed. Boston, MA: Pearson Education, Inc.
//
// Line parsing for flags implementation from UCB Course CSCI 2400, Fall 2011.
//   and from GNU (gnu.org/software/libc/manual/html_node/Getopt.html)
// 
// Vargas, P. (2012) Assignment 1. CSCI 3104: Algorithms. Boulder, CO:
//   University of Colorado
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#include <cstdio>   // cin, cout
#include <cstdlib>  // EXIT_SUCCESS and things
#include <fstream>  // File I/O
#include <iostream> // printf() etc...
#include <queue>    // Queue Container
#include <string>   // String class
#include <unistd.h> // Line Parsing
#include <vector>   // Vector Containter

using namespace std;
ifstream input;

#define INF 100       // Iinfinity value for Floyd's Algorithm
bool verbose = false; // For Diagnostic information.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Declarations
// Finds the solution to the all-pairs, shortest-path problem, or Floyd's
//   algorithm (Levitin, pp.308-311).
// Pre:  The weighted, directed adjacency matrix and the number of vertices.
// Post: The distance matrix of the all-pairs, shortest-path problem.
vector< vector<int> > distFloyd(vector< vector<int> > adj, int size);

// Tests each number in vector, and checks each character is a digit.
//   Pre:  A string to be checked.
//   Post: Returns true if string is all digits; false otherwise.
bool isValid(const string &expr);

// Opens file and throws information into a queue to be processed.
// Pre:  The name of the input file.
// Post: The file is opened, the input is stored in the queue. The queue is
//       then returned. 
queue<string> loadGraph(string fp);

// Returns the smaller of the two integers.
int min(int a, int b) { if(a < b) return a; else return b; }

// Creates a size by size two-dimensional vector.
vector< vector<int> > newMatrix(int size);

// Interprets and prints the all-pairs, shortest-path distance matrix.
// Pre:  The all-pairs, shortest-path distance matrix given from the function,
//       distFloyd() and the total number of vertices
// Post: None.
void printDist(vector< vector<int> > dist, int size);

// Prints the matrix to the console.
// Pre:  Any size by size matrix.
// Post: None.
void printMatrix(vector< vector<int> > mat, int size);

// Same as printMatrix but formatted for verbose.
void printMatrixVerb(vector< vector<int> > mat, int size);

// Process the input from the file.
// Pre:  A queue containing all the information from the file and an empty
//       integer for the number of vertices.
// Post: The total number of vertices is stored in 'size' and returns a
//       weighted, directed adjacency matrix.
vector< vector<int> > processInput(queue<string> graph, int& size);

// Creates and returns the transitive matrix of a graph, or Warshall's
//   algorithm
// Pre:  The weighted, directed adjacency matrix and total number of vertices.
// Post: The transitive matrix.
vector< vector<int> > transWarshall(vector< vector<int> > adj, int size);

// Prints out instructions on the use of command line options.
void usage();
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  char c;                                 // Used for line parsing
  string fp;                              // Name of the input file
  int size;                               // Total number of vertices
  vector< vector<int> > adj, trans, dist; // Adjacency, Transitive and All-Pairs
                                          // Shortest-Path Distance matrices
 
  printf("Welcome to Warehouse 13, Department of Software!\n");

  // Parse the line
  while ((c = getopt(argc, argv, "hv")) != EOF) {
    switch (c) {
    case 'h': // Help message on command line options.
      usage();
      break;
    case 'v': // Print Diagnostic information.
      verbose = true;
      break;
    default:
      usage();
    }
  }
  if (verbose)
    printf("Verbose turned on.\n" \
	   "Normal Output\t\t\t\tVerbose Output\n"); 
  
  if (optind == argc) { // No command line arguments
    printf("Please enter the name of the input file: ");
    getline(cin, fp);
  }
  else { fp = argv[optind]; } // File given on command line

  // Imput file and process
  adj = processInput(loadGraph(fp), size);
  printf("Weighted, Directed Adjaceny Matrix\n");
  printMatrix(adj, size);

  // Print shortest distance matrix
  dist = distFloyd(adj, size);
  printf("Shortest-Distance Matrix\n");
  printDist(dist, size);

  // Print transitive matrix
  trans = transWarshall(adj, size);
  printf("Transitive Matrix\n");
  printMatrix(trans, size);
    
  printf("Good Bye!\n");
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
// Finds the solution to the all-pairs, shortest-path problem, or Floyd's
//   algorithm (Levitin, pp.308-311).
// Pre:  The weighted, directed adjacency matrix and the number of vertices.
// Post: The distance matrix of the all-pairs, shortest-path problem.
vector< vector<int> > distFloyd(vector< vector<int> > adj, int size) {
  vector< vector<int> > dist;  
  dist = newMatrix(size);

  // Set up first matrix in matrix
  for (int i = 0; i < size; ++i)
    for (int j = 0; j < size; ++j)
      if (adj[i][j] == 0) dist[i][j] = INF;
      else dist[i][j] = adj[i][j];
      
  if (verbose)
    printf("\t\t\t\t\tSet up first matrix in cube\n");

  // Floyd's Algorithm (Levitin p.310)
  for (int k = 0; k < size; ++k) {
    for (int i = 0; i < size; ++i)
      for (int j = 0; j < size; ++j)
	dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);	

    if (verbose)
      printMatrixVerb(dist, size);
  }

  return dist;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests each number in vector, and checks each character is a digit.
//   Pre:  A string to be checked.
//   Post: Returns true if string is all digits; false otherwise.
bool isValid(const string &expr) {
  char next;
  bool failed = false;

  for (unsigned int i = 0; !failed && (i < expr.length()); ++i) {
    next = expr[i];
    if (!(isdigit(next))) { failed = true; }
  }
  return !failed;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Opens file and throws information into a queue to be processed.
// Pre:  The name of the input file.
// Post: The file is opened, the input is stored in the queue and the queue is
//       then returned. 
queue<string> loadGraph(string fp) {
  queue<string> graph;
  string extract;

  input.open(fp.c_str());
  if (input.fail()) {
    printf("Error loading file <%s>. Good Bye!\n", fp.c_str());
    exit(EXIT_FAILURE);
  }

  while(input.good()) {
    input >> extract;
    graph.push(extract);
  }

  return graph;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Creates a size by size two-dimensional vector.
vector< vector<int> > newMatrix(int size) {
  vector< vector<int> > square;
  vector<int> line;
  
  for (int i = 0; i < size; ++i) { line.push_back(0); }
  for (int i = 0; i < size; ++i) { square.push_back(line); }

  return square;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Interprets and prints the all-pairs, shortest-path distance matrix.
// Pre:  The all-pairs, shortest-path distance matrix given from the function,
//       distFloyd() and the total number of vertices
// Post: None.
void printDist(vector< vector<int> > dist, int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j)
      if (i == j) printf(" 0 ");
      else if (dist[i][j] == INF) printf(" X ");
      else printf("%2i ", dist[i][j]);
    printf("\n");
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prints the matrix to the console.
// Pre:  Any size by size matrix.
// Post: None.
void printMatrix(vector< vector<int> > mat, int size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j)
      printf("%2i ", mat[i][j]);
    printf("\n");
  }
}
// Same as printMatrix but formatted for verbose.
void printMatrixVerb(vector< vector<int> > mat, int size) {
  for (int i = 0; i < size; ++i) {
    printf("\n\t\t\t\t\t");
    for (int j = 0; j < size; ++j)
      printf("%2i ", mat[i][j]);
  } 
  printf("\n");
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Process the input from the file.
// Pre:  A queue containing all the information from the file and an empty
//       integer for the number of vertices.
// Post: The total number of vertices is stored in 'size' and returns a
//       weighted, directed adjacency matrix.
vector< vector<int> >  processInput(queue<string> graph, int& size) {
  string temp, tempu, tempv, tempw;
  int u, v, w;
  vector<int> row;
  vector< vector<int> > adj;

  // Check for empty queue
  if(graph.empty()) {
    printf("Error processing input. Good bye!\n");
    exit(EXIT_FAILURE);
  }
  
  // Import number of vertices
  temp = graph.front();
  graph.pop();
  if(!isValid(temp)) {
    printf("First number is not the number of vertices. Good  bye! \n");
    exit(EXIT_FAILURE);
  }
  size = atoi(temp.c_str());
  if (verbose)
    printf("\t\t\t\t\tSize: %i\n", size);
  
  // Create new matrix
  adj = newMatrix(size);
    
  // Create adjacency matrix while checking for bad input
  while(!graph.empty()) {
    temp = graph.front();
    graph.pop();

    if (verbose)
      printf("\t\t\t\t\t%s \t", temp.c_str());
    
    if(!(isdigit(temp[0]) &&
	 isdigit(temp[2]) &&
	 isdigit(temp[4]))) {
      printf("Invalid character in graph. Good bye!\n");
      exit(EXIT_FAILURE);
    }

    // Extract information
    tempu = temp[0];
    tempv = temp[2];
    tempw = temp[4];
    u = atoi(tempu.c_str());
    v = atoi(tempv.c_str());
    w = atoi(tempw.c_str());

    if(verbose)
      printf("(%i, %i) :%i \n", u, v, w);

    // Set information
    adj[u][v] = w;
    
    // Reset varibles
    u = v = w = 0;
    temp.clear();
    tempu.clear();
    tempv.clear();
    tempw.clear();
  }

  return adj;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Creates and returns the transitive matrix of a graph, or Warshall's
//   algorithm
// Pre:  The weighted, directed adjacency matrix and total number of vertices.
// Post: The transitive matrix.
vector< vector<int> > transWarshall(vector< vector<int> > adj, int size) {
  vector< vector<int> > dist, trans;
  int temp; // Save memory call
  trans = newMatrix(size);
  dist = distFloyd(adj, size);
  
  for (int i = 0; i < size; ++i)
    for (int j = 0; j < size; ++j)
      if ((temp = dist[i][j]) == INF) trans[i][j] = 0;      
      else if (temp > 0) trans[i][j] = 1;
      else trans[i][j] = 0;
  
  return trans;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prints out instructions on the use of command line options.
void usage() {
  printf("Flags:\n"
	 "\t-h\t\tPrints this message.\n"
	 "\t-v\t\tPrint diagnostic info\n"
	 "<input_file_name> \tIf you would like to supply an input file name,\n"
	 "\t\t\tyou may do so here, otherwise, the program will ask for it.\n\n");
}
////////////////////////////////////////////////////////////////////////////////