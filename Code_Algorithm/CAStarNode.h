
#pragma once

class CAStarNode
{
public:
	CNodeLocation location;	// node location
	CAStarNode* parent;		// parent node (zero pointer represents starting node)
	float cost;             // cost to get to this node
	float total;            // total cost (cost + heuristic estimate)
	bool onOpen;            // on Open list
	bool onClosed;          // on Closed list
	CAStarNode () { 
		onOpen = false;
		onClosed = false;
	};
	CAStarNode (CNodeLocation loc) {
		location = loc;
		onOpen = false;
		onClosed = false;
	}
};
