#pragma once
#include "precomp.h"

typedef struct _VARIABLE_DATA
{
    DWORD dwType;
    LPTSTR lpName;
    LPTSTR lpRawValue;
    LPTSTR lpCookedValue;
	
} VARIABLE_DATA, *PVARIABLE_DATA;

INT_PTR CALLBACK
EditVariableDlgProc(HWND hwndDlg,
                    UINT uMsg,
                    WPARAM wParam,
                    LPARAM lParam);


EXTERN_C void EnvVarsEdit_resizeInit(HWND hDlg);
EXTERN_C void EnvVarsEdit_resizeSize(HWND hDlg, WPARAM wParam, LPARAM lParam);
EXTERN_C void EnvVarsEdit_resizeDestroy(HWND hDlg);
