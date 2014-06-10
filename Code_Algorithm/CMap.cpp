
// File:  CMap.cpp
// Description: 
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>		// file operations
#include <string>		// needed for getline (ifstream, string)
#include <sstream>		// line tokenized
#include <ctime>		// elapse time calculations
#include <algorithm>	// find
#include <vector>		// container positions in the file

using namespace std;

#include "Globals.h"
#include "CNodeLocation.h"
#include "CMap.h"

// read and store the m_map topography details 
bool CMap::load (string filename)
{
	ifstream		inFile;
	inFile.open (filename.c_str ());
	if (!inFile.is_open()) {
		cerr << "Cannot open the input file" << endl;
		return false;
	}
	inFile >> m_width;	// read from the keyboard and store in member var m_width
	inFile >> m_height;	// and so on

	string line;

	getline(inFile, line); // skip the rest of the line

	setSize (m_width,m_height); 

	for (int j = 0;j < m_height ;j++) {
		getline(inFile, line);
		for (int i = 0;i < m_width ;i++) {
			string sdot;
			sdot = line[i];
			this->set (j, i, sdot);
		}
	}
	inFile.close ();
	return true;
}

// set the size and reserve the allocation in advance of the map
void CMap::setSize (int width, int height) { 
	m_width = width; 
	m_height = height;
	m_map.reserve (m_width * m_height);	// change the allocated space of the data to n * n elements
	for (int i = 0; i < m_width * m_height; i++) {
		m_map.push_back (0);	// initialize
	}
}

// set the value of the given point 
void CMap::set (int j, int i, string sdot) {	// set m_map[j,i] = sdot
	if (j < 0 || j >= m_height) return;
	if (i < 0 || i >= m_width) return;

	dot_t dot;
	dot = atoi (sdot.c_str ()); // convert to integer
		
	m_map[j * m_width + i] = dot;
}

// set the value of the given point but using the CNodeLocation class
void CMap::set (CNodeLocation loc, int dot)
{
	int j = loc.y; 
	int i = loc.x;

	if (j < 0 || j >= m_height) return;
	if (i < 0 || i >= m_width) return;

	m_map[j * m_width + i] = dot;
}

// return the value of the given point
int	CMap::get (int j, int i) {	// get m_map[j,i]
	if (j < 0 || j >= m_height) return MAP_WHT_EMPTY;
	if (i < 0 || i >= m_width) return MAP_WHT_EMPTY;
	return m_map[j * m_width + i];
}

// return the value of the given point but using the CNodeLocation class
int	CMap::get (CNodeLocation loc) {	// get m_map[j,i]
	int j = loc.y;
	int i = loc.x;
	if (j < 0 || j >= m_height) return MAP_WHT_EMPTY;
	if (i < 0 || i >= m_width) return MAP_WHT_EMPTY;
	return m_map[j * m_width + i];
}

// This methods build a path from the point bestloc to the goal. 
// It return true if there are no points obstructed between both points 
// (and return a vector with the points) and false otherwise.
bool CMap::visual (CNodeLocation bestloc, CNodeLocation goal, vector<CNodeLocation>& road)
{
	int i, steep = 0, sx, sy, dx, dy, e;
	int x1, y1, x2, y2;

	x1 = (int) bestloc.x;
	y1 = (int) bestloc.y;
	x2 = (int) goal.x;
	y2 = (int) goal.y;
		
	dx = ABS(x2 - x1);
	sx = ((x2 - x1) > 0) ? 1 : -1;
	dy = ABS(y2 - y1);
	sy = ((y2 - y1) > 0) ? 1 : -1;

	if (dy > dx) {
		steep = 1;
		x1 ^= y1;
		y1 ^= x1;
		x1 ^= y1;
		dx ^= dy;
		dy ^= dx;
		dx ^= dy;
		sx ^= sy;
		sy ^= sx;
		sx ^= sy;
	}

	e = (dy << 1) - dx;
	for (i = 0; i < dx; i++) {
		if (steep) {
			if (obstructed (y1, x1))
				return false;
			road.push_back (CNodeLocation(x1, y1));
		} else {
			if (obstructed (x1, y1))
				return false;
			road.push_back (CNodeLocation(y1, x1));
		}
		while (e >= 0) {
			y1 += sy;
			e -= (dx << 1);
		}
		x1 += sx;
		e += (dy << 1);
	}
	if (steep) {
		if (obstructed (y1, x1))
			return false;
		road.push_back (CNodeLocation(x1, y1));
	} else {
		if (obstructed (x1, y1))
			return false;
		road.push_back (CNodeLocation(y1, x1));
	}
	return true;
}


// This methods return true if the space indicated by x, y is obstructed false otherwise
bool CMap::obstructed (int x, int y)
{
	Assert (x < MAP_WDT, "obstructed: x out of range");
	Assert (y < MAP_HGT, "obstructed: y out of range");
	if (get (y, x) == MAP_WHT_WALL)
		debug ("1: AS obstructed (%02d, %02d)", y, x);
	return (get (y, x) == MAP_WHT_WALL);	// wall
}

// insert road in map
void CMap::insertRoad (vector<CNodeLocation> road)
{
	vector<CNodeLocation>::iterator p;

	for (p = road.begin (); p != road.end ();p++) {
		this->set ((*p), MAP_WHT_ROAD);
	}
}

// print the map

void  CMap::print (bool cls)
{
	changeColors (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, BACKGROUND_RED);
	cout << "  ";
	for (int i = 1;i <= m_width ;i++) {
		if (i && i % 10 == 0)
			cout << i / 10;
		else
			cout << " ";
	}
	cout  << endl << "  ";
	for (int i = 1;i <= m_width ;i++) {
		cout << i % 10;
	}
	cout  << endl;
	for (int j = 0;j < m_height ;j++) {
		changeColors (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, BACKGROUND_RED);
		int k = j + 1;
		if (k && k % 10 == 0)
			cout << k / 10;
		else
			cout << " ";
		cout << k % 10;
		for (int i = 0;i < m_width ;i++) {
			dot_t dot = this->get (j, i);
			string sdot;
			if (m_begin.x == i && m_begin.y == j) {
				sdot = "B";
				dot = 2;
			} else if (m_end.x == i && m_end.y == j) {
				sdot = "E";
				dot = 3;
			}
			switch (dot) {
			case 2: // begin
				changeColors (FOREGROUND_RED | FOREGROUND_GREEN, BACKGROUND_RED);
				break;
			case 3: // end
				changeColors (FOREGROUND_RED | FOREGROUND_GREEN, BACKGROUND_RED);
				break;
			case 1:	// wall
				sdot = "#";
				changeColors (FOREGROUND_RED, BACKGROUND_BLUE);
				break;
			case 4:	// road
				sdot = "@";
				changeColors (FOREGROUND_RED | FOREGROUND_GREEN, BACKGROUND_RED);
				break;
			case 5:	// candidate
				if (!cls) {
					sdot = "@";
					changeColors (FOREGROUND_GREEN, BACKGROUND_RED);
					break;
				}
			default:
			case 0:	// empty
				sdot = ".";
				changeColors (FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BLUE);
				break;
			}
			cout << sdot;
		}
		cout << endl;
	}
	changeColors (FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BLUE); // reset original colors
}


//+=====================================================+ 
//| POV Point of visibility                             |
//| This class is intended to handle the list           |
//| of interesting points to be used by the A* algorith |
//+=====================================================+

#define POV_MAXDIST			600.0f
#define TSC_POV_DEBUG		0

// This handle the set of neighbor points where neighbor means good candidates

vector<pov_vct_clss>::iterator CMap::find (CNodeLocation location)
{
	int	x, y;

	x = location.x;
	y = location.y;
	if (!sort_tbl.empty())
		sort_tbl.erase (sort_tbl.begin (), sort_tbl.end ());

	int bx, by, ex, ey;
	bx = x - 1; if (bx < 0) bx = 0;
	by = y - 1; if (by < 0) by = 0;
	ex = x + 1; if (bx >= m_width) bx = m_width - 1;
	ey = y + 1; if (by >= m_height) by = m_height - 1;
	for (int j = by; j <= ey; j++) {
		for (int i = bx; i <= ex; i++) {
			if (j == y && i == x)
				continue;
			pov_vct_clss pov_vct;
			pov_vct.m_loc.set (j, i);
			float f = tsc_manhattan_fdist (x, y, i, j); // put the list sorted by distance
			pov_vct.m_f    = f;
			sort_tbl.push_back (pov_vct);
		}
	}
	// sort by distance
	sort (sort_tbl.begin (), sort_tbl.end (), Compare_pov ());
	return sort_tbl.begin ();	// retorn the iterator to the first point
}


// Init the pov

void CMap::setnode (CNodeLocation location)
{
	// changing of node
	m_current_node = location;	
	m_pov_pt = this->find (location);	// search the adjacent points
	debug ("tsc_pov_setnode: leaving. => %s", (*m_pov_pt).m_loc.print ());
}

// Next point visible from bestnode. 
CNodeLocation& CMap::next (CNodeLocation bestnode_location)
{
	if (m_current_node != bestnode_location) {
		setnode (bestnode_location);
		return (*m_pov_pt).m_loc;	// setted in setnode ()
	} else {
		m_pov_pt++;	// next pov from this node
		if (m_pov_pt != this->sort_tbl.end ()) {
			debug ("CPov::next: selected => %s", (*m_pov_pt).m_loc.print ());
			return (*m_pov_pt).m_loc;
		}
	}
	debug ("tsc_pov_next: empty");
	return (NULLLOCATION);
}
