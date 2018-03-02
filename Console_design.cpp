#include "Console_design.h"

int design_tool::getkey()
{
	int ch = _getch();
	if (ch == 0xE0 || ch == 0)
	{
		ch = _getch();
	}
	return ch;
}

void design_tool::SetColor(color colFront, color colBack)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((colBack << 4) | colFront));
}

void design_tool::gotoxy(int x, int y)
{
	COORD Pos = { (short)x - 1, (short)y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


// Ŀ���� x ��ǥ�� �����Ѵ�.
int design_tool::wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}

// Ŀ���� y��ǥ�� �����Ѵ�.
int design_tool::wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

// consoleâ �ʱ�ȭ
void design_tool::Clean_Console()
{
	/* ȭ�� �ʱ�ȭ */
	system("cls");
}

void design_tool::enter_gotoxy(int x)
{
	int y;
	y = wherey();
	gotoxy(x, y + 2);
}

/* design ��� */
int design_tool::Printdesign(bool if_get_input, wstring * input, int px, int py)
{
	int option = 0; // default value is 0

	Clean_Console();

	/* input�� �޾ƾ��ϴ� ��� */
	if (if_get_input) {
		/* error checking */
		if (px >= this->pos_x + x_size * 2 || px <= this->pos_x) {
			Print_Error(L"design_tool", L"Printdesign : posx is not valid value");
			return false;
		}
		if (py >= this->pos_y + y_size || py <= this->pos_y + 4) {
			Print_Error(L"design_tool", L"Printdesign : posy is not valid value");
			return false;
		}
		for (int i = 0; i < num_message; i++)
		{
			if (message_pos[i].x == px && message_pos[i].y == py) {
				Print_Error(L"design_tool", L"Printdesign : other message already occupy that space");
				return false;
			}
		}
		for (int i = 0; i < num_button; i++)
		{
			if (button_pos[i].x == px && button_pos[i].y == py) {
				Print_Error(L"design_tool", L"Printdesign : other button already occupy that space");
				return false;
			}
		}
	}

	//�簢�� ��� ����
	SetColor(color_set, backcolor_set);
	gotoxy(pos_x, pos_y);
	cout << "��";
	gotoxy(pos_x + x_size*2, pos_y);
	cout << "��";
	gotoxy(pos_x, pos_y + y_size - 1);
	cout << "��";
	gotoxy(pos_x + x_size*2, pos_y + y_size - 1);
	cout << "��";

	gotoxy(pos_x + 2, pos_y);
	for (int i = 0; i < x_size*2 - 3; i++) // ���� �簢�� ����
		cout << "��";

	gotoxy(pos_x + 2, pos_y + y_size - 1);
	for (int i = 0; i < x_size*2 - 3; i++) // �Ʒ��� �簢�� ����
		cout << "��";

	for (int i = 1; i < y_size - 1; i++) // ����, ������ �簢�� ����
	{
		gotoxy(pos_x, pos_y + i);
		wcout << L"��";
		gotoxy(pos_x + 2 * x_size, pos_y + i);
		wcout << L"��";
	}
	//�簢�� �׸��� �Ϸ�

	//Ÿ��Ʋ ���
	gotoxy(pos_x + (x_size - len_title / 2) - 2, pos_y + 1);
	wcout << title;

	gotoxy(pos_x + 4, pos_y + 3);

	for (int i = 0; i < 2*x_size - 7; i++)
		cout << "��";

	/* message drawer */
	for (int i = 0; i < num_message; i++)
	{
		gotoxy(this->message_pos[i].x, this->message_pos[i].y);
		wcout << message[i];
	}

	/* button drawer */
	for (int i = 0; i < num_button; i++)
	{
		gotoxy(this->button_pos[i].x, this->button_pos[i].y);
		wcout << L"[" << button_name[i] << L"]";
		this->button_length.push_back(wherex() - this->button_pos[i].x); // button�� ���� ���� ���
	}

	/* button act */
	int pos_x = -80;
	int pos_y = -80;

	if (!if_get_input) {
		th1 = thread(&design_tool::Check_click_thread, this, std::ref(pos_x), std::ref(pos_y));

		/* button act */
		while (1)
		{
			if (num_button == 0)
				break;

			/* goto default place */
			gotoxy(1, 1);
			/* ��ư�� Ŭ���� ��� */
			if (button_click_check(pos_x, pos_y) != -1) {
				option = button_click_check(pos_x, pos_y);
				break;
			}
		}

		mtx.lock();
		this->thread_state = 1;
		mtx.unlock();
		if (th1.joinable()) {
			th1.join();
		}
	}
	else
	{
		std::promise<bool> p;
		wchar_t * tmp_input = new wchar_t[1024];

		th1 = thread(&design_tool::Check_click_thread, this, std::ref(pos_x), std::ref(pos_y));
		th2 = thread(&design_tool::Get_input, this, std::ref(px), std::ref(py), std::ref(tmp_input), std::ref(p));

		auto future = p.get_future();

		/* button act */
		while (1)
		{

			auto status = future.wait_for(0ms);

			if (num_button == 0 && (status == std::future_status::ready)) {
				break;
			}

			/* ��ư�� Ŭ���ǰ� input�� �޾��� ��� */
			if (button_click_check(pos_x, pos_y) != -1 && (status == std::future_status::ready)) {
				option = button_click_check(pos_x, pos_y);
				break;
			}

		}

		input->assign(tmp_input);
		delete[] tmp_input;

		mtx.lock();
		this->thread_state = 1;
		mtx.unlock();

		if (th2.joinable() && th1.joinable()) {
			th1.join();
			th2.join();
		}
	}
	
	this->button_length.clear();

	mtx.lock();
	this->thread_state = 0;
	mtx.unlock();

	Clean_Console();

	return option;
}

bool design_tool::GetFontSize(HANDLE windowHandle, COORD *size) // ���� ũ�� �ޱ�
{
	CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

	if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return false;
	}

	*size = font.dwFontSize;

	return true;
}
bool design_tool::SetFontSize(HANDLE windowHandle, COORD size) // ���� ũ�� ����
{
	CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };

	if (!GetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return false;
	}

	font.dwFontSize = size;

	if (!SetCurrentConsoleFontEx(windowHandle, 0, &font))
	{
		return false;
	}

	return true;
}

int design_tool::Message_length(wstring message)
{
	int required_len = 0;

	for (int i = 0; i < message.length(); i++) {
		if (message[i] & 0x80)
			required_len += 2;
		else
			required_len += 1;
	}
	required_len++;

	return required_len;
}

/* title�� �ٲ۴�. */
bool design_tool::Changetitle(wstring new_title)
{
	if (this->x_size - (len_title / 2) < 3) // Ÿ��Ʋ�� �簢�� �� ���� ũ�⸦ �Ѿ�� ���
	{
		Print_Error(L"design_tool", L"Changetitle : Width setting is not correct.");
		return false;
	}

	/* set design id */
	this->len_title = new_title.length();
	this->title = new_title;

	return true;
}

/* ���� ũ�⸦ �ٲ۴�. */
bool design_tool::Changetext_size(int text_xsize, int text_ysize)
{
	/* set text size */
	COORD size;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	try {
		size.X = (SHORT)text_xsize;
		size.Y = (SHORT)text_ysize;
		SetFontSize(h, size);
	}
	catch (...) {
		Print_Error(L"design_tool", L"Changtext_size : Changing text_size is not working");
		return false;
	}
	this->text_xsize = text_xsize;
	this->text_ysize = text_ysize;

	return true;
}

/* Button�� �����Ѵ�. */
bool design_tool::Deletebutton(int idx)
{
	for (int i = 0; i < num_button; i++)
	{
		/* ���� idx�� button ���� */
		if (button_pos[i].idx == idx) {
			button_name.erase(button_name.begin() + i);
			button_pos.erase(button_pos.begin() + i);
			num_button--;
			return true;
		}
	}

	//Print_Error(L"design_tool", L"Deletebutton : corresponding button doesn't exist");
	return false;
}

/* Message�� �����Ѵ�. */
bool design_tool::DeleteMessage(int idx)
{
	for (int i = 0; i < num_message; i++)
	{
		/* ���� idx�� button ���� */
		if (message_pos[i].idx == idx) {
			message.erase(message.begin() + i);
			message_pos.erase(message_pos.begin() + i);
			num_message--;
			return true;
		}
	}

	//Print_Error(L"design_tool", L"DeleteMessage : corresponding message doesn't exist");
	return false;
}

/* Button�� �߰��Ѵ�. */
bool design_tool::Addbutton(wstring button_name, int posx, int posy, int idx_num)
{
	_2D_pos pos;
	pos.x = posx;
	pos.y = posy;
	pos.idx = idx_num;

	/* Error catcher */
	if (posx + Message_length(button_name) + 4 >= this->pos_x + x_size * 2 || posx <= this->pos_x) {
		Print_Error(L"design_tool", L"addbutton : posx is not valid value");
		return false;
	}
	if (posy >= this->pos_y + y_size || posy <= this->pos_y + 4) {
		Print_Error(L"design_tool", L"addbutton : posy is not valid value");
		return false;
	}
	if (button_name.find(L"\n") != string::npos) {
		Print_Error(L"design_tool", L"addbutton : \\n must not be included in button name");
		return false;
	}
	if (idx_num < 0) {
		Print_Error(L"design_tool", L"addbutton : button idx must be positive");
		return false;
	}
	for (int i = 0; i < num_button; i++) {
		if (button_pos[i].idx == idx_num) {
			Print_Error(L"design_tool", L"addbutton : same button idx already exists");
			return false;
		}
	}
	for (int i = 0; i < num_message; i++)
	{
		if (message_pos[i].x == posx && message_pos[i].y == posy) {
			Print_Error(L"design_tool", L"addbutton : other message already occupy that space");
			return false;
		}
	}
	for (int i = 0; i < num_button; i++)
	{
		if (button_pos[i].x == posx && button_pos[i].y == posy) {
			Print_Error(L"design_tool", L"addbutton : other button already occupy that space");
			return false;
		}
	}

	this->button_name.push_back(button_name);
	this->button_pos.push_back(pos);
	num_button++;
	return true;
}
/* Message�� �߰��Ѵ�. */
bool design_tool::AddMessage(wstring message, int posx, int posy, int idx)
{
	_2D_pos pos;
	pos.x = posx;
	pos.y = posy;
	pos.idx = idx;

	/* Error catcher */
	if (posx + Message_length(message) >= this->pos_x + x_size * 2 || posx <= this->pos_x) {
		Print_Error(L"design_tool", L"addmessage : posx is not valid value");
		return false;
	}
	if (posy >= this->pos_y + y_size || posy <= this->pos_y + 4) {
		Print_Error(L"design_tool", L"addmessage : posy is not valid value");
		return false;
	}
	if (message.find(L"\n") != string::npos) {
		Print_Error(L"design_tool", L"addmessage : \\n must not be included in message");
		return false;
	}
	if (idx > 0) {
		Print_Error(L"design_tool", L"addmessage : message idx must be negative");
		return false;
	}
	for (int i = 0; i < num_message; i++) {
		if (message_pos[i].idx == idx) {
			Print_Error(L"design_tool", L"addmessage : same message idx already exists");
			return false;
		}
	}
	for (int i = 0; i < num_message; i++)
	{
		if (message_pos[i].x == posx && message_pos[i].y == posy) {
			Print_Error(L"design_tool", L"addmessage : other message already occupy that space");
			return false;
		}
	}
	for (int i = 0; i < num_button; i++)
	{
		if (button_pos[i].x == posx && button_pos[i].y == posy) {
			Print_Error(L"design_tool", L"addmessage : other button already occupy that space");
			return false;
		}
	}

	this->message.push_back(message);
	this->message_pos.push_back(pos);
	num_message++;
	return true;
}

// console x��ǥ�� mouse x��ǥ�� ��ȯ (���� ���� ����)
int design_tool::Console_to_Mouse_conv_x(int x_pos)
{
	return x_pos*this->text_xsize - 7;
}
// console y��ǥ�� mouse y��ǥ�� ��ȯ (���� ���� ����)
int design_tool::Console_to_Mouse_conv_y(int y_pos)
{
	return (y_pos - 1)*this->text_ysize - 29;
}

// � ��ư�� Ŭ�� �ƴ��� üũ
int design_tool::button_click_check(int mouse_x, int mouse_y)
{
	int button_id = -1; // �ƹ��͵� �ȴ��� ���

	/* ��ư�� x ���� */
	int x_min = 0;
	int x_max = 0;

	/* ��ư�� y ���� */
	int y_min = 0;
	int y_max = 0;

	for (int i = 0; i < num_button; i++)
	{
		x_min = Console_to_Mouse_conv_x(this->button_pos[i].x);
		x_max = Console_to_Mouse_conv_x(this->button_pos[i].x + this->button_length[i]);
		
		y_min = Console_to_Mouse_conv_y(this->button_pos[i].y);
		y_max = y_min + this->text_ysize;
		/* ��ġ�ϴ� ��ư�� ã�� ��� */
		if (mouse_x >= x_min && mouse_x <= x_max && mouse_y >= y_min && mouse_y <= y_max) {
			button_id = this->button_pos[i].idx;
			break;
		}
	}

	return button_id;
}

/* Mouse click checker */
void design_tool::Check_click_thread(int& posx, int & posy)
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
		mtx.lock();
		if (this->thread_state) {
			mtx.unlock();
			break;
		}
		mtx.unlock();
	}
}

/* Get wstring input */
void design_tool::Get_input(int & posx, int & posy, wchar_t * input, std::promise<bool>& p)
{
	mtx.lock();
	COORD Pos = { posx - 1,posy - 1 };
	mtx.unlock();

	try {
		mtx.lock();
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
		wcin.clear();
		wcin >> ws;
		wcin.getline(input, 1024); // get input
		mtx.unlock();
	}
	catch (...) {
		mtx.lock();
		p.set_value(true);
		mtx.unlock();
		Print_Error(L"Thread.cpp", L"Get_input : input function is in error : allocation problem or size problem");
	}
	mtx.lock();
	p.set_value(true);
	mtx.unlock();
}