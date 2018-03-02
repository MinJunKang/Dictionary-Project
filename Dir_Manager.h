#pragma once

#include "defs.h"
#include "Thread.h"

using namespace std;


/* function */
/*

root directory를 갱신

1. 특정 directory 안의 모든 파일 이름, 확장자를 저장(p tree 구조)
2. Update
3. Fast Finder(특정 파일을 빠르게 찾는 기능)

모든 경로는 상대경로
.add root_directory
.get_pid(wstring * folder_name) // int형 반환 특정 directory의 pid 반환
.update(wstring * root_directory) // filename 하의 모든 폴더를 tree화 한다.

*/

class Dir_Manager {
private:
	vector<Dir_box> DirTable;
	int AllocDir(Dir_Info data);

	int Update_dir(wstring path, int parent_pid);
	void Update_child();
	bool Check_Exclusive(wstring dir_name);

	vector<wstring> Get_file(vector<pair<wstring, bool>> filename);
	vector<wstring> Get_dir(vector<pair<wstring, bool>> filename);
	vector<pair<wstring, bool>> FILE_Path(wstring fordername); // 폴더 내의 파일을 모두 찾는다.
	int isFileorDir(wstring name);

	wstring To_wstr(const string & str);
	string To_str(const wstring & wstr);
	wstring To_stdPath(wstring & path);
	
	wstring Path2name(wstring path);
	int Path2pid(wstring path);
	wstring pid2Path(int dir_pid);
	vector<wstring> Split_path(wstring path);

	int Distance(int pid_1, int pid_2);
	
	int current_root_pid;
	wstring Upper_bound_path;
	

	void Update_freq();
	/*
		Contents:
		1. run Update() for every minute
		2. for thread
	*/

	void Update();
	/*
		Contents :
		1. Update all the hierarchy and rearrange it
		2. If the current path exits after update, maintain it
		3. Otherwise, it will be reset to highest path
	*/

	int update_state; // for thread
	thread m_th; // for threading
	mutex mtx; // for threading

	int update_interval;
public:

	Dir_Manager(wstring Upper_bound_path);

	void Set_bound_path(wstring Upper_bound_path);
	/*
		Contents : 
		Change upper_bound_path
	*/
	
	/* Get Root */
	int Get_Root_pid();
	/*
		Contents :
		Get current root_dir's pid
	*/

	/* Get Root path */
	wstring Get_Root_Path();
	/*
		Contents :
		Get current root_dir's path
	*/

	/* Set pid's path to root */
	int Set_Root_pid(int dir_pid); //**
	/*
		Contents :
		Set dir_pid's dir to root dir
	*/

	/* Find path's pid */
	int Find_path(wstring path, bool set_root = 0); // **
	/*
		Contents :
		1. Find path's directory
		2. Return pid if it hits, otherwise -1
		3. set to root or not is determined by 3rd parameter
	*/

	/* Find dir name's pid */
	int Find_dir(wstring dir_name, bool set_root = 0); //**
	/*
		Contents :
		1. Find dir_name's directory
		2. if it success, return dir_name's pid, otherwise return -1
		3. set to root or not is determined by 3rd parameter
		4. Return closest pid if there is two or more dir
	*/

	/* Show Current dir's under dir */
	vector<wstring> ls();
	/*
		Contents :
		1. Show all the dir name of current root's below
	*/

	int Go_upper(); 
	/*
		Contents :
		Go to current root dir's upper dir
		1. if it success, return upper dir's pid otherwise return -1 (it means it's the upper bound dir)
	*/

	int Go_lower(wstring dir_name);
	/*
		Contents :
		Go to current root dir's posth lower dir(by name)
		1. if it success, return lower dir's pid otherwise return -1 (it means there's no matched dir to pid)
	*/

	int Go_lower(int pos);
	/*
		Contents :
		Go to current root dir's posth lower dir(by pos)
		1. if it success, return lower dir's pid otherwise return -1 (it means there's no matched dir to pid)
	*/

	vector<wstring> Get_Root_file();
	/*
		Contents :
		1. Find all files in current root dir and return them as path
	*/

	void Update_begin(int time_interval = 5);
	/*
		Contents :
		1. Start Update(time interval : seconds)
	*/

	void Update_pause();
	/*
		Contents :
		1. Pause Update
	*/

	bool Create_dir(wstring dir_name);
	/*
		Contents :
		1. Create new directory under the current root path
		2. If creation is successful, return true otherwise false
	*/

	bool Delete_dir(wstring dir_name);
	/*
		Contents :
		1. Delete directory under the current root path
		2. If deletion is successful, return true otherwise false
		3. Deletion is only possible for empty directory
	*/
};
