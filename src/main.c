#include <windows.h>
#include "resource.h"
HMODULE hApplet;

INT_PTR CALLBACK
EnvironmentDlgProc(HWND hwndDlg,
                   UINT uMsg,
                   WPARAM wParam,
                   LPARAM lParam);


int main()
{
	hApplet = GetModuleHandle(NULL);
	DialogBox(hApplet,
		MAKEINTRESOURCE(IDD_ENVIRONMENT_VARIABLES),
		NULL, EnvironmentDlgProc);














}
