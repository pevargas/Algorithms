////////////////////////////////////////////////////////////////////////////////
// File:   pvargas-assign1.cpp          10 Feb 2012
// Author: Patrick Vargas               (patrick.vargas@colorado.edu)
//
// Description
//------------------------------------------------------------------------------
//     This program applys the bubble sort algorithm to a file of integers. The 
//   bubble sort algorithm, given by Levitin (2012), is handled in the funciton
//   bubbleSort() (p 100). 
//
//     The list of integers is held within a vector. The algorthim first 
//   compares the the first to elements. If  i+1 > i, then a swap is made. 
//   The function goes on to compare each pair until the end. Once the end is 
//   reached, the same procedure is applied again, going to n - 1, where 
//   n = number of elements in the vector.
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
// Global variables for outputting information
int compare = 0; // Number of comparisons made
int swaps = 0;   // Number of swaps performed
int length = 0;  // Size of List
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Declarations
//
// Sorts vector using a bubble sort implementaion, as described above.
//   Pre:  A vector of ints.
//   Post: A vector of ints, sorted in ascending order.
void BubbleSort(vector<int> &numbers);

// Open file and import list into a vector.
//   Pre:  The name of the file containing the list of numbers to be sorted.
//   Post: A vector of the newly imported list of numbers.
vector<int> importFile(string file_name);

// Tests each number in vector, and checks each character is a digit.
//   Pre:  A string to be checked.
//   Post: Returns true if string is all digits; false otherwise.
bool isValid(const string &expr);

// Prints out instructions on the use of command line options.
void usage();
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  char c;               // Used for line parsing
  bool verbose = false; // For Diagnostic information.
  string file_name;
  vector<int> master;

  printf("Welcome to Aperature Laboratories, Department of Software!\n");

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

  // Sort the list
  BubbleSort(master);

  // Output information of the sort.
  if (verbose) { printf("Compares:\t%i\nSwaps:\t\t%i\n\n", compare, swaps); }
  output << "Compares:\t" << compare << "\n"
	 << "Swaps:\t\t" << swaps << "\n\n";

  for (int i = 0; i < length; ++i) {
    if (verbose) { printf("%i\n", master[i]); }
    output << master[i] << endl;
  }

  output.close();
  printf("Good Bye!\n");
  return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
//
// Sorts vector using a bubble sort implementaion, as described above.
//   Pre:  A vector of ints.
//   Post: A vector of ints, sorted in ascending order.
void BubbleSort(vector<int> &numbers) {
  int temp;              // For swap
  bool made_swap = true; // For unnecessary compares when algorithm finishes

  for (int i = 0; (i < (length - 1)) && (made_swap); ++i) {
    made_swap = false;
    for (int j = 0; j < (length - 1 - i); ++j) {
      ++compare;
      if (numbers[j + 1] < numbers[j]) { 
	temp = numbers[j];
	numbers[j] = numbers[j + 1];
	numbers[j + 1] = temp;
	++swaps;
	made_swap = true;
      }
    }
  }
}

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

// Prints out instructions on the use of command line options.
void usage() {
  printf("Flags:\n"
	 "\t-h\t\tPrints this message.\n"
	 "\t-v\t\tPrint diagnostic info\n"
	 "<input_file_name> \tIf you would like to supply an input file name,\n"
	 "\t\t\tyou may do so here, otherwise, the program will ask for it.\n\n");
}
////////////////////////////////////////////////////////////////////////////////
