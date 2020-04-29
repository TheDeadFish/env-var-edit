#include "stdshit.h"
#include "resize.h"
#include <commctrl.h>

const char progName[] = "sysdm.cpl";

static WndResize resize;
const WndResize::CtrlDef crtlLst[] = { 
	{411, VER(2, 1) | HOR_BOTH}, {402, VER(2, 1) | HOR_BOTH},
	{412, VERPOS(1, 2) | HOR_RIGH}, {413, VERPOS(1, 2) | HOR_RIGH},
	{414, VERPOS(1, 2) | HOR_RIGH}, {415, VER(2, 2) | HOR_BOTH},
	{400, VER(2, 2) | HOR_BOTH}, {407, HOR_RIGH | VER_BOTT}, 
	{408, HOR_RIGH | VER_BOTT},{409, HOR_RIGH | VER_BOTT},
	{1, HOR_RIGH | VER_BOTT}, {2, HOR_RIGH | VER_BOTT}};
	
void resize_list(HWND hwnd)
{
	HWND hList1 = GetDlgItem(hwnd, 402);
	ListView_SetColumnWidth(hList1, 0, LVSCW_AUTOSIZE);
	ListView_SetColumnWidth(hList1, 1, LVSCW_AUTOSIZE_USEHEADER);
	HWND hList2 = GetDlgItem(hwnd, 400);
	ListView_SetColumnWidth(hList2, 0, LVSCW_AUTOSIZE);
	ListView_SetColumnWidth(hList2, 1, LVSCW_AUTOSIZE_USEHEADER);
}
	
extern "C" int __stdcall EnvVarsDlgProc(HWND hDlg,
	UINT uMsg, WPARAM wParam, LPARAM lParam);
extern "C" int __stdcall EnvVarsDlgProc_hook(HWND hDlg,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg) {
	case WM_INITDIALOG:
		new (&resize)WndResize(); 
		resize.init(hDlg);
		resize.add(hDlg, crtlLst, 12);
		break;
	case WM_DESTROY:
		resize.~WndResize();
		break;
	case WM_SIZE:
		resize.resize(hDlg, wParam, lParam);
		resize_list(hDlg);
		break;
	}
	
	ARGFIX(hDlg); ARGFIX(uMsg);
	ARGFIX(wParam); ARGFIX(lParam);	
	return EnvVarsDlgProc(hDlg,
		uMsg, wParam, lParam);
}
