#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

// This template will be used to implement the lists of nodes
template<class T> 
class LinkedList
{
	// struct to store the node data
	struct Node {
		T*		data;
		Node*	link;
		Node*	prev;
		bool	end;
	};
	int		m_size;
	Node*	m_list;
	Node*	m_current;	// this is to handle the navigation throught the list
	map<int,Node*>	m_keys;
public:

	// Default constructor. Creates an empty list.
	LinkedList() : m_size(0), m_list(NULL),m_current(NULL) {};

	~LinkedList(){ clear_list();}; // destructor clear the list

	int		size() const { return m_size; }; 	// return the size of the list, ie how many nodes it has

	Node* get_head (void) { return m_list; }

	Node* pop (void) {
		if (!m_size)
			return NULL;
		Node*	n = m_list;
		m_list = m_list->link;
		m_size--;
		m_keys.erase (kHash(n->data)); // erasing by key the element
		return n; 
	}

	void remove (T* data) 
	{
		if (!m_size)
			return;
		if (m_keys.find (kHash(data)) == m_keys.end ())
			return;

		Node*	n = m_keys[kHash(data)];
		Node* prev;	// just for clear code 
		prev = n->prev;
		if (prev != NULL) {
			prev->link = n->link;
			if (n->link != NULL) {
				n->link->prev = prev;
			}
		} else {
			// first element
			m_list = n->link;
			if (n->link != NULL) {
				n->link->prev = NULL;
			}
		}
		m_size--;
		m_keys.erase (kHash(n->data)); // erasing by key the element
	}

	// Return pointer to the data of the first node
	T*    get_first (void){ 
		m_current = m_list; 
		if (m_current != NULL) 
			return m_current->data; 
		else return NULL; 
	};

	// Return pointer to the data of the next node
	T*    get_next (void){ 

		if (m_current == NULL)
			return NULL;

		m_current = m_current->link;

		if (m_current != NULL) 
			return m_current->data; 
		else 
			return NULL; 
	};

	// add a new node to the list
	// user data->cost
	// first element must has the lowest cost so,
	// the sorting is n->cost < n2->cost ...
	void	add (T* data) {
		if (data == NULL) {
			cerr <<  "add() - NULL data."<<endl;
			return;
		}
		
		Node* n;
		
		n = new Node;	// create a new node
		// store the data
		n->data = data;
		n->link = NULL;
		n->prev = NULL;
		m_keys[kHash(n->data)] = n;

		// insert in the list
		if (m_size == 0) {	// size == 0, then use the first node
			m_list = n;		// assign the node as the first node
			m_size = 1;		// size of the list at this momment is 1
			return;			// done
		}

		// insert the node in order
		
		// special case first
		if (n->data->cost <= m_list->data->cost) {
			n->link = m_list;
			m_list->prev = n;
			m_list = n;
			m_size++;
			return;
		}

		Node*	c;
        c = m_list;			// get the current node
		Node*	prev;		// we need to keep the previous node to do the insertion

		prev = c;

		// insert the node in descending order
		// loop until n->data is in order or link is null, then insert the node

		while (c->link != NULL && n->data->cost > c->data->cost) {
			prev = c;
			c = c->link;
		}
		if (n->data->cost <= c->data->cost) {
			// insert the node in order
			if (prev != c) {
				prev->link = n;
				n->prev = prev;
			}
			n->link = c;
			c->prev = n;
		} else {
			c->link = n;
			n->prev = c;
		}
		m_size++;
	}

	// Removes all the nodes from the list, does not delete the data.
	void clear_list(void)
	{
		Node* n;	// node to delete

		while (m_list != NULL) {
			n = m_list;
			m_list = m_list->link;
			delete n;
		}
		m_list = NULL;
		m_size = 0;
	};


};


#endif
