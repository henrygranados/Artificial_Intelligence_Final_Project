
#include <string>		// needed for getline (ifstream, string)
#include <algorithm>	// find
#include <vector>		// container positions in the file

using namespace std;

#include "Globals.h"
#include "CNodeLocation.h"
#include "CAStarNode.h"
#include "IPriorityQueue.h"
#include "PriorityQueue.h"

class NodeTotalGreater 
{
public:
   //This is required for STL to sort the priority queue
   //(its entered as an argument in the STL heap functions)
   bool operator() (CAStarNode* first, CAStarNode* second) const {		//	CAStarNode*
      return (first->total > second->total);
   }
};

// Extract the node at the beggining of the queue. This node has the best cost
CAStarNode* PriorityQueue::PopPriorityQueue (void)
{  //Total time = O(log n)
   
   //Get the node at the front - it has the lowest total cost
   CAStarNode * node = heap.front();

   //pop_heap will move the node at the front to the position N
   //and then sort the heap to make positions 1 through N-1 correct
   //(STL makes no assumptions about your data and doesn't want to change
   //the size of the container.)
   std::pop_heap (heap.begin(), heap.end(), NodeTotalGreater () );

   //pop_back() will actually remove the last element from the heap
   //(now the heap is sorted for positions 1 through N)
   heap.pop_back();

   return (node);
}

// Insert the node in the queue
void PriorityQueue::PushPriorityQueue (CAStarNode* node )
{  //Total time = O(log n)

	//Pushes the node onto the back of the 
	// (the heap is now unsorted)
	heap.push_back (node);

	//Sorts the new element into the heap
	std::push_heap (heap.begin(), heap.end(), NodeTotalGreater ());
}

// Search the node in the priority queue and update it
void PriorityQueue::UpdateNodeOnPriorityQueue (CAStarNode* node)
{
	std::vector<CAStarNode*>::iterator i;
	for (i = heap.begin(); i != heap.end(); i++) {
		if ((*i)->location == node->location) { // found it
			// update the heap
			std::push_heap (heap.begin(), i+1, NodeTotalGreater () );
			return;
		}
	}
}

bool PriorityQueue::IsPriorityQueueEmpty (void)
{
   return (heap.empty());
}

void PriorityQueue::ErasePriorityQueue (void)
{
	if (!IsPriorityQueueEmpty ())
		heap.erase (heap.begin (), heap.end ());
}
