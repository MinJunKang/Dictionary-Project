#pragma once

#include "defs.h"

using namespace std;

class Data_Manager {
private:

public:
	/* Data I/O */
	template <typename T>
	int Reg_data(vector<vector<T>> Raw_data,wstring data_name);

	int Find_data(wstring data_name);

	/* File Manager's function */
	bool File_copy(vector<wstring> file_path, wstring target_dir_path);
	bool File_copy(wstring file_path, wstring target_dir_path);

	bool File_merge(vector<wstring> file_path, wstring target_file_name, bool SAME_DIR = TRUE, wstring target_dir_path = L"");
	
	bool File_delete(vector<wstring> file_path);
	bool File_delete(wstring file_path);

	bool File_Move(vector<wstring> file_path, wstring target_dir_path);
	bool File_Move(wstring file_path, wstring target_dir_path);

	/* File I/O */
	int Read_File(wstring file_path, int begin_pos, int end_pos, bool Standard_decode = TRUE, wstring Decoder_path = L"");

	bool Write_File(wstring file_path, vector<int> & data_id, bool Standard_encode = TRUE, wstring Decoder_path = L"");


};

template<typename T>
inline int Data_Manager::Reg_data(vector<vector<T>> Raw_data, wstring data_name)
{
	return 0;
}
