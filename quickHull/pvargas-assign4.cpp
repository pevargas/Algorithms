////////////////////////////////////////////////////////////////////////////////
// File:   pvargas-assign4.cpp          1 Apr 2012
// Author: Patrick Vargas               (patrick.vargas@colorado.edu)
//
// Description
//------------------------------------------------------------------------------
//   This program is given a polygon and then calculates if it actually is a
// polygon, and if it is convex or not. To test if it is convex, the program 
// implements the QuickHull algorithm. (Levitin, Wikipedia & Mirxain). If it is
// convex, the program says so and asks for another file. If it is not, it then
// tests to make sure the polygon is closed, (closed meaning there is a clear
// inside and outside.) If it is not closed, the program says so. From there,
// the program will try and find an intersection, meaning it is a closed 
// polygon, but not a convex one.
//
// Invariant
//------------------------------------------------------------------------------
//   The file input is formated as follows: The vertices  appear first, then a
// dash (-) and then the edges. Each vertices/edge/dash appear on a line by 
// themselves. The vertices and edges are defined as follows: (#,#) where #
// is a single digit (0-9).
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
// 
// Vargas, P. (2012) pvargas-assign2.cpp. CSCI 3104: Algorithms. Boulder. CO: 
//   Unviversity of Colorado.
//
// Wikipedia. (2012) QuickHull. San Francisco, CA: Wikimedia Foundation, Inc.
//   Accessed 31 March 2012 (en.wikipedia.org/wiki/QuickHull)
//
// Mirzaian, A. (2007) Quick Hull Algorithm. Toronto, Ontario: York
//   University. Accessed 31 March 2012 (www.cse.yorku.ca/~aaw/Hang/quick_hull
//   /Algorithm.html)
// 
// Assitance from Haug, L. and Tuell, H.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Libraries
#include <cassert>  // Assert statements
#include <cctype>   // type checking
#include <cmath>    // For distance calculations
#include <cstdio>   // cin, cout
#include <cstdlib>  // EXIT_SUCCESS and things
#include <fstream>  // File Stream
#include <iostream> // printf() etc...
#include <stack>    // Stack container
#include <string>   // String class
#include <unistd.h> 
#include <queue>    // Queue Container
#include <vector>   // Vector Container

using namespace std;
ifstream input;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Structs and Such
// A vertex is a point on the Cartisian plane, defined with a x- and y-value.
struct vertex { double x, y; };

// Maximum number of points allowed
#define MAXPOINTS 10

// A vecter of vertexes
typedef vector<vertex> vset; 

// For debugging informaiton
bool verbose = false;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Declarations
// Makes sure there is at least two edges contected to the point in question.
// Pre:  'test' is the entire set of points in the polygon, 'adj' is the
//       adjacency matrix and 'point' is the index of the point in question.
// Post: Returns 'true' if the point has two or more edges connected, 'fasle' 
//       otherwise.
bool checkEdges(vset test, vector< vector<int> >& adj, int point);

// Calculates the perpendicular distance between a point and a line.
// Pre:  'a', 'b' and 'c' are coefficiants to a linear function and 'x' and 'y'
//       is the point in question.
// Post: The perpendiculaer distance from the point to the line is returned.
double dist(int a, int b, int c, int x, int y);

// Implements the main part of the quickHull algorithm. Using recursion, 
//   the function takes a line (defined between alpha and beta) and finds
//   the farthest point from the line. It then repeats with the line defined
//   between alpha and the farthest point. Once all the points are found, 
//   the convex hull is held within the vset hull.
// Pre:  'testSet' is the set of points to be analyzed, 'alpha' and 'beta' are
//       the endpoints of the line and 'hull' is the solution set.
// Post: The solution has been filled into the set 'hull'
void findHull(vset testSet, vertex alpha, vertex beta, vset& hull);

// Finds the point in the set that is farthest from the line. (most likely
//   on the convex hull.) 
// Pre:  'alpha' and 'beta' are the endpoints of a line and 'sample' is the
//       set to coose the point in.
// Post: The vertex of the point perpendicularly farthest from the line
//       alpha->beta.
vertex findThird(vset sample, vertex alpha, vertex beta);

// Inserts the vertex toAdd right after the addAfter vertex
// Pre:  Vertex 'toAdd' is a point to be added after the point 'addAfter' in
//       the set 'hull'.
// Post: 'toAdd' has been added to the right location after 'addAfter' in the
//       set 'hull'.
void insert(vset& hull, vertex addAfter, vertex toAdd);

// Finds a point if an intersection occurs.
// Pre:  'test' is the set of all points in the polygon and 'adj' is the 
//       adjacency matrix.
// Post: If an intersection is found, it is added to the avaible points,
//       the adjacency matrix is updated and returns true. Returns false
//       if unable to find an intersection.
bool intersect(vset& test, vector< vector<int> >& adj);

// Tests whether the polygon is a convex polygon
// Pre:  'test' is the polygon in question and 'adj' is the adjacency matrix 
//       for the polygon.
// Post: Returns true if the polygon is convex, 'false' otherwise.
bool isConvex(vset test, vector< vector<int> >& adj);

// Tests if the 'point' is actually on the line segments
// Pre:  'test' is all the points in the polygon, 'alpha' and 'beta' are
//       endpoints for one line and 'gamma' and 'delta' for another, and
//       'point' is the vertex in question.
// Post: Returns true if the point is within the polygon, on the line
//       alpha->beta and on the line gamma->delta. Returns false if 
//       any of those conditions fails.
bool isCorrect(vset test, vertex alpha, vertex beta, 
	       vertex gamma, vertex delta, vertex point);

// Checks if there is an edge missing from the actual polygon and the convex 
//   hull solution. This function also returns, thru the reference calls, the
//   two points where the edge is missing.
// Pre:  'first' and 'second' are empty, 'test' is the set of points for the 
//       polygon and 'adj' is the adjacency matrix.
// Post: Returns true if there is a descrepancy between the polygon and the
//       convex hull solution. The indicies for the endpoints where the line 
//       should appear are filled into 'first' and 'second'.
bool isMissingEdge(vset test, vector< vector<int> >& adj,
		   int& first, int& second);

// Tests for the 'exit' or 'quit' command.
// Pre:  A string with the user input is passed in.
// Post: If the command givin is equal to 'quit' or 'exit', not case sensitive,
//       the function returns true.
bool isQuit(const string& expr);

// Imports the polygon from a file and adds to a queue
// Pre:  'fp' is the file name where the polygon is stored.
// Post: The contents of the file are imported into the queue 'poly'.
bool loadPoly(string fp, queue<string>& poly);

// Used to find the farthest east (or largest) x vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest east point in the polygon (largest x-value)
int maxExtreme(vset points);

// Used to find the farthest west (or smallest) x vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest west point in the polygon (smallest x-value)
int minExtreme(vset points);

// Used to find the farthest north (or largest) y vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest north point in the polygon (largest y-value)
int maxExtremeY(vset points);

// Used to find the farthest south (or smallest) y vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest south point in the polygon (smallest y-value)
int minExtremeY(vset points);

// Prints Adjacency Matrix
// Pre:  Size of adjacency matrix is length by length.
void printAdj(int length, vector< vector<int> > adj);

// Create the adjacency matrix for the polygon and add the points to a vector
// Pre:  The 'points', 'adj', 'vertices' and 'edges' have not been assigned
//       values. The queue 'poly' is the unprocessed input from the file.
// Post: An adjacency matrix of the edges in the polygon, the number of edges
//       and verticies, and a set of all the points are filled in.
bool processInput(queue<string>& poly, vector< vector<int> >& adj,
		  vset& points, int& vertices, int& edges);

// Sets up the inital environment for the quickHull search
// Pre:  The set of all the vertices in the polygon.
// Post: A convex hull solution is returned.
vset quickHull (vset sample);

// Splits the set of points into two sets, one above the line and one below
// Pre:  'alpha' and 'beta' are the endpoints of a line and 'sample' is the 
//       available points to be sorted.
// Post: Returns a subset of 'sample' where all the points are to the left of
//       the directed line, alpha->beta.v
vset split(vertex alpha, vertex beta, vset sample);

// Prints out instructions on the use of command line options.
void usage();
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Main program
int main(int argc, char **argv) {
  char c;
  int vertices, edges;
  string command;
  stack<int> results;
  queue<string> poly;
  vector< vector<int> > adj;
  vset points;

  vertices = edges = 0;

  printf("Welcome to Torchwood, Division of Software!\n");

  // Parse the line
  while ((c = getopt(argc, argv, "v")) != EOF) {
    switch (c) {
    case 'v': // Print Diagnostic information.
      verbose = true;
      break;
    default:
      usage();
    }
  }
  if (verbose) { printf("Verbose turned on.\n"); }

  do {
    printf("Please enter the name of the input file: ");
    cin >> command;

    if (!isQuit(command)) {
      if (verbose) { printf("Didn't quit...\n"); }
      if (loadPoly(command, poly)) {
	if (verbose) { printf("Loaded poly...\n"); }
	if (processInput(poly, adj, points, vertices, edges)) {
	  
	  // Debugging information
	  if (verbose) { 
	    printf("Vertices = %i, Edges = %i\n", vertices, edges);

	    for (int i = 0; i < vertices; ++i) {
	      printf("Vertex: (%.2f, %.2f)\n", points[i].x, points[i].y);
	    }
	    printAdj(vertices, adj);
	    
	  }
	  
	  // Results
	  if (isConvex(points, adj))
	    printf("The figure is a convex polygon.\n");
	  else if (intersect(points, adj))
	    printf("The figure is a polygon, but it is non-convex.\n");
	  else
	    printf("The figure is not a polygon.\n");
	}
      }
    }
  }
  while(!isQuit(command));
  
  printf("Thank you!\n");
  
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions

////////////////////////////////////////////////////////////////////////////////
// Makes sure there is at least two edges contected to the point in question.
// Pre:  'test' is the entire set of points in the polygon, 'adj' is the
//       adjacency matrix and 'point' is the index of the point in question.
// Post: Returns 'true' if the point has two or more edges connected, 'fasle' 
//       otherwise.
bool checkEdges(vset test, vector< vector<int> >& adj, int point) {
  int count = 0;            // Number of Edges
  int length = test.size(); // Save Memeory Call

  for (int i = 0; i < length; ++i) {
    if (adj[point][i] == 1) { count++; }
  }
  if (count < 2) { return false; }

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Calculates the perpendicular distance between a point and a line.
// Pre:  'a', 'b' and 'c' are coefficiants to a linear function and 'x' and 'y'
//       is the point in question.
// Post: The perpendiculaer distance from the point to the line is returned.
double dist(int a, int b, int c, int x, int y) {
  double numer, denom;
 
  numer = abs(a*x + b*y - c);
  denom = sqrt(a*a + b*b);

  return numer/denom;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Implements the main part of the quickHull algorithm. Using recursion, 
//   the function takes a line (defined between alpha and beta) and finds
//   the farthest point from the line. It then repeats with the line defined
//   between alpha and the farthest point. Once all the points are found, 
//   the convex hull is held within the vset hull.
// Pre:  'testSet' is the set of points to be analyzed, 'alpha' and 'beta' are
//       the endpoints of the line and 'hull' is the solution set.
// Post: The solution has been filled into the set 'hull'
void findHull(vset testSet, vertex alpha, vertex beta, vset& hull) {
  vertex gamma;
  vset left, right;

  // Base case
  if (testSet.empty()) { return; }

  // Find the point farthest from the line and add to the convex hull
  gamma = findThird(testSet, alpha, beta);
  insert(hull, alpha, gamma);

  // Split into three sets, inside the triangle, to the left and to the right
  left = split(alpha, gamma, testSet);
  right = split(gamma, beta, testSet);

  // Recurse through the rest of the sets
  findHull(left, alpha, gamma, hull);
  findHull(right, gamma, beta, hull);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Finds the point in the set that is farthest from the line. (most likely
//   on the convex hull.) 
// Pre:  'alpha' and 'beta' are the endpoints of a line and 'sample' is the
//       set to coose the point in.
// Post: The vertex of the point perpendicularly farthest from the line
//       alpha->beta.
vertex findThird(vset sample, vertex alpha, vertex beta) {
  int a, b, c, length, max;
  double maxD, test;

  // Set length
  length = sample.size();
  
  // Find coefficiants
  a = beta.y - alpha.y;
  b = alpha.x - beta.x;
  c = alpha.x*beta.y - alpha.y*beta.x;
  
  max = 0;

  // Find third coordinate
  for (int i = 1; i < length; ++i) {
    maxD = dist(a, b, c, sample[max].x, sample[max].y);
    test = dist(a, b, c, sample[i].x, sample[i].y);
    if (maxD < test) { max = i; }
  }

  return sample[max];
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Inserts the vertex toAdd right after the addAfter vertex
// Pre:  Vertex 'toAdd' is a point to be added after the point 'addAfter' in
//       the set 'hull'.
// Post: 'toAdd' has been added to the right location after 'addAfter' in the
//       set 'hull'.
void insert(vset& hull, vertex addAfter, vertex toAdd) {
  vset temp;
  int length = hull.size();
  int i = 0;

  for(; (addAfter.x != hull[i].x) && (addAfter.y != hull[i].y); ++i)
    temp.push_back(hull[i]);

  temp.push_back(toAdd);

  for(; i < length; ++i)
    temp.push_back(hull[i]);

  hull = temp;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Finds a point if an intersection occurs.
// Pre:  'test' is the set of all points in the polygon and 'adj' is the 
//       adjacency matrix.
// Post: If an intersection is found, it is added to the avaible points,
//       the adjacency matrix is updated and returns true. Returns false
//       if unable to find an intersection.
bool intersect(vset& test, vector< vector<int> >& adj) {
  int first, second;                // The two indexes for the two points to
                                    // find an intersection
  int length = test.size();         // Save memory call
  double a[2], b[2], c[2];             // Coefficiants for the two lines
  double det;                       // The determinant
  vector<int> row;                  // Used to add edge
  vertex alpha, beta, gamma, delta; // The four points of the two lines
  vertex point;                     // The intersection  
  vector< vector<int> > temp;       // To add new edges
  
  if (verbose) { 
    printf("Entered intersect...\n");
    printf("length = %i\n", length);
  }
    
  // Find the missing edge
  if(isMissingEdge(test, adj, first, second)) {
    
    if (verbose)
      printf("Found missing edge...\n");
    
    // Check to see if there are more than one edge for each point.
    if (!checkEdges(test, adj, first)) { return false; }
    if (verbose)
      printf("Checked first point for multi-edges...\n");

    if (!checkEdges(test, adj, second)) { return false; }
    if (verbose)
      printf("Checked second point for multi-edges...\n");
    
    // Set first and second points
    alpha = test[first];
    gamma = test[second];

    // for-loop to test if multiple edges for the point in question
    for (int i = 0; i < length; ++i) {
      if (adj[first][i] == 1) {
	
	beta = test[i];
 
	if (verbose)
	  printf("Found first line...\n" \
		 "Alpha (%.2f, %.2f), Beta(%.2f, %.2f)\n", 
		 alpha.x, alpha.y, beta.x, beta.y);
	
	// Find coefficiants for first line
	a[0] = beta.y - alpha.y;
	b[0] = alpha.x - beta.x;
	c[0] = alpha.x*beta.y - alpha.y*beta.x;

	// for-loop to test if multiple edges for the point in question
	for (int j = 0; j < length; ++j) {
	  if (adj[second][j] == 1) {
	    delta = test[j];
	    
	    if (verbose)
	      printf("Found second line...\n" \
		     "Gamma (%.2f, %.2f), Delta(%.2f, %.2f)\n",
		     gamma.x, gamma.y, delta.x, delta.y);
	    
	    // Find coefficiants for second line
	    a[1] = gamma.y - delta.y;
	    b[1] = delta.x - gamma.x;
	    c[1] = delta.x*gamma.y - delta.y*gamma.x;
	    
	    // Determine the determinant of the two lines
	    det = a[0]*b[1] - a[1]*b[0];
	    
	    if (verbose)
	      printf("Found determinant (%.2f)...\n", det);

	    // If the determinant is parrallel, then it equals 0, and is not of
	    //   our concern. Also avoids divide by zero.
	    if (det != 0) {
	      // Calculate the point of intersection
	      point.x = (b[1]*c[0] - b[0]*c[1]) / det;
	      point.y = (a[0]*c[1] - a[1]*c[0]) / det;
	      
	      if (verbose)
		printf("Found intersection (%.2f, %.2f)\n", point.x, point.y);
	      // Make sure intersection is within graph space, 
	      //   otherwise, we have the wrong two lines
	      if (isCorrect(test, alpha, beta, gamma, delta, point)) {
		
		if (verbose)
		  printf("Is within parameters...\n" \
			 "length = %i\n", length);
		
		// Add point to polygon
		test.push_back(point);
		
		// Adjust length
		length = test.size();
		
		if (verbose)
		  printf("length = %i\n", length);

		if (length > MAXPOINTS) {
		  printf("Error: Exceeded maximum vertices. Good bye!\n");
		  exit(EXIT_FAILURE);
		}

		// Remove edges
		adj[first][i] = adj[i][first] = 0;
		adj[second][j] = adj[j][second] = 0;
		
		if (verbose)
		  printf("Removed edges...\n" \
			 "Made room for new edge...\n");
		
		// Add edge
		adj[first][length - 1] = adj[length - 1][first] = 1;
		adj[second][length - 1] = adj[length - 1][second] = 1;
		adj[i][length - 1] = adj[length - 1][i] = 1;
		adj[j][length - 1] = adj[length - 1][j] = 1;
		
		if (verbose) {
		  printAdj(length, adj);
		  printf("Added edge...\n");
		}
		return true;
	      }
	    }
	  }
	}
      }
    }
  }
  else { printf("Error in solution. Please try again.\n"); }

  return false;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests whether the polygon is a convex polygon
// Pre:  'test' is the polygon in question and 'adj' is the adjacency matrix 
//       for the polygon.
// Post: Returns true if the polygon is convex, 'false' otherwise.
bool isConvex(vset test, vector< vector<int> >& adj) {

  int first, second; // Not needed for this function, but needed for function
                     //   call isMissingEdge

  if (isMissingEdge(test, adj, first, second)) { return false; }

  if (verbose)
    printf("Exiting isConvex...\n");

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests if the 'point' is actually on the line segments
// Pre:  'test' is all the points in the polygon, 'alpha' and 'beta' are
//       endpoints for one line and 'gamma' and 'delta' for another, and
//       'point' is the vertex in question.
// Post: Returns true if the point is within the polygon, on the line
//       alpha->beta and on the line gamma->delta. Returns false if 
//       any of those conditions fails.
bool isCorrect(vset test, vertex alpha, vertex beta, 
	       vertex gamma, vertex delta, vertex point) {
  int north, east, south, west; // Make sure the intersection is within the
                                // two lines

  // Make sure point isn't one of the end points
  if (((point.x != alpha.x) && (point.y != alpha.y)) ||
      ((point.x != beta.x) && (point.y != beta.y)) ||
      ((point.x != gamma.x) && (point.y != gamma.y)) ||
      ((point.x != delta.x) && (point.y != delta.y))) {
    
    // Set perimeter of polygon to limit calculation
    north = test[maxExtremeY(test)].y;
    east = test[maxExtreme(test)].x;
    south = test[minExtremeY(test)].y;
    west = test[minExtreme(test)].x;
    
    // Test if point is within perimeter
    if ((point.y >= south) && (point.y <= north) && 
	(point.x >= west) && (point.x <= east)) {
      
      // Test if point is actually on existing line alpha->beta
      if (alpha.x > beta.x) {
	east = alpha.x;
	west = beta.x;
      }
      else {
	west = alpha.x;
	east = beta.x;
      }
      if (alpha.y > beta.y) {
	north = alpha.y;
	south = beta.y;
      }
      else {
	south = alpha.y;
	north = beta.y;
      }
      if ((point.y >= south) && (point.y <= north) && 
	  (point.x >= west) && (point.x <= east)) {
	
	// Test if point is actually on existing line gamma->delta
	if (gamma.x > delta.x) {
	  east = gamma.x;
	  west = delta.x;
	}
	else {
	  west = gamma.x;
	  east = delta.x;
	}
	if (gamma.y > delta.y) {
	  north = gamma.y;
	  south = delta.y;
	}
	else {
	  south = gamma.y;
	  north = delta.y;
	}
	if ((point.y >= south) && (point.y <= north) && 
	    (point.x >= west) && (point.x <= east)) { return true; }
      }
    } 
  }
  
  return false;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Checks if there is an edge missing from the actual polygon and the convex 
//   hull solution. This function also returns, thru the reference calls, the
//   two points where the edge is missing.
// Pre:  'first' and 'second' are empty, 'test' is the set of points for the 
//       polygon and 'adj' is the adjacency matrix.
// Post: Returns true if there is a descrepancy between the polygon and the
//       convex hull solution. The indicies for the endpoints where the line 
//       should appear are filled into 'first' and 'second'.
bool isMissingEdge(vset test, vector< vector<int> >& adj,
		   int& first, int& second) {

  if (verbose) { printf("Entered isConvex...\n"); }

  int hLen, len; // Used to save a memory call
  int i, j;      // Iterators
  vset hull;     // The convex hull solution

  if (verbose)
    printf("Entered isMissingEdge...\n");
  
  // Set values
  hull = quickHull(test);
  hLen = hull.size();
  len = test.size();
  
  if (verbose) {
    printf("Convex Hull:\n");
    for (unsigned int i = 0; i < hull.size(); ++i)
      printf("(%.2f, %.2f)\n", hull[i].x, hull[i].y);
  }
  
  if (verbose)
    printf("Found Convex Hull...\n");
  
  for (i = 0; i < hLen; ++i) {

    // Find the first point in the polygon that is part of the convex sol'n
    for (j = 0; j < len; ++j) {
      if ((hull[i].x == test[j].x) &&
	  (hull[i].y == test[j].y)) {
       		
	first = j;
	break;
      }
    }
    // Find the second point in the polygon that is part of the convex sol'n
    for (j = 0; j < len; ++j) {
      if ((hull[(i + 1)%hLen].x == test[j].x) &&
	  (hull[(i + 1)%hLen].y == test[j].y)) {
	
	second = j;
	break;
      } 
    }     
      
    // Test to see if there is an edge there
    if (verbose)
      printf("adj[%i][%i] = %i\n", first, second, adj[first][second]);
    
    if (adj[first][second] == 0) { 
      if (verbose)
	printf("Failed! first = %i, second = %i\n", first, second);
      return true; }
  }
  
  return false; 
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests for the 'exit' or 'quit' command.
// Pre:  A string with the user input is passed in.
// Post: If the command givin is equal to 'quit' or 'exit', not case sensitive,
//       the function returns true.
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
// Imports the polygon from a file and adds to a queue
// Pre:  'fp' is the file name where the polygon is stored.
// Post: The contents of the file are imported into the queue 'poly'.
bool loadPoly(string fp, queue<string>& poly) {
  string extract;

  if (isQuit(fp)) { return false; }

  input.open(fp.c_str());
  if (input.fail()) {
    printf("Error loading file <%s>. Please try again.\n", fp.c_str());
    return false;
  }

  while(input.good()) {
    input >> extract;
    poly.push(extract);
  }

  input.close();
  if (input.fail()) {
    printf("Error closing file <%s>. Please try again.\n", fp.c_str());
    return false;
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Used to find the farthest east (or largest) x vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest east point in the polygon (largest x-value)
int maxExtreme(vset points) {
  int max = 0, length = points.size();

  for(int i = 1; i < length; ++i) {
    if (points[max].x < points[i].x) { max = i; }
  }
  return max;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Used to find the farthest west (or smallest) x vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest west point in the polygon (smallest x-value)
int minExtreme(vset points) {
  int min = 0, length = points.size();

  for(int i = 1; i < length; ++i) { 
    if (points[min].x > points[i].x) { min = i; }
  }
  return min;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Used to find the farthest north (or largest) y vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest north point in the polygon (largest y-value)
int maxExtremeY(vset points) {
  int max = 0, length = points.size();

  for(int i = 1; i < length; ++i) {
    if (points[max].y < points[i].y) { max = i; }
  }
  return max;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Used to find the farthest south (or smallest) y vertex
// Pre:  The set of points in the polygon.
// Post: The index of the farthest south point in the polygon (smallest y-value)
int minExtremeY(vset points) {
  int min = 0, length = points.size();

  for(int i = 1; i < length; ++i) { 
    if (points[min].y > points[i].y) { min = i; }
  }
  return min;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prints Adjacency Matrix
// Pre:  Size of adjacency matrix is length by length.
void printAdj(int length, vector< vector<int> > adj) {
  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < length; ++j){
      printf("%i ", adj[i][j]);
    }
    printf("\n");
  }	        
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Create the adjacency matrix for the polygon and add the points to a vector
// Pre:  The 'points', 'adj', 'vertices' and 'edges' have not been assigned
//       values. The queue 'poly' is the unprocessed input from the file.
// Post: An adjacency matrix of the edges in the polygon, the number of edges
//       and verticies, and a set of all the points are filled in.
bool processInput(queue<string>& poly, vector< vector<int> >& adj,
		  vector<vertex>& points, int& vertices, int& edges) {
  int i;  // Iterator
  int x, y; // Used to store the integer digits
  int length; // Used to save a memory call
  vector<int> row;
  string temp;
  string tempx, tempy; // Used to store the asci digits
  bool passedDash = false;
  vertex extracted;

  // Make sure inputs are empty
  vertices = edges = 0;
  adj.clear();
  points.clear();

  // Create adjacency and points matrix while checking for bad input
  while(!poly.empty()) {  
    temp = poly.front();
    poly.pop();

    // Test to see if before or after the dash
    if (temp[0] == '-') {
      passedDash = true;

      // Zero out adjacency and points matrix
      length = points.size();
      for (i = 0; i < MAXPOINTS; ++i) { row.push_back(0); }
      for (i = 0; i < MAXPOINTS; ++i) { adj.push_back(row); }
    }
    else {
      // Make sure input is in form (#,#) where # is a single digit (0-9)
      if(!(isdigit(temp[1]) &&
	   isdigit(temp[3]))) {
	printf("Invalid character in graph.\n");
	return false;
      }
      
      // Convert the ascii digits to integers
      tempx = temp[1];
      tempy = temp[3];
      x = atoi(tempx.c_str());
      y = atoi(tempy.c_str());
      
      // If after the dash, create adjacency matrix of the edges
      if (passedDash) {
	adj[x][y] = 1;
	adj[y][x] = 1;
	edges++;
      }
      else { // If before the dash, add vertices to points vector
	extracted.x = x;
	extracted.y = y;
	points.push_back(extracted);
	vertices++;
      }
      
      // Reset varibles
      x = y = 0;
      temp.clear();
      tempx.clear();
      tempy.clear();
    }
  }

  return true;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Sets up the inital environment for the quickHull search
// Pre:  The set of all the vertices in the polygon.
// Post: A convex hull solution is returned.
vset quickHull (vset sample) {
  vset hull;        // The convex hull found
  vset left, right; // The set above the line created from the max/min line
  vertex max, min;  // The minimum and maximum points in the set

  if (verbose)
    printf("Entered quickHull...\n");

  // Find the extreme vertices
  max = sample[maxExtreme(sample)];
  min = sample[minExtreme(sample)];

  // Add the extreme vertices to the solution
  hull.push_back(max);
  hull.push_back(min);

  // Split the set in two
  left = split(min, max, sample);
  right = split(max, min, sample);

  // Begin split recursion
  findHull(left, min, max, hull);
  findHull(right, max, min, hull);

  if (verbose)
    printf("Exiting quickHull...\n");

  return hull;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Splits the set of points into two sets, one above the line and one below
// Pre:  'alpha' and 'beta' are the endpoints of a line and 'sample' is the 
//       available points to be sorted.
// Post: Returns a subset of 'sample' where all the points are to the left of
//       the directed line, alpha->beta.
vset split(vertex alpha, vertex beta, vset sample) {
  int a, b, c; // Used for linear algebra
  int length;  // Used to save a memory call
  vertex test; // The vertex we are testing
  vset above;

  // Set length
  length = sample.size();
  
  // Find coefficiants
  a = beta.y - alpha.y;
  b = alpha.x - beta.x;
  c = alpha.x*beta.y - alpha.y*beta.x;

  // Split above and below the line. Points on the line can be ignored, 
  //   (are not on the convex hull since they are between points.)
  for (int i = 0; i < length; ++i) {
    test = sample[i];
    if ((a*test.x + b*test.y - c) > 0) { above.push_back(test); }
  }

  return above;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prints out instructions on the use of command line options.
void usage() {
  printf("Add flag -v to turn verbose on.");
}
////////////////////////////////////////////////////////////////////////////////
