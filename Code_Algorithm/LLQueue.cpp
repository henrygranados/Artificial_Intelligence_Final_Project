
#include <iostream>
#include <string>		// needed for getline (ifstream, string)
#include <vector>		// container positions in the file
#include <map>			// container positions in the file

using namespace std;

#include "Globals.h"
#include "CNodeLocation.h"
#include "CAStarNode.h"
#include "IPriorityQueue.h"
#include "LLQueue.h"

// implementation using linked list

int kHash (CAStarNode* node);

CAStarNode* LLQueue::PopPriorityQueue (void)
{  
	//Get the node at the front - it has the lowest total cost
	CAStarNode * node = ll.pop ()->data;

   return (node);
}


void LLQueue::PushPriorityQueue (CAStarNode* node )
{  
	ll.add (node);
}


void LLQueue::UpdateNodeOnPriorityQueue (CAStarNode* node)
{  
	ll.remove (node);
	ll.add (node);
}

bool LLQueue::IsPriorityQueueEmpty (void)
{
   return (ll.size() == 0);
}

void LLQueue::ErasePriorityQueue (void)
{
	ll.clear_list ();
}

int kHash (CAStarNode* node)
{
	return node->location.y * 511 + node->location.x;
}