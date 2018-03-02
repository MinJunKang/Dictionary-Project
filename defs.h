#pragma once

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <io.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <thread>
#include <future>
#include <algorithm>
#include <limits>
#include <ios>     // for <streamsize>
#include <direct.h>		//mkdir
#include <errno.h>		//errno

using namespace std;

#define TRUE 1
#define FALSE 0

typedef unsigned char color;
const color BLACK = 0;
const color BLUE = 1;
const color GREEN = 2;
const color CYAN = 3;
const color RED = 4;
const color MAGENTA = 5;
const color PURPLE = MAGENTA;
const color BROWN = 6;
const color LIGHTGRAY = 7;
const color GRAY = 8;
const color LIGHTBLUE = 9;
const color LIGHTGREEN = 10;
const color LIGHTCYAN = 11;
const color LIGHTRED = 12;
const color LIGHTMAGENTA = 13;
const color LIGHTPURPLE = LIGHTMAGENTA;
const color YELLOW = 14;
const color WHITE = 15;

typedef struct _2D_pos{
	int x;
	int y;
	int idx; // message idx : negative // button idx : positive
}_2D_pos;

/* For Voca Design class data type */
typedef struct Word
{
	wstring English_word;
	vector<wstring> meaning;
	wstring Group_name;
	double error_rate;
	int test_num;
}Word;


/* For Dir_Manager class data type */
class Dir_Info {
	friend class Dir_Manager;
private:
	int upper_pid;
	wstring Dir_name;
	vector<int> child_pid;
	vector<wstring> file;

	bool operator==(Dir_Info new_data) {
		if (new_data.Dir_name != this->Dir_name)
			return false;
		else if (new_data.upper_pid != this->upper_pid)
			return false;
		else if (new_data.file != this->file)
			return false;
		return true;
	}
	void operator=(Dir_Info new_data) {
		this->upper_pid = new_data.upper_pid;
		this->Dir_name = new_data.Dir_name;
		this->child_pid = new_data.child_pid;
		this->file = new_data.file;
	}

	void clear() {
		this->Dir_name.clear();
		this->child_pid.clear();
		this->file.clear();
	}
};

typedef struct Dir_box {
	Dir_Info data;
	bool valid;
	bool useable;
}Dir_box;

// FileManager Update exclusive member : time consumming so not consider
static wchar_t * Dir_Exclusive[1] = {
	L" "
};

/* For Data Manager class data type */

class Data_Storage {
	friend class Data_Manager;
private:
	vector<vector<wstring>> Data;
	int start_pos;

};

typedef struct Data_box {

}Data_box;

/* Get the Error Handle function as external function */
extern void Make_Error_log();
extern void End_Error_log();
extern void Print_Error(wstring class_name, wstring message, bool exit = TRUE);