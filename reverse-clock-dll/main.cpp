#include <windows.h>
#include "MinHook.h"

// Статически линкуемся с libMinHook.dll
#pragma comment(lib, "libMinHook.x64.lib")

// Указатель на оригинальный GetLocalTime
static void (WINAPI *GetLocalTime_)(LPSYSTEMTIME lpSystemTime);

void WINAPI MyGetLocalTime(LPSYSTEMTIME lpSystemTime)
{
  // Вызываем оригинальный GetLocalTime
  GetLocalTime_(lpSystemTime);

  // Инвертируем количество секунд: 59 -> 0, 0 -> 59
  lpSystemTime->wSecond = 59 - lpSystemTime->wSecond;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason) {
  case DLL_PROCESS_ATTACH:
    // Не повторяйте этого в продакшене!
    // Во избежание неприятных дедлоков, внутри DllMain() нужно оставлять только самый необходимый функционал

    // Инициализируем библиотеку libMinHook
    if (MH_Initialize() != MH_OK) {
      return FALSE;
    }

    // Создаём хук (пока что он будет в выключенном состоянии)
    if (MH_CreateHook(&GetLocalTime, &MyGetLocalTime, reinterpret_cast<void**>(&GetLocalTime_)) != MH_OK) {
      return FALSE;
    }

    // Активизируем хук
    if (MH_EnableHook(&GetLocalTime) != MH_OK) {
      return FALSE;
    }
  }

  return TRUE;
}
