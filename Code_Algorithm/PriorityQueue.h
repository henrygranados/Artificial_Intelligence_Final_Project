// implementation using std::push_heap and a vector
class PriorityQueue  : public IPriorityQueue
{
	//Heap implementation using an STL vector
   // Note: the vector is an STL container, but the
   // operations done on the container cause it to
   // be a priority queue organized as a heap
   std::vector<CAStarNode*> heap;	//	CAStarNode*
public:
	// methods
	CAStarNode* PopPriorityQueue (void);
	void PushPriorityQueue (CAStarNode* node);
	void UpdateNodeOnPriorityQueue (CAStarNode* node);
	bool IsPriorityQueueEmpty (void);
	void ErasePriorityQueue (void);
};