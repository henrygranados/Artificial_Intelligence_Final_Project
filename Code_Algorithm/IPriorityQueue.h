
#pragma once
// interfase of the priority queue
class IPriorityQueue {
public:
	virtual CAStarNode* PopPriorityQueue (void) = 0;
	virtual void PushPriorityQueue (CAStarNode* node ) = 0;
	virtual void UpdateNodeOnPriorityQueue (CAStarNode* node) = 0;
	virtual bool IsPriorityQueueEmpty (void) = 0;
	virtual void ErasePriorityQueue (void) = 0;
};