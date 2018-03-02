#include "Data_Manager.h"

int Data_Manager::Find_data(wstring data_name)
{
	return 0;
}

bool Data_Manager::File_copy(vector<wstring> file_path, wstring target_dir_path)
{
	return false;
}

bool Data_Manager::File_copy(wstring file_path, wstring target_dir_path)
{
	return false;
}

bool Data_Manager::File_merge(vector<wstring> file_path, wstring target_file_name, bool SAME_DIR, wstring target_dir_path)
{
	return false;
}

bool Data_Manager::File_delete(vector<wstring> file_path)
{
	return false;
}

bool Data_Manager::File_delete(wstring file_path)
{
	return false;
}

bool Data_Manager::File_Move(vector<wstring> file_path, wstring target_dir_path)
{
	return false;
}

bool Data_Manager::File_Move(wstring file_path, wstring target_dir_path)
{
	return false;
}

int Data_Manager::Read_File(wstring file_path, int begin_pos, int end_pos, bool Standard_decode, wstring Decoder_path)
{
	return 0;
}

bool Data_Manager::Write_File(wstring file_path, vector<int>& data_id, bool Standard_encode, wstring Decoder_path)
{
	return false;
}
