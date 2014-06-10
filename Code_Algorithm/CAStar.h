
#ifndef _CASTAR_H_
#define _CASTAR_H_

#include <map>		
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
#include "CNodeLocation.h"
#include "CAStarNode.h"
#include "IPriorityQueue.h"

//+=======================================+ 
//| A*                                    |
//+=======================================+ 

typedef map<float, CAStarNode*> as_map;						// to store nodes been processed


#define NODEBANK_SZ	1000

class CAStar {
	CMap*					m_map;				// map to be used 
	vector<CAStarNode>		m_nodeBank;
	int						m_nb_cnt;
	as_map					MasterNodeList;		// nodes list (open y closed)
	// graphics
	bool					m_animate;			// if true, display the evolution of the path finding
public:
	bool					m_path_initialized;	// true if the engine has been intialized, for example there are path finding active
	CNodeLocation			m_goal;				// current goal
	vector<CNodeLocation>	m_road;				// the road been detected
	IPriorityQueue*			m_openlist;			// open list implemented using an Interfase
	unsigned int			m_starttime;		// debug, to check the start time
	CAStar (CMap* map);							// constructor
	~CAStar ();
	bool			FindPath (CNodeLocation start, CNodeLocation goal);
	CAStarNode*		GetNode (CNodeLocation node_location);
	void			ConstructPathToGoal (CNodeLocation start, CAStarNode* bestnode);
	float			CostFromNodeToNode (CAStarNode* newnode, CAStarNode* bestnode);
	bool			AtGoal (CAStarNode* bestnode, CNodeLocation goal);
	CNodeLocation&  next (CAStarNode* bestnode);

	CAStarNode*		GetNodeFromMasterNodeList (CNodeLocation node_location);
	void			StoreNodeInMasterNodeList (CAStarNode* newNode);
	CAStarNode* GetFreeNodeFromNodeBank (void);
	// Reset NodeBank and MasterNodeList
	void ResetMasterNodeList (void);
	void reset (void);

	// graphics
	void animate (bool animate) { m_animate = animate; };

	// accessor
	vector<CNodeLocation>& getRoad (void) { return m_road; };
};

#endif

