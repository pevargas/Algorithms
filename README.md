Algorithms
==========

A repository containing my implementations of the algorithims studied in CSCI 3104: Algorithms.

Directories
-----------

###bubbleSort  
This program applys the bubble sort algorithm to a file of integers. The bubble sort algorithm, given by Levitin (2012), is handled in the funciton bubbleSort() (p 100). 

The list of integers is held within a vector. The algorthim first compares the the first to elements. If  i+1 > i, then a swap is made. The function goes on to compare each pair until the end. Once the end is reached, the same procedure is applied again, going to n - 1, where n = number of elements in the vector.

###depthFirstSearch

This program searches a graph, depth first. You may ask this program for a path within the graph between two vertices. If such a path exists, the path is given to the user, other wise an error message is given. The user may also request a path of a certain length, if it exists. If any path will sufice, the user shall swipe 0 for the path length.

###floydWarshall

This program implements both Floyd's and Warshall's algorithm for a weighted, directed graph. Floyd's algorithm tells us the shortest distance between two given vertices (Levitin, pp.308-311). We calculate this as follows: for a givin edge between vertices i and j, if the path from i to some vertex k to j is less than the path from i to j, then return the distance i->k->j, else return path i->j.

This algorithm then tests each point in the graph to every pair of points. Loops, or an edge with only one vertex, counts as a 0. For non-existant edges, so the case where there is no edge between two given vertices, we begin the algorithm with the weight of these edges at infinity. The algorithm will work and slowly replace the infinities. At the end, if there still is any infinities, we will replace with an 'X' to signify there is no possible path between these two edges.

To calculate the transitive matrix, or Warshall's algorithm, we can use the matrix given from Floyd's algorithm. We simply mark with a one if there is some distance between two points and if no distance or no possible path, a zero.

###mergeSort

This program applies the merge sort algorithm to a file of integers. The merge sort algorithm, given by Levitin (2012), is handled in the funciton mergeSort() and merge() (p 172). 

The list of integers is held within a vector. Thru the method of the divide-and-conquor technique, the mergeSort() function first splits the list in two and passes each half into the mergeSort() function again. This continues until each half contains only one item.

From there, the merge() function takes over. This function compares the the two halves, and writes them to the original vector in the sorted order.

###quickHull
This program is given a polygon and then calculates if it actually is a polygon, and if it is convex or not. To test if it is convex, the program implements the QuickHull algorithm. (Levitin, Wikipedia & Mirxain). If it is convex, the program says so and asks for another file. If it is not, it then tests to make sure the polygon is closed, (closed meaning there is a clear inside and outside.) If it is not closed, the program says so. From there, the program will try and find an intersection, meaning it is a closed polygon, but not a convex one.

References
----------
Levitin, A. (2012) Introduction to the Design and Analysis of Algorithms, 3rd Ed. Upper Saddle River, NJ: Pearson Education, Inc.

I/O file implementatoin from Savitch, W. (2009) Problem Solving with C++, 7th Ed. Boston, MA: Pearson Education, Inc.

Line parsing for flags implementation from UCB Course CSCI 2400, Fall 2011. and from GNU [gnu.org/software/libc/manual/html_node/Getopt.html] (gnu.org/software/libc/manual/html_node/Getopt.html)

Wikipedia. (2012) QuickHull. San Francisco, CA: Wikimedia Foundation, Inc. Accessed 31 March 2012 [en.wikipedia.org/wiki/QuickHull] (en.wikipedia.org/wiki/QuickHull)

Mirzaian, A. (2007) Quick Hull Algorithm. Toronto, Ontario: York University. Accessed 31 March 2012 [www.cse.yorku.ca/~aaw/Hang/quick_hull/Algorithm.html] (www.cse.yorku.ca/~aaw/Hang/quick_hull/Algorithm.html)
