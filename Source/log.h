#include <Windows.h>

void Log(const char* title, const char* text) noexcept
{
	MessageBoxA(0, text, title, MB_OK | MB_ICONINFORMATION);
}
void LogError(const char* title, const char* text) noexcept
{
	MessageBoxA(0, text, title, MB_OK | MB_ICONERROR);
}