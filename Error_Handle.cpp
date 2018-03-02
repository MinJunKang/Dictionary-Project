#include <cstdio>
#include <Windows.h>
#include <iostream>

using namespace std;

/* Error 로그 파일 생성 */
void Make_Error_log()
{
	FILE * fp;
	fopen_s(&fp, "Error_log.txt", "wt");
	if (fp) {
		fwprintf(fp, L"[ 작성하시지 않으시려면 '종료'를 눌러주세요 ]\n\n");
		fwprintf(fp, L"* This content will be posted to Developer by e-mail *\n (이 내용은 개발자에게 e-mail로 전송됩니다.)\n\n");

		fwprintf(fp, L"* Error Message Log\n----------------------\n");

		fclose(fp);
	}
}

void End_Error_log()
{
	FILE * fp;
	fopen_s(&fp, "Error_log.txt", "at");
	if (fp) {

		fwprintf(fp, L"\n----------------------\n");
		fwprintf(fp, L"\n* 사용자 의견(개발에 참고가 됩니다. 자유롭게 작성해주세요 :) : \n");
		fclose(fp);
	}
	exit(1);
}

// Error print
void Print_Error(wstring class_name, wstring message, bool exit = TRUE)
{
	FILE * fp;

	fopen_s(&fp, "Error_log.txt", "at");

	if (fp) {
		fwprintf(fp, L"\nError : Class = %s, Content = %s \n", class_name.c_str(), message.c_str());
		fclose(fp);
	}

	if (exit)
		End_Error_log();
}