
#include <Windows.h>
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <string>		// needed for getline (ifstream, string)
#include "Globals.h"

using namespace std;

long tsc_get_time (void)
{
	time_t timer;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */
	return (long) timer;
}

void debug (const char * lpszFormat, ...)
{
	if (1) return;

	int			nBuf;
	char		szBuffer[512];
	va_list		args;

	va_start (args, lpszFormat);
	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
	if (nBuf >= sizeof(szBuffer))
		snprintf (szBuffer, 512, "g_write_debug: Data too long!!");
	printf ("%s\n", szBuffer);
}

void Assert (bool cond, string error)
{
	if (!cond) {
		cerr << error << endl;
		exit (1);
	}
}

void changeColors (int fg, int bg)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdout == INVALID_HANDLE_VALUE)  {
        cout << "Error while getting input handle" << endl;
        return;
    }
    SetConsoleTextAttribute(hStdout, fg | bg | FOREGROUND_INTENSITY);
    // normal : SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return;
}

void moveCursor (int row, int col)
{
	COORD dwCursorPosition;

	dwCursorPosition.Y = row;
	dwCursorPosition.X = col;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdout == INVALID_HANDLE_VALUE)  {
        cout << "Error while getting input handle" << endl;
        return;
    }
	SetConsoleCursorPosition (hStdout, dwCursorPosition);
}

/* Standard error macro for reporting API errors */ 
 #define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__, GetLastError(), api, __LINE__);}

void cls(void)
{
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the
								cursor */ 
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
	DWORD dwConSize;                 /* number of character cells in
								the current buffer */ 

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
	/* get the number of character cells in the current buffer */ 

	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	PERR( bSuccess, "GetConsoleScreenBufferInfo" );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */ 

	bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
	dwConSize, coordScreen, &cCharsWritten );
	PERR( bSuccess, "FillConsoleOutputCharacter" );

	/* get the current text attribute */ 

	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	PERR( bSuccess, "ConsoleScreenBufferInfo" );

	/* now set the buffer's attributes accordingly */ 

	bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
	dwConSize, coordScreen, &cCharsWritten );
	PERR( bSuccess, "FillConsoleOutputAttribute" );

	/* put the cursor at (0, 0) */ 

	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	PERR( bSuccess, "SetConsoleCursorPosition" );
	return;
}

// wait ms milliseconds.

void sleep (int ms)
{
	Sleep(ms);
}

// Validate if the file exist, if it not display error message
bool validateFileName (string filename, string error)
{
	FILE* fp = fopen (filename.c_str (), "r");
	if (fp == NULL) {
		cerr << error << endl;
		return false;
	}
	fclose (fp);

	return true;
}
