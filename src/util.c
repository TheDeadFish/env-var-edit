#include "editvar.h"
#include <windowsx.h>

LPTSTR stringAlloc(DWORD dwLength)
{
  return GlobalAlloc(GPTR, (dwLength + 1) * sizeof(TCHAR));
}

LPTSTR getDlgItemText(HWND hwndDlg, UINT idItem)
{
    DWORD dwLength = (DWORD)SendDlgItemMessage(hwndDlg, idItem, WM_GETTEXTLENGTH, 0, 0);
    LPTSTR p = stringAlloc(dwLength);
    SendDlgItemMessage(hwndDlg, idItem, WM_GETTEXT, dwLength + 1, (LPARAM)p);
    return p;
}

LPTSTR getListItemText(HWND hwndDlg, UINT idItem)
{
    DWORD dwLength = ListBox_GetTextLen(hwndDlg, idItem);
    LPTSTR p = stringAlloc(dwLength);
    ListBox_GetText(hwndDlg, idItem, p);
    return p;
}

void ListBox_SetText(HWND hList, int iItem, LPTSTR str)
{
  if(iItem >= 0) {
    int nSel = ListBox_GetCurSel(hList);
    ListBox_DeleteString(hList, iItem);
    ListBox_InsertString(hList, iItem, str);
    ListBox_SetCurSel(hList, nSel);
  }
}

void ListBox_Move(HWND hList, int iItem, BOOL down)
{
  if(iItem < 0) return; int iIns = iItem+1;
  if(!down) { if(!iItem) return; iIns = iItem-1; }
  LPTSTR p = getListItemText(hList, iItem);
  ListBox_DeleteString(hList, iItem);
  ListBox_InsertString(hList, iIns, p);
  ListBox_SetCurSel(hList, iIns);
  GlobalFree(p);
}
