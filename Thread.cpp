#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <thread>
#include <mutex>
#include <future>
#include <limits>
#include <Windows.h>

using namespace std;

/* Get the Error Handle function as external function */
extern void Make_Error_log();
extern void End_Error_log();
extern void Print_Error(wstring class_name, wstring message, bool exit = 1);

/* definition of threading function */

std::mutex mtx;

/* Mouse click checker */
void Check_click_thread(int& posx, int & posy)
{
	POINT a;
	HWND hWnd;
	RECT window_size;

	while (1) {

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			GetCursorPos(&a);
			hWnd = WindowFromPoint(a);
			ScreenToClient(hWnd, &a);
			ScreenToClient(hWnd, &a);
			GetWindowRect(hWnd, &window_size);

			::mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, 0, 0, 0, ::GetMessageExtraInfo());

			mtx.lock();
			posx = a.x + window_size.left;
			posy = a.y + window_size.top;
			mtx.unlock();
		}
	}
}

/* Get wstring input */
void Get_input(int & posx, int & posy, wchar_t * input, std::promise<bool>& p)
{
	mtx.lock();
	COORD Pos = { posx - 1,posy - 1 };
	mtx.unlock();
	
	try {
		mtx.lock();
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
		wcin.clear();
		wcin.getline(input, 1024); // get input
		mtx.unlock();
	}
	catch (...) {
		mtx.lock();
		p.set_value(true);
		mtx.unlock();
		Print_Error(L"Thread.cpp",L"Get_input : input function is in error : allocation problem or size problem");
	}
	mtx.lock();
	p.set_value(true);
	mtx.unlock();
}