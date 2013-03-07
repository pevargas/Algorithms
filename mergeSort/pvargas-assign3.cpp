////////////////////////////////////////////////////////////////////////////////
// File:   pvargas-assign3.cpp          4 Mar 2012
// Author: Patrick Vargas               (patrick.vargas@colorado.edu)
//
// Description
//------------------------------------------------------------------------------
//     This program applies the merge sort algorithm to a file of integers. The 
//   merge sort algorithm, given by Levitin (2012), is handled in the funciton
//   mergeSort() and merge() (p 172). 
//
//     The list of integers is held within a vector. Thru the method of
//   the divide-and-conquor technique, the mergeSort() function
//   first splits the list in two and passes each half into the mergeSort()
//   function again. This continues until each half contains only one item.
//   
//     From there, the merge() function takes over. This function compares the
//   the two halves, and writes them to the original vector in the sorted order.
//
// Invariant
//------------------------------------------------------------------------------
//     The input to this program is a file name of a list of integers,
//   given one line per entry, as a .txt file. The file name may be given on
//   the command line as an argument; otherwise the user will be prompted to
//   provide one. Each number should be an integer, containing only digits,
//   otherwise the program will terminate.
//
// References
//------------------------------------------------------------------------------
// I/O file implementatoin from Savitch, W. (2009) Problem Solving with C++,
//   7th Ed. Boston, MA: Pearson Education, Inc.
//
// Levitin, A. (2012) Introduction to the Design and Analysis of Algorithms,
//   3rd Ed. Upper Saddle River, NJ: Pearson Education, Inc.
//
// Line parsing for flags implementation from UCB Course CSCI 2400. (2011)
//   and from GNU (gnu.org/software/libc/manual/html_node/Getopt.html)
//
// Vargas, P. (2012) Assignment 1. CSCI 3104 Algorithms. Univertsity of
//   Colorado at Boulder. Boulder, CO.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;
ifstream input;
ofstream output;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Declarations
//
// Open file and import list into a vector.
//   Pre:  The name of the file containing the list of numbers to be sorted.
//   Post: A vector of the newly imported list of numbers.
vector<int> importFile(string file_name);

// Tests each number in vector, and checks each character is a digit.
//   Pre:  A string to be checked.
//   Post: Returns true if string is all digits; false otherwise.
bool isValid(const string& expr);

// Sorts the vector master recursivly using the merge sort algorithm.
//   Pre:  A vector of ints.
//   Post: A vector of sorted ints in ascending order.
void mergeSort(vector<int>& master);

// Merges two sorted vectors into one.
//   Pre:  Two vectors (bot and top) each sorted.
//   Post: The final sorted vector (master) containing the other two vectors.
void merge(vector<int>& master, vector<int>& bot, vector<int>& top);

// Prints out instructions on the use of command line options.
void usage();
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  char c;               // Used for line parsing
  bool verbose = false; // For Diagnostic information.
  string file_name;     // Input file name
  int length = 0;       // Initialize length of list
  vector<int> master;   // Vector to store input

  printf("Welcome to Global Dynamics, Software Divison!\n");

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
  if (verbose) { printf("Verbose turned on.\n"); }

  if (optind == argc) { // No command line arguments
    printf("Please enter the name of the input file: ");
    getline(cin, file_name);
  }
  else { // File given on command line
    file_name = argv[optind];
  }

  // Imput file into a vector
  master = importFile(file_name);

  output.open("output.txt");
  if (output.fail()) {
    printf("Output file opening failed.\nGood Bye!\n");
    exit(EXIT_FAILURE);
  }
 
  // Output file name
  if (verbose) { cout << "File Name:\t" << file_name << "\n"; }
  output << "File Name:\t" << file_name << "\n";

  // Output length
  length = master.size();
  if (verbose) { printf("Length:\t\t%i\n", length); }
  output << "Length:\t\t" << length << "\n";

  // Merge sort list
  mergeSort(master);

  // Output sorted list
  for (int i = 0; i < length; ++i) {
    if (verbose) { printf("%i\n", master[i]); }
    output << master[i] << endl;

    // Test if list is actually sorted
    if(i != (length - 1)) {
      if(master[i] > master[i + 1]) {
	printf("Sorting algorithm failed!\n");
	break;
      }
    }
  }

  // Close file
  output.close();
  printf("Good Bye!\n");
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
//
// Open file and import list into a vector.
//   Pre:  The name of the file containing the list of numbers to be sorted.
//   Post: A vector of the newly imported list of numbers.
vector<int> importFile(string file_name) {
  vector<int> newList;
  string number;

  // Establish connection to input file
  input.open(file_name.c_str());
  if (input.fail()) {
    printf("Unable to open \"%s\".\nGood Bye!\n", file_name.c_str());
    exit(EXIT_FAILURE);
  }
  output << file_name << "\n";

  // Import numbers into a vector
  while (input.good()) {
    input >> number;
    
    if (!isValid(number)) { 
      printf("File contains invalid characters.\n"
	      "Good Bye!\n");
      exit(EXIT_FAILURE);
    }
    newList.push_back(atoi(number.c_str()));
  }
  input.close();

  return newList;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Tests each number in vector, and checks each character is a digit.
//   Pre:  A string to be checked.
//   Post: Returns true if string is all digits; false otherwise.
bool isValid(const string& expr) {
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
// Sorts the vector master recursivly using the merge sort algorithm.
//   Pre:  A vector of ints.
//   Post: A vector of sorted ints in ascending order.
void mergeSort(vector<int>& master) {
  int i;
  int n = master.size();
  vector<int> top; // First (top) half
  vector<int> bot; // Last (bottom) half
  
  if (n > 1) {
    // Split list in two parts
    for (i = 0; i < (n/2); ++i) { bot.push_back(master[i]); }
    for (; i < n; ++i) { top.push_back(master[i]); } 
    
    // Recursivly split list
    mergeSort(bot);
    mergeSort(top);
    
    // Join the split lists
    merge(master, bot, top);
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Merges two sorted vectors into one.
//   Pre:  Two vectors (bot and top) each sorted.
//   Post: The final sorted vector (master) containing the other two vectors.
void merge(vector<int>& master, vector<int>& bot, vector<int>& top) {
  int i, j, k, p, q;
  i = j = k = 0;
  p = bot.size();
  q = top.size();

  while ((i < p) && (j < q)) {
    // Sort list
    if (bot[i] <= top[j]) { master[k++] = bot[i++]; }
    else { master[k++] = top[j++]; }
  }

  // Finish the lists
  if (i == p) { for (; j < q; ++j) { master[k++] = top[j]; } }
  else { for (; i < p; ++i) { master[k++] = bot[i]; } }
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
