#include "editvar.h"

INT_PTR CALLBACK
EditVariableDlgProc(HWND hwndDlg,
                    UINT uMsg,
                    WPARAM wParam,
                    LPARAM lParam)
{
    PVARIABLE_DATA VarData;
    DWORD dwNameLength;
    DWORD dwValueLength;

    VarData = (PVARIABLE_DATA)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
    

    switch (uMsg)
    {
        case WM_INITDIALOG:
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
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
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
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hwndDlg, 0);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}
