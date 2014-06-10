
#ifndef _CMAP_H_
#define _CMAP_H_

#include <vector>
using namespace std;

#include "CNodeLocation.h"

#define MAP_WDT		70
#define MAP_HGT		70
#define MAP_SQ		1		// map square size, in case each square must be bigger because graphics, ie tile size
#define MAP_WHT_WALL	1		
#define MAP_WHT_EMPTY	0
#define MAP_WHT_ROAD	4
#define MAP_WHT_CAND	5

typedef int dot_t;

//+=====================================================+ 
//| POV Point of visibility                             |
//| This class is intended to handle the list           |
//| of interesting points to be used by the A* algorith |
//+=====================================================+

// sort de los povs

class pov_vct_clss {
public:
	float			m_f;			// distance to current point
	CNodeLocation	m_loc;			// to this point
	pov_vct_clss &operator= (pov_vct_clss a)	// Assignment
	{
		m_f		= a.m_f; 
		m_loc	= a.m_loc;
		return *this;
	};
};

class Compare_pov {
public:
	//This is required for STL to sort the vector
   //(its entered as an argument in the STL sort functions)
   bool operator() (pov_vct_clss first, pov_vct_clss second) const {
      return first.m_f < second.m_f;
   }
};



class CMap {
	vector<dot_t>	m_map;
	int				m_width, m_height;
	CNodeLocation	m_begin, m_end;
	// pov
	CNodeLocation	m_current_node;		// current node been analized
	
public:	
	CMap() {	// null constructor
		m_width = m_height = 0;
	};

	~CMap() {
	};
	bool	load (string filename);							// read and store the map topography details 
	bool	obstructed (int x, int y);
	// This methods build a path from the point bestloc to the goal. 
	// It return true if there are no points obstructed between both points 
	// (and return a vector with the points) and false otherwise.
	bool	visual (CNodeLocation bestloc, CNodeLocation goal, vector<CNodeLocation>& road);

	// map handling
	// accessors
	int		getWidth (void)  { return m_width; };
	int		getHeight (void) { return m_height; };
	void	setSize (int width, int height);
	void	set (int j, int i, string sdot);
	void	set (CNodeLocation loc, int dot);
	int		get (int j, int i);
	int		get (CNodeLocation loc);
	void	begin (int j, int i) { m_begin.set (j-1, i-1); };
	void	end (int j, int i) { m_end.set (j-1, i-1); };
	void	setBegin (CNodeLocation loc) { m_begin = loc; };
	void	setEnd (CNodeLocation loc) { m_end = loc; };

	// insert road in map
	void	insertRoad (vector<CNodeLocation> road);
	// display map

	void	print (bool cls);

	
	// pov
	// These methods handle the pov, the point adjacent to the given location
	// setnode set the current location to be analized.
	void					setnode (CNodeLocation location);
	// find and store in the vector sort_tbl the adjacent points
	vector<pov_vct_clss>::iterator find (CNodeLocation location); 
	// return the next best position in the current set of adjacent points
	CNodeLocation&			next (CNodeLocation bestnode_location);
	// this iterator store the current point in the search, it is used by next to return the next point
	vector<pov_vct_clss>::iterator m_pov_pt;	// iterator for the vector of points been analized
	vector<pov_vct_clss>	sort_tbl;	// points sorted in this search.

};

#endif