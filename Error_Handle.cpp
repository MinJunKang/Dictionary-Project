#include <cstdio>
#include <Windows.h>
#include <iostream>

using namespace std;

/* Error �α� ���� ���� */
void Make_Error_log()
{
	FILE * fp;
	fopen_s(&fp, "Error_log.txt", "wt");
	if (fp) {
		fwprintf(fp, L"[ �ۼ��Ͻ��� �����÷��� '����'�� �����ּ��� ]\n\n");
		fwprintf(fp, L"* This content will be posted to Developer by e-mail *\n (�� ������ �����ڿ��� e-mail�� ���۵˴ϴ�.)\n\n");

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
		fwprintf(fp, L"\n* ����� �ǰ�(���߿� ���� �˴ϴ�. �����Ӱ� �ۼ����ּ��� :) : \n");
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