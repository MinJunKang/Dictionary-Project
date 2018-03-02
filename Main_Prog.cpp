#include <iostream>
#include "Console_design.h"
#include "Dir_Manager.h"

using namespace std;

wstring To_wstr(const string & str)
{
	int required_len = str.length();

	wchar_t * dest = new wchar_t[required_len];

	mbstowcs(dest, str.c_str(), required_len);

	wstring converted_str(dest);

	return converted_str;
}

string To_str(const wstring & wstr)
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

void main(int argc,char * argv[])
{
	/* set locale version to kor */
	_wsetlocale(LC_ALL, L"korean");
	wcout.imbue(locale("korean"));
	wcin.imbue(locale("korean"));

	/* setting for console */
	SetConsoleTitle(L"Easy_Dic_ver1.3");

	/* Generate error log */
	Make_Error_log();

	
	/* find dir folder */
	wstring root_dir_path = To_wstr(argv[0]);
	wstring not_path = L"\\Debug\\Easy_Dic_ver1.3.exe";
	root_dir_path = root_dir_path.substr(0, root_dir_path.find(not_path, 0));

	int start = clock();
	//Dir_Manager file_m(L"C:\\Users\\minjun\\Desktop\\Data\\Programming\\C언어 실험\\boost_1_65_1\\boost_1_65_1");
	Dir_Manager file_m(root_dir_path);
	double end_time = (clock() - start) / CLOCKS_PER_SEC;

	/* start update */
	file_m.Update_begin();

	design_tool sc1(30, 10, 14, 34, L"파일 찾기");
	sc1.AddMessage(L"입력 : ", 33, 17, -1);
	sc1.AddMessage(to_wstring(end_time), 33, 19, -2);
	sc1.Addbutton(L"진행", 47, 20, 2);

	design_tool sc2(30, 10, 14, 34, L"진행 확인");
	sc2.Addbutton(L"진행", 47, 20, 2);
	sc2.Addbutton(L"중지", 67, 20, 3);

	int option = 0;

	while (option != 3) {
		wstring input;
		sc1.Printdesign(1, &input, 40, 17);

		if (file_m.Find_dir(input, TRUE) != -1)
		{
			wcout << file_m.Get_Root_Path() << endl;
			_sleep(2000);
			file_m.Go_upper();
			file_m.Delete_dir(input);
		}
		else {
			file_m.Create_dir(input);
		}
		option = sc2.Printdesign(0);
		sc2.DeleteMessage(-2);
	}

	/* exit Update */
	file_m.Update_pause();

	/* exit error log */
	End_Error_log();
}