#ifndef __CPL_PRECOMP_H
#define __CPL_PRECOMP_H
#include <windows.h>
#include <tchar.h>
#include <Commctrl.h>

#include "resource.h"

#define NUM_APPLETS (1)

typedef LONG (CALLBACK *APPLET_INITPROC)(VOID);

typedef struct _APPLET
{
  int idIcon;
  int idName;
  int idDescription;
  APPLET_INITPROC AppletProc;
} APPLET, *PAPPLET;

extern HINSTANCE hApplet;

void ShowLastWin32Error(HWND hWndOwner);

/* Prop sheet pages */
INT_PTR CALLBACK GeneralPageProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK HardwarePageProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK AdvancedPageProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* Dialogs */
INT_PTR CALLBACK HardProfDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK UserProfileDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK EnvironmentDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK StartRecDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK VirtMemDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK LicenceDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

/* System information */
BOOL GetSystemName(PWSTR pBuf, SIZE_T cchBuf);

typedef struct _PAGEFILE
{
    TCHAR  szDrive[3];
    LPTSTR pszVolume;
    INT    OldMinSize;
    INT    OldMaxSize;
    INT    NewMinSize;
    INT    NewMaxSize;
    UINT   FreeSize;
    BOOL   bUsed;
} PAGEFILE, *PPAGEFILE;

typedef struct _VIRTMEM
{
    HWND   hSelf;
    HWND   hListBox;
    LPTSTR szPagingFiles;
    TCHAR  szDrive[10];
    INT    Count;
    BOOL   bModified;
    PAGEFILE  Pagefile[26];
} VIRTMEM, *PVIRTMEM;

typedef struct _BOOTRECORD
{
  DWORD BootType;
  WCHAR szSectionName[128];
  WCHAR szBootPath[MAX_PATH];
  WCHAR szOptions[512];

}BOOTRECORD, *PBOOTRECORD;

INT
ResourceMessageBox(
    IN HINSTANCE hInstance,
    IN HWND hwnd,
    IN UINT uType,
    IN UINT uCaption,
    IN UINT uText);


#endif /* __CPL_SYSDM_H */
