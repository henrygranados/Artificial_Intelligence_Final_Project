
#ifndef _CNODELOCATION_H_
#define _CNODELOCATION_H_

class CNodeLocation {		// class to handle the location, very standard not big deal here
public:
	int	x;
	int	y;
	CNodeLocation () 
	{
		x = 0;
		y = 0;
	};
	CNodeLocation (int j, int i) 
	{
		x = i;
		y = j;
	};
	~CNodeLocation () {};
	bool operator== (CNodeLocation a)	// equality	
	{
		return x == a.x && y == a.y;
	};
	bool operator!= (CNodeLocation a)	// not equality	
	{
		return x != a.x || y != a.y;
	};
	CNodeLocation &operator= (CNodeLocation a)	// Assignment
	{
		x = a.x; 
		y = a.y;
		return *this;
	};
	void set (int j, int i);
	char *print (void);

};

extern CNodeLocation NULLLOCATION;
#endif