#include <windows.h>
#include "MinHook.h"

// ���������� ��������� � libMinHook.dll
#pragma comment(lib, "libMinHook.x64.lib")

// ��������� �� ������������ GetLocalTime
static void (WINAPI *GetLocalTime_)(LPSYSTEMTIME lpSystemTime);

void WINAPI MyGetLocalTime(LPSYSTEMTIME lpSystemTime)
{
  // �������� ������������ GetLocalTime
  GetLocalTime_(lpSystemTime);

  // ����������� ���������� ������: 59 -> 0, 0 -> 59
  lpSystemTime->wSecond = 59 - lpSystemTime->wSecond;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason) {
  case DLL_PROCESS_ATTACH:
    // �� ���������� ����� � ����������!
    // �� ��������� ���������� ��������, ������ DllMain() ����� ��������� ������ ����� ����������� ����������

    // �������������� ���������� libMinHook
    if (MH_Initialize() != MH_OK) {
      return FALSE;
    }

    // ������ ��� (���� ��� �� ����� � ����������� ���������)
    if (MH_CreateHook(&GetLocalTime, &MyGetLocalTime, reinterpret_cast<void**>(&GetLocalTime_)) != MH_OK) {
      return FALSE;
    }

    // ������������ ���
    if (MH_EnableHook(&GetLocalTime) != MH_OK) {
      return FALSE;
    }
  }

  return TRUE;
}
