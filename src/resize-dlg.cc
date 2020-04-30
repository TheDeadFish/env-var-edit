#include "stdshit.h"
#include "resize.h"
#include <commctrl.h>
#include "resource.h"

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


static WndResize resize2;
const WndResize::CtrlDef crtlLst2[] = { 
	{IDC_VARIABLE_NAME, HOR_BOTH}, {IDC_VARIABLE_VALUE, HOR_BOTH},
	{IDC_ENVVAR_EDIT_PATH, HOR_BOTH}, {IDC_ENVVAR_EDIT_DIV, HOR_BOTH},
	{IDC_ENVVAR_EDIT_DIR, HOR_RIGH}, {IDC_ENVVAR_EDIT_LIST, HVR_BOTH},
	{IDC_ENVVAR_EDIT_UP, VER_BOTT}, {IDC_ENVVAR_EDIT_DOWN, VER_BOTT},
	{IDC_ENVVAR_EDIT_ADD, VER_BOTT}, {IDC_ENVVAR_EDIT_SET, VER_BOTT},
	{IDC_ENVVAR_EDIT_DEL, VER_BOTT},  {IDOK, VER_BOTT|HOR_RIGH},
	{IDCANCEL, VER_BOTT|HOR_RIGH}
};

EXTERN_C void EnvVarsEdit_resizeInit(HWND hDlg) 
{
	new (&resize2)WndResize(); 
	resize2.init(hDlg);
	resize2.add(hDlg, crtlLst2, 13);
}

EXTERN_C void EnvVarsEdit_resizeSize(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	resize2.resize(hDlg, wParam, lParam);
}

EXTERN_C void EnvVarsEdit_resizeDestroy(HWND hDlg)
{
	resize2.~WndResize();
}
