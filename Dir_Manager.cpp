#include "Dir_Manager.h"

int Dir_Manager::AllocDir(Dir_Info data)
{
	/* Check if it is new or not */
	for (int i = 0; i < DirTable.size(); i++)
	{
		if (!DirTable[i].useable) 
		{
			if (DirTable[i].data == data) {
				DirTable[i].valid = 0;
				DirTable[i].useable = 0;
				return i;
			}
		}
	}

	/* Check if there is free space */
	for (int i = 0; i < DirTable.size(); i++)
	{
		if (DirTable[i].useable)
		{
			DirTable[i].data = data;
			DirTable[i].valid = 0;
			DirTable[i].useable = 0;
			return i;
		}
	}

	/* All space all fulled create one*/
	Dir_box new_dir;
	new_dir.data = data;
	new_dir.valid = 0;
	new_dir.useable = 0;
	DirTable.push_back(new_dir);

	return DirTable.size() - 1;
}

int Dir_Manager::Update_dir(wstring path, int parent_pid)
{
	auto path_info = FILE_Path(path);
	auto file_info = Get_file(path_info);
	auto dir_info = Get_dir(path_info);

	wstring dir_split = L"/";
	Dir_Info new_data;

	/* Dir name save */
	if (path == Upper_bound_path) {
		new_data.Dir_name = dir_split;
	}
	else {
		new_data.Dir_name = Path2name(path) + dir_split;
	}

	/* File save */
	for (int i = 0; i < file_info.size(); i++) {
		new_data.file.push_back(file_info[i]);
	}

	/* upper pid save */
	new_data.upper_pid = parent_pid;

	int current_pid = AllocDir(new_data);

	for (int i = 0; i < dir_info.size(); i++) {
		if (Check_Exclusive(dir_info[i])) {
			Update_dir(path + dir_split + dir_info[i],current_pid);
		}
	}

	return current_pid;
}

void Dir_Manager::Update_child()
{
	for (int i = 1; i < DirTable.size(); i++) {
		if (DirTable[i].useable == 0) {
			DirTable[DirTable[i].data.upper_pid].data.child_pid.push_back(i);
		}
	}
}

bool Dir_Manager::Check_Exclusive(wstring dir_name)
{

	for (int i = 0; i < sizeof(Dir_Exclusive) / sizeof(wchar_t*); i++) {
		if (dir_name == Dir_Exclusive[i])
			return false;
	}

	return true;
}

vector<wstring> Dir_Manager::Get_file(vector<pair<wstring, bool>> filename)
{
	vector<wstring> file;

	for (int i = 0; i < filename.size(); i++) {
		if (filename[i].second) {
			file.push_back(filename[i].first);
		}
	}

	return file;
}

vector<wstring> Dir_Manager::Get_dir(vector<pair<wstring, bool>> filename)
{
	vector<wstring> dir;

	for (int i = 0; i < filename.size(); i++) {
		if (!filename[i].second) {
			dir.push_back(filename[i].first);
		}
	}

	return dir;
}

wstring Dir_Manager::To_wstr(const string & str)
{
	int required_len = str.length();

	wchar_t * dest = new wchar_t[required_len];

	mbstowcs(dest, str.c_str(), required_len);

	wstring converted_str(dest);

	return converted_str;
}

string Dir_Manager::To_str(const wstring & wstr)
{
	int required_len = 0;

	for (int i = 0; i < wstr.length(); i++) {
		if (wstr[i] & 0x80)
			required_len += 2;
		else
			required_len += 1;
	}
	required_len++;

	char * dest = new char[required_len];

	wcstombs(dest, wstr.c_str(), required_len);

	dest[required_len] = '\0';

	string converted_str(dest);

	return converted_str;
}

wstring Dir_Manager::To_stdPath(wstring & path)
{
	wstring path_format = L"/";
	wstring non_format = L"\\";

	size_t start_pos = 0;

	while ((start_pos = path.find(non_format, start_pos)) != std::string::npos) {
		path.replace(start_pos, non_format.length(), path_format);
		start_pos += path_format.length(); // ...
	}

	return path;
}

vector<pair<wstring, bool>> Dir_Manager::FILE_Path(wstring foldername)
{
	vector<pair<wstring, bool>> filename;

	_wfinddatai64_t fd;
	long long handle;

	int result = 1;
	int is_File = 0;
	int count = 0;
	wstring path_back = L".\\**";
	wstring path_split = L"/";

	handle = _wfindfirsti64((foldername + path_back).c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1) // 폴더 안에 파일이 없는 경우
	{
		return filename;
	}

	while (result != -1)
	{
		if (count != 0 && count != 1)
		{
			is_File = isFileorDir(foldername + path_split + fd.name);
			filename.push_back(make_pair(fd.name, is_File));
		}
		result = _wfindnexti64(handle, &fd);
		count++;
	}
	_findclose(handle);
	return filename;
}

wstring Dir_Manager::Path2name(wstring path)
{
	vector<wstring> splitted_path = Split_path(path);

	int num = splitted_path.size();

	if (num >= 1)
		return splitted_path.at(num - 1);
	else {
		Print_Error(L"Dir_Manager", L"Path2name : invalid path");
		return L"";
	}
}

int Dir_Manager::Distance(int pid_1, int pid_2)
{
	vector<int> path_1;
	vector<int> path_2;

	int d1 = 0;
	int d2 = 0;

	int case_ = 0;

	int up_pid = pid_1;

	/* get pid_1 path */
	while (1) {
		path_1.push_back(up_pid);

		up_pid = DirTable[up_pid].data.upper_pid;
		if (up_pid == -1) {
			break;
		}
	}

	up_pid = pid_2;

	/* get pid_2 path */
	while (1) {
		path_2.push_back(up_pid);

		up_pid = DirTable[up_pid].data.upper_pid;
		if (up_pid == -1) {
			break;
		}
	}

	for (int i = 0; i < path_1.size(); i++) {
		for (int j = 0; j < path_2.size(); j++) {
			if (path_1[i] == path_2[j]) {
				d1 = i;
				d2 = j;
			}
			if (path_1[i] == pid_2)
				case_ = 1;
			if (path_2[j] == pid_1)
				case_ = 1;

		}
	}

	if (case_) {
		return abs(d1 - d2);
	}
	else {
		return d1 + d2;
	}
}

int Dir_Manager::Path2pid(wstring path)
{
	path = To_stdPath(path);

	vector<wstring> splitted_path = Split_path(path);
	vector<wstring> splitted_bound_path = Split_path(Upper_bound_path);

	if (splitted_path.size() < splitted_bound_path.size())
		Print_Error(L"Dir_Manager", L"Path2pid : path is not valid");

	vector<wstring> valid_path;

	for (int i = splitted_bound_path.size(); i < splitted_path.size(); i++) {
		valid_path.push_back(splitted_path[i] + L"/");
	}

	int child_pid = 0;

	for (int i = 0; i < valid_path.size(); i++) {
		for (int j = 0; j < DirTable[child_pid].data.child_pid.size(); j++) {
			if (DirTable[DirTable[child_pid].data.child_pid[j]].data.Dir_name == valid_path[i])
			{
				child_pid = DirTable[child_pid].data.child_pid[j];
				break;
			}
			Print_Error(L"Dir_Manager", L"Path2pid : path is not valid");
		}
	}

	return child_pid;
}

wstring Dir_Manager::pid2Path(int dir_pid)
{
	/* If there is no corresponding pid */
	if (dir_pid >= this->DirTable.size() || dir_pid < 0) {
		Print_Error(L"Dir_Manager", L"pid2Path : Invalid dir pid");
		return false;
	}

	if (this->DirTable[dir_pid].useable) {
		Print_Error(L"Dir_Manager", L"pid2Path : There is no dir");
		return false;
	}

	vector<int> path_pid;

	wstring path = L"";

	int up_pid = dir_pid;

	while (1) {
		path_pid.push_back(up_pid);

		up_pid = DirTable[up_pid].data.upper_pid;
		if (up_pid == -1) {
			break;
		}
	}

	reverse(path_pid.begin(), path_pid.end());

	for (int i = 0; i < path_pid.size(); i++) {
		path = path + DirTable[path_pid[i]].data.Dir_name;
	}

	path = Upper_bound_path + path;

	return path;
}

vector<wstring> Dir_Manager::Split_path(wstring path)
{
	vector<wstring> splitted_result;

	wstring split_char = L"/";

	size_t pos = 0;
	size_t p_pos = 0;

	path = To_stdPath(path);

	while (1) {
		pos = path.find(split_char, pos + 1);
		if (pos == string::npos) {
			if (path.length() - p_pos != 0) {
				splitted_result.push_back(path.substr(p_pos, path.length() - p_pos));
			}
			break;
		}
		else {
			splitted_result.push_back(path.substr(p_pos, pos - p_pos));
			p_pos = pos + 1;
		}
	}

	return splitted_result;
}

int Dir_Manager::isFileorDir(wstring name)
{
	_wfinddatai64_t c_file;
	intptr_t hFile;
	int result;


	if ((hFile = _wfindfirsti64(name.c_str(), &c_file)) == -1L)
		result = -1; // 파일 또는 디렉토리가 없으면 -1 반환
	else
		if (c_file.attrib & _A_SUBDIR)
			result = 0; // 디렉토리면 0 반환
		else
			result = 1; // 그밖의 경우는 "존재하는 파일"이기에 1 반환


	_findclose(hFile);

	return result;
}

void Dir_Manager::Update_freq()
{
	int startTime, elapseTime;

	startTime = clock();
	mtx.lock();
	while (update_state) {
		
		elapseTime = clock() - startTime;
		
		if (elapseTime / CLOCKS_PER_SEC == this->update_interval) {
			startTime = clock();
			
			Update();
			
		}

	}
	mtx.unlock();
}



Dir_Manager::Dir_Manager(wstring Upper_bound_path):update_state(0), update_interval(5)
{
	/* To check if it is valid or not */
	_wfinddatai64_t c_file;

	if (_wfindfirsti64(Upper_bound_path.c_str(), &c_file) == -1L) {
		Print_Error(L"Dir_Manager", L"Constructor : Upper_bound_path is not valid");
	}

	this->Upper_bound_path = To_stdPath(Upper_bound_path);

	/* set current_root_pid */
	this->current_root_pid = 0;

	/* Make the dir/file list and measure time consumed*/
	int start = clock();
	Update();
	this->update_interval = (clock() - start) / CLOCKS_PER_SEC * 2;
	if (this->update_interval == 0)
		this->update_interval = 5;
}

void Dir_Manager::Set_bound_path(wstring Upper_bound_path)
{
	int state = 0;

	/* If it is updating stop it */
	if (update_state) {
		state = 1;
		Update_pause();
	}

	/* To check if it is valid or not */
	_wfinddatai64_t c_file;

	if (_wfindfirsti64(Upper_bound_path.c_str(), &c_file) == -1L) {
		Print_Error(L"Dir_Manager", L"Constructor : Upper_bound_path is not valid");
	}

	this->Upper_bound_path = To_stdPath(Upper_bound_path);

	for (int i = 0; i < this->DirTable.size(); i++) {
		this->DirTable[i].data.clear();
	}
	this->DirTable.clear();

	/* set current_root_pid */
	this->current_root_pid = 0;
	
	/* Make the dir/file list */
	Update();

	/* Resume update */
	if (state)
		Update_begin();
}


int Dir_Manager::Get_Root_pid()
{
	return this->current_root_pid;
}

wstring Dir_Manager::Get_Root_Path()
{
	return pid2Path(this->current_root_pid);
}

int Dir_Manager::Set_Root_pid(int dir_pid)
{
	/* If there is no corresponding pid */
	if (dir_pid >= this->DirTable.size() || dir_pid < 0) {
		Print_Error(L"Dir_Manager", L"Set_Root : Invalid dir pid");
		return false;
	}

	if (this->DirTable[dir_pid].useable) {
		Print_Error(L"Dir_Manager", L"Set_Root : There is no dir");
		return false;
	}

	this->current_root_pid = dir_pid;

	return 0;
}

int Dir_Manager::Find_path(wstring path,bool set_root)
{
	int pid = this->Path2pid(path);

	if (set_root)
		Set_Root_pid(pid);

	return pid;
}

int Dir_Manager::Find_dir(wstring dir_name, bool set_root)
{
	int pid = -1;
	int distance = DirTable.size();

	for (int i = 0; i < DirTable.size(); i++)
	{
		if (DirTable[i].useable != 0)
			continue;
		if (Path2name(DirTable[i].data.Dir_name) == Path2name(dir_name))
		{
			if (distance >= Distance(this->current_root_pid, i)) {
				pid = i;
				distance = Distance(this->current_root_pid, i);
			}
		}
	}

	if (set_root && pid !=-1)
		Set_Root_pid(pid);

	return pid;
}

vector<wstring> Dir_Manager::ls()
{
	vector<wstring> under_dir;

	for (int i = 0; i < DirTable[current_root_pid].data.child_pid.size(); i++) {
		under_dir.push_back(DirTable[DirTable[current_root_pid].data.child_pid[i]].data.Dir_name);
	}

	return under_dir;
}

int Dir_Manager::Go_upper()
{
	if (this->current_root_pid != 0) {
		this->current_root_pid = DirTable[current_root_pid].data.upper_pid;
	}

	return this->current_root_pid;;
}

int Dir_Manager::Go_lower(wstring dir_name)
{
	for (int i = 0; i < DirTable[current_root_pid].data.child_pid.size(); i++)
	{
		if (DirTable[DirTable[current_root_pid].data.child_pid[i]].data.Dir_name == dir_name)
		{
			this->current_root_pid = DirTable[current_root_pid].data.child_pid[i];
			break;
		}
	}

	return this->current_root_pid;
}

int Dir_Manager::Go_lower(int pos)
{
	if (pos < DirTable[current_root_pid].data.child_pid.size() && pos >= 0)
	{
		this->current_root_pid = DirTable[current_root_pid].data.child_pid[pos];
	}

	return this->current_root_pid;
}

vector<wstring> Dir_Manager::Get_Root_file()
{
	vector<wstring> files = this->DirTable[this->current_root_pid].data.file;

	wstring root_path = Get_Root_Path();

	for (int i = 0; i < files.size(); i++) {
		files[i] = root_path + files[i];
	}

	return files;
}

void Dir_Manager::Update()
{
	/* Save current_path's information */
	Dir_Info path_data;

	if (this->current_root_pid != 0) {
		path_data = DirTable[this->current_root_pid].data;
	}

	/* Variable for current_root */
	int if_changed = 0;

	/* Initialization */
	for (int i = 0; i < DirTable.size(); i++){
		DirTable[i].valid = 1;
	}

	/* Do update */
	Update_dir(Upper_bound_path,-1);

	/* Garbage Collector */
	for (int i = 0; i < DirTable.size(); i++) {
		if (DirTable[i].valid) {
			DirTable[i].useable = 1;
		}
		else {
			DirTable[i].useable = 0;
		}
	}

	for (int i = DirTable.size() - 1; i >= 0; i--) {
		if (DirTable[i].useable == 0)
			break;
		else
			DirTable.pop_back();
	}

	/* Update child_dir_pid */
	Update_child();

	/* Update current root pid */
	if (this->current_root_pid != 0) {
		for (int i = 0; i < DirTable.size(); i++)
		{
			if (DirTable[i].useable == 0)
			{
				if (DirTable[i].data == path_data) {
					this->current_root_pid = i;
					if_changed = 1;
					break;
				}
			}
		}
	}

	if (!if_changed)
		this->current_root_pid = 0; // no matched , initialized
}

void Dir_Manager::Update_begin(int time_interval)
{
	if(time_interval != 5)
		this->update_interval = time_interval;
	this->update_state = 1;
	m_th = thread(&Dir_Manager::Update_freq,this);

}

void Dir_Manager::Update_pause()
{
	this->update_state = 0;
	while (!m_th.joinable()){} // wait until thread is joinable
	if (m_th.joinable()) {
		m_th.join();
	}
}

bool Dir_Manager::Create_dir(wstring dir_name)
{
	int state = 0;

	/* If it is updating stop it */
	if (update_state) {
		state = 1;
		Update_pause();
	}

	int nResult = mkdir(To_str(Get_Root_Path() + dir_name).c_str());


	if (nResult == 0)
	{
		/* Make the dir/file list */
		Update();

		/* Resume update */
		if (state)
			Update_begin();

		return true;
	}
	else if (nResult == -1)
	{
		// 13 : not valid, 17 : already exists
		if(errno == 17)
			Print_Error(L"Dir_Manager,Create_dir:", dir_name + L" Dir already exists", FALSE);
		else if(errno == 13)
			Print_Error(L"Dir_Manager,Create_dir:", dir_name + L"'s path is not valid", FALSE);
		else
			Print_Error(L"Dir_Manager,Create_dir:", dir_name + L": Unknown Error code : " + to_wstring(errno));

		/* Resume update */
		if (state)
			Update_begin();

		return false;
	}
}

bool Dir_Manager::Delete_dir(wstring dir_name)
{
	int state = 0;

	/* If it is updating stop it */
	if (update_state) {
		state = 1;
		Update_pause();
	}

	int nResult = rmdir(To_str(Get_Root_Path() + dir_name).c_str());

	wcout << Get_Root_Path() + dir_name << endl;
	_sleep(1000);

	if (nResult == 0)
	{
		/* Make the dir/file list */
		Update();

		/* Resume update */
		if (state)
			Update_begin();

		return true;
	}
	else if (nResult == -1)
	{
		// 13 : not valid, 17 : already exists
		if (errno == 2)
			Print_Error(L"Dir_Manager,Delete_dir:", dir_name + L" Dir not exists", FALSE);
		else if (errno == 41)
			Print_Error(L"Dir_Manager,Delete_dir:", dir_name + L" is not empty dir", FALSE);
		else
			Print_Error(L"Dir_Manager,Delete_dir:", dir_name + L": Unknown Error code : " + to_wstring(errno));

		/* Resume update */
		if (state)
			Update_begin();

		return false;
	}
}
