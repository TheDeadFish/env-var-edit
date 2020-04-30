#include "editvar.h"
#include <windowsx.h>
#include <assert.h>

static
BOOL s_inEditUpdate;


static 
void EditVariableOk(PVARIABLE_DATA VarData, HWND hwndDlg)
{
    DWORD dwNameLength;
    DWORD dwValueLength;

    dwNameLength = (DWORD)SendDlgItemMessage(hwndDlg, IDC_VARIABLE_NAME, WM_GETTEXTLENGTH, 0, 0);
    dwValueLength = (DWORD)SendDlgItemMessage(hwndDlg, IDC_VARIABLE_VALUE, WM_GETTEXTLENGTH, 0, 0);
    if (dwNameLength > 0 && dwValueLength > 0)
    {
        LPTSTR p;

        if (VarData->lpName == NULL)
        {
            VarData->lpName = GlobalAlloc(GPTR, (dwNameLength + 1) * sizeof(TCHAR));
        }
        else if (_tcslen(VarData->lpName) < dwNameLength)
        {
            GlobalFree(VarData->lpName);
            VarData->lpName = GlobalAlloc(GPTR, (dwNameLength + 1) * sizeof(TCHAR));
        }
        SendDlgItemMessage(hwndDlg, IDC_VARIABLE_NAME, WM_GETTEXT, dwNameLength + 1, (LPARAM)VarData->lpName);

        if (VarData->lpRawValue == NULL)
        {
            VarData->lpRawValue = GlobalAlloc(GPTR, (dwValueLength + 1) * sizeof(TCHAR));
        }
        else if (_tcslen(VarData->lpRawValue) < dwValueLength)
        {
            GlobalFree(VarData->lpRawValue);
            VarData->lpRawValue = GlobalAlloc(GPTR, (dwValueLength + 1) * sizeof(TCHAR));
        }
        SendDlgItemMessage(hwndDlg, IDC_VARIABLE_VALUE, WM_GETTEXT, dwValueLength + 1, (LPARAM)VarData->lpRawValue);

        if (VarData->lpCookedValue != NULL)
        {
            GlobalFree(VarData->lpCookedValue);
            VarData->lpCookedValue = NULL;
        }

        p = _tcschr(VarData->lpRawValue, _T('%'));
        if (p && _tcschr(++p, _T('%')))
        {
            VarData->dwType = REG_EXPAND_SZ;
            VarData->lpCookedValue = GlobalAlloc(GPTR, 2 * MAX_PATH * sizeof(TCHAR));

            ExpandEnvironmentStrings(VarData->lpRawValue,
                                     VarData->lpCookedValue,
                                     2 * MAX_PATH);
        }
        else
        {
            VarData->dwType = REG_SZ;
            VarData->lpCookedValue = GlobalAlloc(GPTR, (dwValueLength + 1) * sizeof(TCHAR));
            _tcscpy(VarData->lpCookedValue, VarData->lpRawValue);
        }
    }
    EndDialog(hwndDlg, 1);
}

void EditVariableListInit(HWND hwndDlg)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    ListBox_ResetContent(hList);
    LPTSTR lpRawValue = getDlgItemText(hwndDlg, IDC_VARIABLE_VALUE);
    LPTSTR lpPos = lpRawValue;
    
    if(*lpPos)
    {
        do {
            LPTSTR lpNext = _tcschr(lpPos, _T(';'));
            if(lpNext) *lpNext = 0;
            ListBox_AddString(hList, lpPos);
            lpPos = lpNext;
        }while(lpPos++);
    }

    GlobalFree(lpRawValue);
}

void EditVariableListGet(HWND hwndDlg)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    
    /* get total length */
    DWORD dwLength = 0;
    int nItem = ListBox_GetCount(hList);
    for(int i = 0; i < nItem; i++)
        dwLength += ListBox_GetTextLen(hList, i)+1;
        
    /* build string */
    LPTSTR lpRawValue = stringAlloc(dwLength);
    dwLength = 0;
    for(int i = 0; i < nItem;) {
        dwLength += ListBox_GetText(hList, i, lpRawValue+dwLength);
        if(++i < nItem) lpRawValue[dwLength++] = ';';
    }
    
    /* update text */
    s_inEditUpdate = TRUE;
    SetDlgItemText(hwndDlg, IDC_VARIABLE_VALUE, lpRawValue);
    s_inEditUpdate = FALSE;
    GlobalFree(lpRawValue);
}

void EditVariableListSel(HWND hwndDlg)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    int iSel = ListBox_GetCurSel(hList);
    if(iSel >= 0) {
        LPTSTR lpLine = getListItemText(hList, iSel);
        SetDlgItemText(hwndDlg, IDC_ENVVAR_EDIT_PATH, lpLine);
        GlobalFree(lpLine);
    }
}

void EditVariableListAddSet(HWND hwndDlg, WPARAM wParam)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    LPTSTR lpLine = getDlgItemText(hwndDlg, IDC_ENVVAR_EDIT_PATH);
    if(LOWORD(wParam) == IDC_ENVVAR_EDIT_SET) {
        int iSel = ListBox_GetCurSel(hList);
        ListBox_SetText(hList, iSel, lpLine);
    } else {
        ListBox_AddString(hList, lpLine);
    } GlobalFree(lpLine);
    EditVariableListGet(hwndDlg);
}

void EditVariableListDel(HWND hwndDlg)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    int iSel = ListBox_GetCurSel(hList);
    if(iSel >= 0) {
        ListBox_DeleteString(hList, iSel);
        if(iSel >= ListBox_GetCount(hList)) iSel--;
        ListBox_SetCurSel(hList, iSel);
        EditVariableListSel(hwndDlg);
        EditVariableListGet(hwndDlg);
    }
}

void EditVariableListUpDw(HWND hwndDlg, WPARAM wParam)
{
    HWND hList = GetDlgItem(hwndDlg, IDC_ENVVAR_EDIT_LIST);
    int iSel = ListBox_GetCurSel(hList);
    ListBox_Move(hList, iSel, wParam == IDC_ENVVAR_EDIT_DOWN);
    EditVariableListGet(hwndDlg);    
}

void EditVariableListDir(HWND hwndDlg)
{
    TCHAR buff[MAX_PATH] = {};
    OPENFILENAME ofn = {sizeof(ofn), hwndDlg};
    ofn.lpstrFile = buff; ofn.nMaxFile = MAX_PATH;
    
    
    BOOL result;
    if(GetKeyState(VK_CONTROL) < 0) {        
        result = GetOpenFileName(&ofn);
    } else {
        _tcscpy(buff, _T("dummy.dummy"));
        ofn.lpstrTitle = _T("Select Folder");
        result = GetSaveFileName(&ofn);
        LPTSTR slash = _tcsrchr(buff, '\\');
        assert(slash); *slash = 0;
    }
    
    if(result) {
        SetDlgItemText(hwndDlg, IDC_ENVVAR_EDIT_PATH, buff);
    }
}

INT_PTR CALLBACK
EditVariableDlgProc(HWND hwndDlg,
                    UINT uMsg,
                    WPARAM wParam,
                    LPARAM lParam)
{
    PVARIABLE_DATA VarData;


    VarData = (PVARIABLE_DATA)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
    

    switch (uMsg)
    {
        case WM_SIZE:
            EnvVarsEdit_resizeSize(hwndDlg, wParam, lParam);
            break;
        
        case WM_DESTROY:
            EnvVarsEdit_resizeDestroy(hwndDlg);
            break;
    
        case WM_INITDIALOG:
            EnvVarsEdit_resizeInit(hwndDlg);
        
            SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG_PTR)lParam);
            VarData = (PVARIABLE_DATA)lParam;

            if (VarData->lpName != NULL)
            {
                SendDlgItemMessage(hwndDlg, IDC_VARIABLE_NAME, WM_SETTEXT, 0, (LPARAM)VarData->lpName);
            }

            if (VarData->lpRawValue != NULL)
            {
                SendDlgItemMessage(hwndDlg, IDC_VARIABLE_VALUE, WM_SETTEXT, 0, (LPARAM)VarData->lpRawValue);
            }
            
            EditVariableListInit(hwndDlg);
            
            
            break;

        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                    EditVariableOk(VarData, hwndDlg);
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hwndDlg, 0);
                    return TRUE;
                    
                case IDC_ENVVAR_EDIT_DIR:
                    EditVariableListDir(hwndDlg);
                    return TRUE;
                    
                case IDC_ENVVAR_EDIT_DEL:
                    EditVariableListDel(hwndDlg);
                    return TRUE;
                    
                case IDC_ENVVAR_EDIT_ADD:
                case IDC_ENVVAR_EDIT_SET:
                    EditVariableListAddSet(hwndDlg, wParam);
                    return TRUE;
                    
                case IDC_ENVVAR_EDIT_DOWN:
                case IDC_ENVVAR_EDIT_UP:
                    EditVariableListUpDw(hwndDlg, wParam);
                    return TRUE;
                    
                case MAKELONG(IDC_ENVVAR_EDIT_LIST, LBN_SELCHANGE):
                    EditVariableListSel(hwndDlg);
                    return TRUE;
                    
                case MAKELONG(IDC_VARIABLE_VALUE, EN_CHANGE):
                    if(!s_inEditUpdate) EditVariableListInit(hwndDlg);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}
