////////////////////////////////////////////////////////////////////////////////
// File:   pvargas-assign2.cpp          24 Feb 2012
// Author: Patrick Vargas               (patrick.vargas@colorado.edu)
//
// Description
//------------------------------------------------------------------------------
//   This program searches a graph, depth first. You may ask this program for 
//   a path within the graph between two vertices. If such a path exists, the
//   path is given to the user, other wise an error message is given. The user
//   may also request a path of a certain length, if it exists. If any path will
//   sufice, the user shall swipe 0 for the path length.
// 
// Invariant
//------------------------------------------------------------------------------
//   The program will ask for a valid file of a graph definition. The file will
//   begin with the total number of vertices in the graph, followed by a list
//   of pairs, in the form (#,#), where # is a digit. Each pair defines an edge.
//   There may be no more than ten (10) vertices.
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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Libraries
#include <cassert>  // Assert statements
#include <cctype>   // type checking
#include <cstdio>   // cin, cout
#include <cstdlib>  // EXIT_SUCCESS and things
#include <fstream>  // File Stream
#include <iostream> // printf() ect...
#include <stack>    // Stack container
#include <string>   // String class
#include <unistd.h> 
#include <queue>    // Queue Container
#include <vector>   // Vector Container

using namespace std;
ifstream input;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Declarations
// 
// Pre:  A stack, a vector, an adjacency matrix, and several other numbers
//       are given as input.
// Post: A path is found and loaded into the path stack, using the Depth First
//       Search algorithm.
void dfs(stack<int>& path, vector<int>& visited, vector< vector<int> >& adj, 
	 int st, const int& fi, const int& ln, const int& size);

// Pre:  A queue with the loaded graph, the adjacency matrix and the total 
//       number of verticies.
// Post: An adjacency matrix is created and stored in adj. If there is any
//       error what-so-ever, the function returns false.
bool createMatrix(queue<string>& graph, vector< vector<int> >& adj, int size);

// Pre:  The adjacency matrix, the start and end verticies, number of vertices
//       and the length of the path.
// Post: Function does all the overhead work for the dfs function. Function 
//       returns a stack with the path found.
stack<int> findPath(vector< vector<int> >& adj, int st, 
		const int& size, const int& fi, const int& ln);

// Pre:  The name of the input file, the number of vertices and a queue to
//       store the input.
// Post: The file is opened, the number of verticies are stored in size and
//       the input is stored in the queue. If anything what-so-ever goes wrong
//       the function returns false.
bool loadGraph(string fp, int& size, queue<string>& graph);

// Pre:  A string with the user input is passed in.
// Post: If the command givin is equal to 'quit' or 'exit', not case sensitive,
//       the function returns true.
bool isQuit(const string& expr);

// Pre:  An string expression is passed into this function.
// Post: If the expression is a number, ie only digits, the function returns
//       true.
bool isValid(const string& expr);

// Pre:  The adjacency matrix and the results stack.
// Post: The adjacency matrix is cleared, the input file is closed and the path
//       stack is is cleared.
void tidyUp(vector< vector<int> >& adj, stack<int>& results);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main() {

  int size;
  string command, dec, start, finish, length; 
  stack<int> results;
  queue<string> graph;
  vector< vector<int> > adj;
  int st, fi, ln;

  printf("Welcome to ACME Inc., Department of Software!\n");

  do {
    printf("Please enter the name of the input file: ");
    cin >> command;
    
    if (!isQuit(command)) {
      if(loadGraph(command, size, graph)) {
	if(createMatrix(graph, adj, size)) {
	  
	  // Recieve inputs
	  printf("Please enter the first vertex: ");
	  cin >> start;
	  assert(cin);

	  printf("And the second vertex: ");
	  cin >> finish;
	  assert(cin);

	  printf("And the length of the path: ");
	  cin >> length;
	  assert(cin);
	  
	  // Convert to numbers
	  st = atoi(start.c_str());
	  fi = atoi(finish.c_str());
	  ln = atoi(length.c_str());
	  
	  // Perform DFS
	  results = findPath(adj, st, size, fi, ln);
	  
	  // Report search
	  if(results.top() < 0) {
	    printf("Unable to find path. Please try again.\n");
	  }
	  else {
	    printf("Path Found: ");
	    while(!results.empty()) {
	      if(results.size() == 1) {
		printf("%i\n", results.top());
	      }
	      else { printf("%i, ", results.top());}
	      results.pop();
	    }
	  }
	}
      }
    }
    // Clean up
    tidyUp(adj, results);
  }
  while(!isQuit(command));
  
  printf("Thank you!\n");
  
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
bool createMatrix(queue<string>& graph, vector< vector<int> >& adj, int size) {
  stack<char> paren;
  string temp, tempx, tempy;
  int x, y;
  vector<int> row;

  // Zero out adjacency matrix
  for(int i = 0; i < size; ++i) {
    row.push_back(0);
  }
  for (int i = 0; i < size; ++i) {
    adj.push_back(row);
  }

  // Create adjacency matrix while checking for bad input
  while(!graph.empty()) {
    temp = graph.front();
    graph.pop();

    //printf("%s ", temp.c_str());
    
    if(!(isdigit(temp[1]) &&
	 isdigit(temp[3]))) {
      printf("Invalid character in graph.\n");
      return false;
    }

    tempx = temp[1];
    tempy = temp[3];
    x = atoi(tempx.c_str());
    y = atoi(tempy.c_str());

    //printf("%i %i\n", x, y);

    adj[x][y] = 1;
    adj[y][x] = 1;

    // Reset varibles
    x = y = 0;
    temp.clear();
    tempx.clear();
    tempy.clear();
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void dfs(stack<int>& path, vector<int>& visited, vector< vector<int> >& adj, 
	 int st, const int& fi, const int& ln, const int& size) {
  for(int i = 0; i < size; ++i) {
    visited[st] = 1;

    if(path.top() != fi) {
      if (ln == 0) {
	// Base Case
	if(adj[st][fi] == 1) {
	  path.push(fi);
	  return;
	}
	// Recursive call
	else if ((adj[st][i] == 1) && (visited[i] == 0)) {
	  path.push(i);
	  dfs(path, visited, adj, i, fi, ln, size);
	}
	// Dead end
	else if ((i == (size + 1)) && (!path.empty())) { path.pop(); }      
      }
      else {
	// Base case
	if ((adj[st][fi] == 1) && (int)(path.size() + 1) == ln) { 
	  path.push(fi);
	  return;
	}
	// Path too long
	else if ((int)(path.size() + 1) > ln) { 
	  path.pop();
	  break;
	}
	// Recursive call
	else if((adj[st][i] == 1) && (visited[i] == 0)) { 
	  path.push(i);
	  dfs(path, visited, adj, i, fi, ln, size);
	}
	// Dead end
	else if ((i == (size - 1)) && (!path.empty())) { path.pop(); }
      }
    }
    // Reached end
    else { return; }
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
stack<int> findPath(vector< vector<int> >& adj, int st, 
		const int& size, const int& fi, const int& ln) {
  vector<int> visited;
  stack<int> path, rev;
  
  for(int i = 0; i < size; ++i) {
    visited.push_back(0);
  }

  path.push(st);
  dfs(path, visited, adj, st, fi, ln, size);

  // Check for unavailible path
  if (ln != 0) {
    if ((int)path.size() != ln) { 
      rev.push(-1);
      return rev;
    }
  }
  else if (path.top() < 0) {
    rev.push(-1);
    return rev;
  }

  while(!path.empty()) {
    rev.push(path.top());
    path.pop();
  }
  
  return rev;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
bool isQuit(const string& expr) {
  if((toupper(expr[0]) == 'Q') &&
     (toupper(expr[1]) == 'U') &&
     (toupper(expr[2]) == 'I') &&
     (toupper(expr[3]) == 'T')) { return true; }
  if((toupper(expr[0]) == 'E') &&
     (toupper(expr[1]) == 'X') &&
     (toupper(expr[2]) == 'I') &&
     (toupper(expr[3]) == 'T')) { return true; }

  return false;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
bool isValid(const string& expr) {
  size_t size = expr.size();
    for(size_t i = 0; i < size; ++i) {
      if(!isdigit(expr[i])) { return false; }
    }
  
  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
bool loadGraph(string fp, int& size, queue<string>& graph) {
  string extract;

  if (isQuit(fp)) { return false; }

  input.open(fp.c_str());
  if (input.fail()) {
    printf("Error loading file <%s>. Please try again.\n", fp.c_str());
    return false;
  }

  input >> size;

  while(input.good()) {
    input >> extract;
    graph.push(extract);
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void tidyUp(vector< vector<int> >& adj, stack<int>& results) {
  // Close file
  input.close();
  // Clear Stack
  while(!results.empty()) { results.pop(); }
  // Empty adjacency matrix
  adj.clear();
}
////////////////////////////////////////////////////////////////////////////////
