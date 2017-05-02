// dualmonfix.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <vector>
#include <math.h>
#include <stdlib.h>
using namespace std;

static LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam) {
	LRESULT Result = 0;

	switch (Message) {
	case WM_CLOSE:
	case WM_ACTIVATEAPP:
	case WM_DESTROY:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_PAINT:
	case WM_MOUSEMOVE:
	default:
	{
		Result = DefWindowProcA(Window, Message, WParam, LParam);
	} break;
	}


	return(Result);
}

vector<MONITORINFO> monitors;

BOOL DoOverlap(int firstA, int secondA, int firstB, int secondB)
{
	if (firstB >= secondA || secondB <= firstA) return false;

}

MONITORINFO GetMonitor(const POINT &p)
{
	for (auto m : monitors)
	{
		if (p.x >= m.rcMonitor.left &&
			p.x < m.rcMonitor.right &&
			p.y >= m.rcMonitor.top &&
			p.y < m.rcMonitor.bottom)
		{
			return m;
		}

	}
}

BOOL GetClosestHorizMonitor(const POINT &p, MONITORINFO *closest, const MONITORINFO &current)
{
	bool found = false;
	int closeDist = 9999999;
	for (auto m : monitors)
	{
		if (p.x >= m.rcMonitor.left &&
			p.x < m.rcMonitor.right)

		{
			//if in a monitor, return that
			if (p.y >= m.rcMonitor.top && p.y < m.rcMonitor.bottom)
			{
				*closest = m;
				return true;
			}


			int distTop = abs(p.y - m.rcMonitor.top);
			int distBottom = abs(p.y - m.rcMonitor.bottom);
			int dist = min(distTop, distBottom);
			if (dist < closeDist && DoOverlap(m.rcMonitor.top, m.rcMonitor.bottom, current.rcMonitor.top, current.rcMonitor.bottom))
			{
				closeDist = dist;
				*closest = m;
				found = true;
			}
		}
	}
	return found;

}

BOOL GetClosestVertMonitor(const POINT &p, MONITORINFO *closest, const MONITORINFO &current)
{
	bool found = false;
	int closeDist = 9999999;
	for (auto m : monitors)
	{
		if (p.y >= m.rcMonitor.top &&
			p.y < m.rcMonitor.bottom)

		{
			//if in a monitor, return that
			if (p.x >= m.rcMonitor.left && p.x < m.rcMonitor.right)
			{
				*closest = m;
				return true;
			}


			int distLeft = abs(p.x - m.rcMonitor.left);
			int distRight = abs(p.x - m.rcMonitor.right);
			int dist = min(distLeft, distRight);
			if (dist < closeDist && DoOverlap(m.rcMonitor.left, m.rcMonitor.right, current.rcMonitor.left, current.rcMonitor.right))
			{
				closeDist = dist;
				*closest = m;
				found = true;
			}
		}

	}
	return found;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
	HDC      hdcMonitor,
	LPRECT   lprcMonitor,
	LPARAM   dwData)
{
	MONITORINFO info;
	info.cbSize = sizeof(info);
	if (GetMonitorInfo(hMonitor, &info))
	{
		monitors.push_back(info);
	}
	return TRUE;  // continue enumerating
}


int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode) {


	POINT oldPoint;
	GetCursorPos(&oldPoint);
	while (true)
	{
		monitors.clear();
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);
		POINT point;
		GetCursorPos(&point);
		auto monitor = GetMonitor(point);

		if (point.x == monitor.rcMonitor.left || point.x == monitor.rcMonitor.right - 1)
		{
			POINT newPoint = point;
			if (point.x == monitor.rcMonitor.left) {
				if (oldPoint.x <= newPoint.x) continue;
				newPoint.x--;
			}
			else {
				if (oldPoint.x >= newPoint.x) continue;
				newPoint.x++;
			}
			MONITORINFO nextMonitor;
			if (GetClosestHorizMonitor(newPoint, &nextMonitor,monitor)) {
				if (point.y < nextMonitor.rcMonitor.top)
				{
					newPoint.y = nextMonitor.rcMonitor.top;
				}
				else if (point.y > nextMonitor.rcMonitor.bottom)
				{
					newPoint.y = nextMonitor.rcMonitor.bottom - 1;
				}
				OutputDebugStringA("Setting cursor because we hit a horizontal edge\n");
				SetCursorPos(newPoint.x, newPoint.y);
			}
		}
		else if (point.y == monitor.rcMonitor.top || point.y == monitor.rcMonitor.bottom - 1)
		{
			POINT newPoint = point;
			if (point.y == monitor.rcMonitor.top) {
				if (oldPoint.y <= newPoint.y) continue;
				newPoint.y--;
			}
			else {
				if (oldPoint.y >= newPoint.y) continue;
				newPoint.y++;
			}
			MONITORINFO nextMonitor;
			if (GetClosestVertMonitor(newPoint, &nextMonitor, monitor)) {
				
				if (point.x < nextMonitor.rcMonitor.left)
				{
					newPoint.x = nextMonitor.rcMonitor.left;
				}
				else if (point.x > nextMonitor.rcMonitor.right)
				{
					newPoint.x = nextMonitor.rcMonitor.right - 1;
				}
				OutputDebugStringA("Setting cursor because we hit a vertical edge\n");
				SetCursorPos(newPoint.x, newPoint.y);
			}
		}

		Sleep(25);
		oldPoint = point;
	}
	return(0);
}


