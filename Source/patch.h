#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <WinUser.h>
#include <TlHelp32.h>
#include <vector>
void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);