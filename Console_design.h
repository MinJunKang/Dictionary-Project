#pragma once

#include "defs.h"
#include "Thread.h"

/* 글자크기 16에 대해 */
//#define MOUSE_X_LENGTH 8 // MOUSE 포인터의 X 한눈금 길이
//#define MOUSE_Y_LENGTH 16 // MOUSE 포인터의 Y 한눈금 길이

using namespace std;

	class design_tool
	{
	private:
		wstring title;
		vector<wstring> message;
		vector<wstring> button_name;
		vector<_2D_pos> message_pos;
		vector<_2D_pos> button_pos;
		vector<int> button_length; // button의 가로길이
		char color_set;
		char backcolor_set;
		int pos_x; // 시작 위치 x
		int pos_y; // 시작 위치 y
		int x_size;
		int y_size;
		int len_title;
		int num_button;
		int num_message;
		int Console_to_Mouse_conv_x(int x_pos);
		int Console_to_Mouse_conv_y(int y_pos);
		int button_click_check(int mouse_x, int mouse_y);
		int text_xsize; // 한글자 가로 크기 // MOUSE 포인터의 X 한눈금 길이
		int text_ysize; // 한글자 세로 크기 // MOUSE 포인터의 Y 한눈금 길이
		bool GetFontSize(HANDLE windowHandle, COORD *size); // 글자 크기 받기
		bool SetFontSize(HANDLE windowHandle, COORD size); // 글자 크기 셋팅
		int Message_length(wstring message); // Message 길이 반환

		/* Mouse click checker */
		void Check_click_thread(int& posx, int & posy);
		/* Get wstring input */
		void Get_input(int & posx, int & posy, wchar_t * input, std::promise<bool>& p);

		thread th1;
		thread th2;
		std::mutex mtx;

		int thread_state;
	protected:
		int getkey();
		void SetColor(color colFront, color colBack);
		void gotoxy(int x, int y);
		void Clean_Console();
		int wherex(); // 커서의 x 좌표를 조사한다.
		int wherey(); // 커서의 y좌표를 조사한다.
		void enter_gotoxy(int x); // enter함수
	public:
		/* 전체적인 design을 만든다. */
		design_tool(int x_pos, int y_pos, int height, int width, wstring title, char Color = WHITE, char BackColor = BLACK, int cmd_height = 36, int cmd_width = 120, int text_xsize = 8, int text_ysize = 16) : num_button(0), num_message(0),thread_state(0)
		{
			/* set cmd size */
			wstring cmd_commend = L"mode con cols=";
			wstring cmd_commend2 = L" lines=";

			_wsystem((cmd_commend + to_wstring(cmd_width) + cmd_commend2 + to_wstring(cmd_height)).c_str());

			/* set text size */
			COORD size;
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

			size.X = (SHORT)text_xsize;
			size.Y = (SHORT)text_ysize;
			SetFontSize(h, size);

			/* set design id */
			this->len_title = title.length();
			this->title = title;

			/* design rule check */

			if (y_pos < 0) {
				Print_Error(L"design_tool", L"y_pos can't have negative value. Automatically set to 0", FALSE);
				y_pos = 0;
			}
			if (x_pos < 0) {
				Print_Error(L"design_tool", L"x_pos can't have negative value. Automatically set to 0", FALSE);
				x_pos = 0;
			}
			if (x_pos + width > cmd_width) {
				Print_Error(L"design_tool", L"x size is too big. Automatically set to maximum size", FALSE);
				width = cmd_width - x_pos;
			}
			if (y_pos + height > cmd_height) {
				Print_Error(L"design_tool", L"y size is too big. Automatically set to maximum size", FALSE);
				height = cmd_height - y_pos;
			}
			if (height <= 1) // 높이나 0이면 경고
			{
				Print_Error(L"design_tool", L"Height can't be 0. Automatically set to 14", FALSE);
				height = 14;
			}
			if (width <= 1) // 너비가 0이면 경고
			{
				Print_Error(L"design_tool", L"Width can't be 0. Automatically set to 14", FALSE);
				width = 14;
			}
			if (width - (len_title / 2) < 3) // 타이틀이 사각형 윗 변의 크기를 넘어서면 경고
			{
				Print_Error(L"design_tool", L"Width setting is not correct. Automatically set to 30", FALSE);
				width = 30;
			}
			if (width < 8) // 크기가 메뉴 버튼보다 작으면 경고
			{
				Print_Error(L"design_tool", L"Width setting is not correct. Automatically set to 30", FALSE);
				width = 30;
			}

			/* Initialization */
			this->text_xsize = text_xsize;
			this->text_ysize = text_ysize;
			this->color_set = Color;
			this->backcolor_set = BackColor;
			this->x_size = width;
			this->y_size = height;
			this->pos_x = x_pos;
			this->pos_y = y_pos;

		}
		~design_tool(){}

		/* title을 바꾼다. */
		bool Changetitle(wstring new_title);

		/* 글자 크기를 바꾼다. */
		bool Changetext_size(int text_xsize, int text_ysize);

		/* Button을 추가한다. */
		bool Addbutton(wstring button_name, int pos_x, int pos_y, int idx);

		/* Message를 추가한다. */
		bool AddMessage(wstring message, int pos_x, int pos_y, int idx);

		/* Button을 삭제한다. */
		bool Deletebutton(int idx);

		/* Message를 삭제한다. */
		bool DeleteMessage(int idx);

		/* Design한 콘솔을 출력 */
		int Printdesign(bool, wstring * input = nullptr, int px = 0, int py = 0);
	};


