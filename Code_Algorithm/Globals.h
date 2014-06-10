#include <string>
#include <cmath>		// abs

#define ABS(a)           ((a) >= 0 ? (a) : -(a))

long tsc_get_time (void);
void debug (const char * lpszFormat, ...);

#define manhattan_dist(x1,y1,x2,y2)				(ABS((x1)-(x2)) + ABS((y1)-(y2)))
#define tsc_manhattan_fdist(x1,y1,x2,y2)		((float ) std::abs ((x1)-(x2)) + std::abs((y1)-(y2)))

void Assert (bool cond, std::string error);


#ifndef snprintf
#define snprintf sprintf_s
#endif

// colors to display the map
void changeColors (int fg, int bg);	// change the foreground and background color
void moveCursor (int row, int col);	// move the curso to the position indicated by row and col
void cls(void);						// clear the screen
void sleep (int ms);				// wait ms milliseconds.
bool validateFileName (std::string filename, std::string error);

#ifndef FOREGROUND_BLUE

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

#endif
