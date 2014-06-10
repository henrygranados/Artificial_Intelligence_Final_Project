
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "Globals.h"
#include "CMap.h"
#include "CAStar.h"
#include "PriorityQueue.h"
#include "LLQueue.h"

void getInput (string &line, string prompt);

int main(int argc, char* argv[])
{
	string filename;
	filename = "map1.txt";					// default map input file
	int sy = 18, sx = 5, ey = 5, ex = 3;	// default starting and ending point
	bool	animate_flag = false;			// animate the evelution of the path finding
	bool	priorityqueue_flag = false;		// set the path finding to use a priority queue
	bool	print_flag = false;				// print the map and exit

	// Do we have enough command line arguments?
    if (argc >= 2) {
		int i = 1;
		while (i < argc) {
			// parse input file name
			if (strncmp (argv[i],	"-f", 2) == 0 || strncmp (argv[i],	"--file", 6) == 0) {
				i++;
				filename = argv[i];
				if (!validateFileName (filename, "Input file not found")) {
					return 1;
				}
				i++;
				continue;
			}
			// parse starting point
			if (strncmp (argv[i],	"-s", 2) == 0) {
				i++;
				if (i >= argc ||  *argv[i] < '0' || *argv[i] > '9') {
					cout << "Invalid starting row number" << endl;
					return 1;
				}
				sy = atoi (argv[i]);
				sy--; // put it 0 based, as the C++ array indexes
				i++;
				if (i >= argc ||  *argv[i] < '0' || *argv[i] > '9') {
					cout << "Invalid starting col number" << endl;
					return 1;
				}
				sx = atoi (argv[i]);
				sx--; // put it 0 based, as the C++ array indexes
				i++;
				continue;
			}
			// parse ending point
			if (strncmp (argv[i],	"-e", 2) == 0) {
				i++;
				if (i >= argc ||  *argv[i] < '0' || *argv[i] > '9') {
					cout << "Invalid ending row number" << endl;
					return 1;
				}
				ey = atoi (argv[i]);
				ey--; // put it 0 based, as the C++ array indexes
				i++;
				if (i >= argc ||  *argv[i] < '0' || *argv[i] > '9') {
					cout << "Invalid ending col number" << endl;
					return 1;
				}
				ex = atoi (argv[i]);
				ex--; // put it 0 based, as the C++ array indexes
				i++;
				continue;
			}
			if (strncmp (argv[i],	"-a", 2) == 0) {
				animate_flag = true;
				i++;
				continue;
			}
			if (strncmp (argv[i],	"-print", 6) == 0) {
				print_flag = true;
				i++;
				continue;
			}
			if (strncmp (argv[i],	"-p", 2) == 0) {
				priorityqueue_flag = true;
				i++;
				continue;
			}
			if (strncmp (argv[i],	"-h", 2) == 0 || strncmp (argv[i],	"--help", 6) == 0) {
				cout << "USAGE: main -f inputFile -h -s sx sy -e ex ey" << endl;
				cout << "-h\tPrint this help" << endl;
				cout << "-f\tselect the specified input file. Default map1.txt" << endl;
				cout << "-s\tInsert the starting point, it waits 2 numbers white spaced." << endl;
				cout << "\tfirst number is the row, second number is the column" << endl;
				cout << "\tFor example: -s 18 5" << endl;
				cout << "-e\tInsert the ending point, it waits 2 numbers white spaced. " << endl;
				cout << "\tfirst number is the row, second number is the column" << endl;
				cout << "\tFor example: -e 18 5" << endl;
				cout << "-p\tSet priority queue as the system to handle the queue of nodes" << endl;
				cout << "-print\tPrint the map and exit" << endl;
				return 0;
			}
		}
    }
	CMap map;
	CAStar as (&map);
	as.animate (animate_flag);			// set the animation status true or false
	CNodeLocation	begin(sy, sx);	// 0 based, row, col
	CNodeLocation	end(ey, ex);	// 0 based, row, col
	//getInput (filename, "Enter the filename with the map details");
	if (!map.load (filename))
		return (1);
	
	if (print_flag) {
		map.print (true);	// print the map and exit
		return (0);
	}
	// define which prioty queue will be used, queue using a STL heap or a linked list
	if (priorityqueue_flag) {
		as.m_openlist = new PriorityQueue;
	} else { // LLQueue is the default
		as.m_openlist = new LLQueue;
	}

	map.setBegin (begin);
	map.setEnd (end);

	unsigned int startt, endt, elapsed;
	elapsed = 0;

	cls (); // clear the screen
	for (int i = 0;i < 1;i++) {
		as.reset ();
		moveCursor (0,0);
		map.print (false);
		startt = tsc_get_time ();
		if (as.FindPath (begin, end)) {
			map.insertRoad (as.getRoad ());
		}
		endt = tsc_get_time ();
		moveCursor (0,0);
		map.print (true);
		elapsed += endt - startt;
	}
	
	cout << "Elapsed time: " << elapsed;
	return (0);
}


/*
 * Name:		getInput
 * Propose:		get a input line from user
 * Parameter:	line:		the string line passed by reference
 *				prompt:		The prompt to be displayed to the user explaing what should be entered
 */
void getInput (string &line, string prompt)
{
	bool quit = false;	// we going to change to true when we have what we need, the input line correctly reads
	while (!quit) {		// while not quit
		cout << prompt << ": ";
		getline(cin, line);	// read from the keyboard and store in the string 'line'
		// parse the input
		if (line.empty ()) {	// check if the line has some text
			cout << "Invalid input, try again" << endl;
			continue;			// continue => read again the input
		}
		quit = true;	// success we have the correct input
	}
}
