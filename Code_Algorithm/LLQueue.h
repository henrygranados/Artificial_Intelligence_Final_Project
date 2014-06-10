
#include "LinkedList.h"

// implementation using linked list
class LLQueue  : public IPriorityQueue
{
	 LinkedList<CAStarNode> ll;	//	linked list of CAStarNode* nodes
public:
	// methods
	CAStarNode* PopPriorityQueue (void);
	void PushPriorityQueue (CAStarNode* node);
	void UpdateNodeOnPriorityQueue (CAStarNode* node);
	bool IsPriorityQueueEmpty (void);
	void ErasePriorityQueue (void);
};