
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "Globals.h"
#include "CMap.h"
#include "CAStar.h"

// +=======================================+ 
// | A*                                    |
// +=======================================+ 
// CAStar methods
//

CAStar::CAStar (CMap* map) 
{		
	// constructor
	m_path_initialized = false;
	m_goal = NULLLOCATION;
	m_map = map;
	m_nb_cnt = 0;
	m_openlist = NULL;
	m_nodeBank.reserve (NODEBANK_SZ);
	for (int i = 0; i < NODEBANK_SZ; i++) {
		m_nodeBank.push_back (CAStarNode());	// initialize
	}
	m_animate = false;
};

CAStar::~CAStar () {						// destructor
	if (m_openlist != NULL)
		delete m_openlist;
};

// Return the CAStarNode* preallocated from the node bank
CAStarNode* CAStar::GetNode (CNodeLocation node_location)
{
	CAStarNode* node = GetNodeFromMasterNodeList (node_location);
	if (!node) {   // if not found get a new one from the bank
		node = GetFreeNodeFromNodeBank ();
		// and initialize everything
		node->location = node_location;
		node->onOpen = false;
		node->onClosed = false;
		// store the node in the hash table
		StoreNodeInMasterNodeList (node);
	}
	return (node);
}

// Main find path algorithm
bool CAStar::FindPath (CNodeLocation start, CNodeLocation goal)
{
	debug ("AS: FindPath starting... (%s) =>",  start.print ());
	debug ("(%s)", goal.print ());
	// check if we already started the path
	if (!m_path_initialized || goal != m_goal) {
		m_path_initialized = true;
		m_goal = goal;
		// check if we have visual, if yes we have the road and success
		CNodeLocation tmploc;
		tmploc = start;
		if (m_map->visual (tmploc, goal, m_road)) {
			return true;   // success.
		}
		ResetMasterNodeList ();	// reset the bank
		// Create the  first node with the start position
		debug ("AS: FindPath location del npc goal (%s)", m_goal.print ());
		CAStarNode* startnode = new CAStarNode(start);
		startnode->onOpen   = true;		// This node is going on the Open list
		startnode->onClosed = false;	// This node is not on the Closed list
		startnode->parent   = 0;		// This node has no parent
		startnode->cost     = 0;		// This node has no cost to get to
		startnode->total    = 0.0;
		// reset the priority queue and insert the first node in it
		m_openlist->ErasePriorityQueue ();	// erase the list
		m_openlist->PushPriorityQueue (startnode);
		m_starttime = tsc_get_time ();
	}
	debug ("AS: Process m_openlist");
	while (!m_openlist->IsPriorityQueueEmpty ()) {
		// Get the best candidate node
		CAStarNode*	bestnode = m_openlist->PopPriorityQueue ();
		CAStarNode		newnode;
		debug ("AS: FindPath new node from openlist %s", bestnode->location.print ());
		if (AtGoal (bestnode, m_goal)) {  
			// we reach the goal, construct the path and exit
			ConstructPathToGoal (start, bestnode);
			return true;   // success.
		}
		if (m_animate) {
			moveCursor (0,0);		// graphics display
			m_map->print (false);	// graphics display
			sleep (5);				// graphics display
		}
		
		// path is CAStar
		while ((newnode.location = this->next (bestnode)) != NULLLOCATION) {	
			// check we are working on a new fresh node, not already visited
			debug ("AS: FindPath newnode %s", newnode.location.print ());
			if ((bestnode->parent == 0 || bestnode->parent->location != newnode.location) &&
				newnode.location != bestnode->location) { 
				// insert in the parent list, and calculate this node cost
				newnode.parent = bestnode;
				newnode.cost   = bestnode->cost + CostFromNodeToNode (&newnode, bestnode);
				newnode.total  = newnode.cost;
				
				// Get the preallocated node for this location
				CAStarNode* actualnode = GetNode (newnode.location);
				if (!(actualnode->onOpen && newnode.total > actualnode->total) &&
				 !(actualnode->onClosed && newnode.total > actualnode->total)) {  
					// This is a good candidate
					actualnode->onClosed = false;   //effectively removing it from Closed
					actualnode->parent   = newnode.parent;
					actualnode->cost     = newnode.cost;
					actualnode->total    = newnode.total;
					debug ("FindPath inserted %s", actualnode->location.print ());
					if (actualnode->onOpen) {  
						// already in the list, update the cost value
						m_openlist->UpdateNodeOnPriorityQueue (actualnode);
						if (m_map->get (actualnode->location) == MAP_WHT_EMPTY) 
							m_map->set (actualnode->location, MAP_WHT_CAND); // graphics display
					} else {  
						// put the node in the list
						m_openlist->PushPriorityQueue (actualnode);
						actualnode->onOpen = true;
					}
				}
			}
		}
		// we are done with bestnode now put it in closed list
		bestnode->onClosed = true;
		if (m_map->get (bestnode->location) == MAP_WHT_CAND)
			m_map->set (bestnode->location, MAP_WHT_EMPTY); // graphics display
	}
	debug ("FindPath: failed (%s)", start.print ());
	return false;
}
// Store in m_road the nodes from bestnode to the goal
void CAStar::ConstructPathToGoal (CNodeLocation start, CAStarNode* bestnode)
{
	CAStarNode *apnode;
	int		 antilock;

	if (!m_road.empty())
		m_road.erase (m_road.begin (), m_road.end ());
	// build the path from the bextnode until last node
	if (bestnode->parent != 0)
		for (antilock = 0, apnode = bestnode->parent;antilock < 100 && apnode->parent != 0;
				 antilock++, apnode = apnode->parent) {
			m_road.insert (m_road.begin (), apnode->location);
		}
	m_road.push_back (bestnode->location);	// put the last one
	m_road.push_back (m_goal);				// and the goal!

	// debug.
	debug ("Print road");
	vector<CNodeLocation>::iterator p;
	for (p = m_road.begin (); p != m_road.end ();p++) {
		debug ("%s", (*p).print ());
	}
	// end debug
}

// Calculate the cost of this node

float CAStar::CostFromNodeToNode (CAStarNode* newnode, CAStarNode* bestnode)
{
	float	x = (float) newnode->location.x - (float) bestnode->location.x;
	float	y = (float) newnode->location.y - (float) bestnode->location.y;

	vector<CNodeLocation> road;
	return m_map->visual (newnode->location, bestnode->location, road) 
		   ? sqrt (x * x + y * y) 
		   : 3000.0f;		// this is the longest distance in a normal map.
}

bool CAStar::AtGoal (CAStarNode* bestnode, CNodeLocation goal)
{
	// in a simple example like this where there are a squared grid, just check if we are at the goal
	return bestnode->location == goal;
}

// Return the pre allocated node of this location (node_location)
CAStarNode* CAStar::GetNodeFromMasterNodeList (CNodeLocation node_location) 
{
	// calculate the hash value of this node
	float f = (float) node_location.x * 10000.0f + (float) node_location.y;
	as_map::iterator n = MasterNodeList.find (f);
	if (n != MasterNodeList.end ())	// found return the node
		return (MasterNodeList[f]);
	return NULL;
}

// Store the node in the master node list. 
// Use a hash function to calculate the index based on the position of the node (x, y)
void CAStar::StoreNodeInMasterNodeList (CAStarNode* newNode)
{
	float f = (float) newNode->location.x * 10000.0f + (float) newNode->location.y;
	MasterNodeList[f] = newNode;
}

// Get a free node from the node bank
CAStarNode* CAStar::GetFreeNodeFromNodeBank (void) 
{
	if (m_nb_cnt < NODEBANK_SZ)
		return &m_nodeBank[m_nb_cnt++];
	else {
		debug ("GetFreeNodeFromNodeBank: exhausted");
		return NULL;
	}
}

// Reset NodeBank y MasterNodeList
void CAStar::ResetMasterNodeList (void)
{
	for (int i = 0;i < NODEBANK_SZ;++i) {
		m_nodeBank[i].location = NULLLOCATION;
		m_nodeBank[i].parent	= 0;
		m_nodeBank[i].cost		= 0.0f;
		m_nodeBank[i].total	= 0.0f;
		m_nodeBank[i].onOpen   = false;
		m_nodeBank[i].onClosed	= false;
	}
	m_nb_cnt = 0;
	if (!MasterNodeList.empty ())
		MasterNodeList.erase (MasterNodeList.begin (), MasterNodeList.end ());
}

// Reset the whole system, node bank, priority list and so
void CAStar::reset (void)
{
	ResetMasterNodeList ();
	m_path_initialized = false;
	m_goal = NULLLOCATION;
	m_openlist->ErasePriorityQueue ();
	if (!m_road.empty())
		m_road.erase (m_road.begin (), m_road.end ());
}

// This is the only interfase with the Map
// 
CNodeLocation& CAStar::next (CAStarNode*	bestnode)
{
	return m_map->next (bestnode->location);
}



