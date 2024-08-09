#pragma once
#include <tchar.h>
#include <stdlib.h>
#include "settings.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <iostream>

namespace offsets {
	const DWORD localPlayer = 0x10F4F4;
	const DWORD entityList = 0x10F4F8;

	const DWORD healthOffset = 0xF8;
	const DWORD armorOffset = 0xFC;
	const DWORD speedOffset = 0x80;
	const DWORD jumpPowerOffset = 0x18;

	const DWORD arAmmoOffset = 0x150;
	const DWORD smgAmmoOffset = 0x148;
	const DWORD shotgunAmmoOffset = 0x144;
	const DWORD marksmanAmmoOffset = 0x140;
	const DWORD akimboAmmoOffset = 0x15C;
	const DWORD sniperAmmoOffset = 0x14C;
	const DWORD pistolAmmoOffset = 0x13C;
	const DWORD fastPistolAmmoOffset = 0x15C;
	const DWORD grenadesOffset = 0x158;

	const DWORD sniperTimerOffset = 0x174;
	const DWORD knifeTimerOffset = 0x160;
	const DWORD carbineTimerOffset = 0x168;
	const DWORD shotgunTimerOffset = 0x16C;
	const DWORD machinegunTimerOffset = 0x170;
	const DWORD pistolTimerOffset = 0x164;
	const DWORD arTimerOffset = 0x178;
	const DWORD grenadeTimerOffset = 0x180;

	const DWORD canShotOffset = 0x70;
}

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, std::vector<DWORD> offsets)
{
	DWORD pID = NULL; // Game process ID
	GetWindowThreadProcessId(hwnd, &pID);
	HANDLE phandle = NULL;
	phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

	DWORD offset_null = NULL;
	ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
	DWORD pointeraddress = offset_null; // the address we need
	for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
	{
		ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
	}
	return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

void CloseRoutine()
{
	system("cls");
	std::cout << "Uninjected Successfully" << std::endl;
	Sleep(1000);
	system("cls");
	std::cout << "Closing in 3.0" << std::endl;
	Sleep(1000);
	system("cls");
	std::cout << "Closing in 2.0" << std::endl;
	Sleep(1000);
	system("cls");
	std::cout << "Closing in 1.0" << std::endl;
	Sleep(1000);
	system("cls");
	std::cout << "Closing in 0.5" << std::endl;
	Sleep(700);
}

std::string OnOff(bool x)
{
	if (x)
		return "ON ";
	else
		return "OFF";
}

void PrintMenu()
{
	system("cls");

	std::cout
		<< "-------------------------------------------\n"
		<< "              External Trainer              \n"
		<< "-------------------------------------------\n"
		<< "[NUM1] Ammo:\t\t\t--> " << OnOff(settings::infiniteAmmo) << "\n"
		<< "[NUM2] Health:\t\t\t--> " << OnOff(settings::infiniteHealth) << "\n"
		<< "[NUM3] Speed (-> Control + W):\t--> " << OnOff(settings::walkHack) << "\n"
		<< "[NUM4] Recoil:\t\t\t--> " << OnOff(settings::noRecoil) << "\n"
		<< "[NUM5] InstaFire:\t\t--> " << OnOff(settings::instaFire) << "\n";

	std::cout
		<< std::endl
		<< "[END] Uninject / Exit\n"
		<< "-------------------------------------------\n";

}

void SettingsUpdate()
{
	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		settings::infiniteAmmo = !settings::infiniteAmmo;

		PrintMenu();
		Sleep(120);
	}
	if (GetAsyncKeyState(VK_NUMPAD2))
	{
		settings::infiniteHealth = !settings::infiniteHealth;

		PrintMenu();
		Sleep(120);
	}
	if (GetAsyncKeyState(VK_NUMPAD3))
	{
		settings::walkHack = !settings::walkHack;

		PrintMenu();
		Sleep(120);
	}
	if (GetAsyncKeyState(VK_NUMPAD4))
	{
		settings::noRecoil = !settings::noRecoil;

		PrintMenu();
		Sleep(120);
	}
	if (GetAsyncKeyState(VK_NUMPAD5))
	{
		settings::instaFire = !settings::instaFire;

		PrintMenu();
		Sleep(120);
	}
}