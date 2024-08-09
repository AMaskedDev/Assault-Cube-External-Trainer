#include "resources.h"
#include "log.h"
#include "patch.h"
#include "settings.h"
#include <iostream>

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
		<< "[NUM1] Ammo:\t\t\t--> " << OnOff(settings::infiniteAmmo) << " <--" << "\n"
		<< "[NUM2] Health:\t\t\t--> " << OnOff(settings::infiniteHealth) << " <--" << "\n"
		<< "[NUM3] Speed (-> Control + W):\t--> " << OnOff(settings::walkHack) << " <--" << "\n"
		<< "[NUM4] Recoil:\t\t\t--> " << OnOff(settings::noRecoil) << " <--" << "\n"
		<< "[NUM5] InstaFire:\t\t--> " << OnOff(settings::instaFire) << " <--" << "\n"
		<< std::endl
		<< "[END] Uninject / Exit\n"
		<< "-------------------------------------------\n";
}

int main()
{
	HWND processWindow = FindWindowA(0, "AssaultCube");
	if (!processWindow)
	{
		LogError("Finding Window", "Error while finding assault cube.");
		return 1;
	}

	DWORD processID;
	GetWindowThreadProcessId(processWindow, &processID);
	if (processID == 0)
	{
		LogError("Getting proccess ID", "Error while finding assault cube's process ID.");
		return 1;
	}

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processID);
	if (!processHandle || processHandle == INVALID_HANDLE_VALUE)
	{
		LogError("Getting handle to process", "Error while getting a handle to assault cube.");
		return 1;
	}

	DWORD baseAddress = GetModuleBaseAddress(_T((char[])"ac_client.exe"), processID);

	// Getting all pointers
	DWORD ar_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::arAmmoOffset });
	DWORD smg_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::smgAmmoOffset });
	DWORD sniper_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::sniperAmmoOffset });
	DWORD shotgun_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::shotgunAmmoOffset });
	DWORD akimbo_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::akimboAmmoOffset });
	DWORD pistol_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::pistolAmmoOffset });
	DWORD fastPistol_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::fastPistolAmmoOffset });
	DWORD grenades_ammoPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::grenadesOffset });

	DWORD healthPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::healthOffset });
	DWORD armorPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::armorOffset });
	DWORD speedPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::speedOffset });

	DWORD ar_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::arTimerOffset });
	DWORD smg_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::machinegunTimerOffset });
	DWORD sniper_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::sniperTimerOffset });
	DWORD shotgun_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::shotgunTimerOffset });
	DWORD carbine_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::carbineTimerOffset });
	DWORD pistol_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::pistolTimerOffset });
	DWORD knife_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::knifeTimerOffset });
	DWORD grenades_timerPtrAddr = GetPointerAddress(processWindow, baseAddress, offsets::localPlayer, { offsets::grenadeTimerOffset });

	PrintMenu();

	// WPM
	int toWrite = 1337;
	int timer = 0;
	int walkHack = 5;

	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_NUMPAD1))
		{
			settings::infiniteAmmo = !settings::infiniteAmmo;

			PrintMenu();
			Sleep(70);
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

		if (settings::noRecoil)
		{
			NopEx((BYTE*)(baseAddress + 0x63786), 10, processHandle);
		}
		else
		{
			PatchEx((BYTE*)(baseAddress + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, processHandle);
		}

		if (settings::infiniteAmmo)
		{
			WriteProcessMemory(processHandle, (LPVOID*)ar_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)smg_timerPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)sniper_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)shotgun_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)akimbo_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)pistol_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)fastPistol_ammoPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)grenades_ammoPtrAddr, &toWrite, sizeof(int), 0);
		}
		if (settings::infiniteHealth)
		{
			WriteProcessMemory(processHandle, (LPVOID*)healthPtrAddr, &toWrite, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)armorPtrAddr, &toWrite, sizeof(int), 0);
		}
		if (settings::instaFire)
		{
			WriteProcessMemory(processHandle, (LPVOID*)ar_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)smg_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)sniper_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)shotgun_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)carbine_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)pistol_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)knife_timerPtrAddr, &timer, sizeof(int), 0);
			WriteProcessMemory(processHandle, (LPVOID*)grenades_timerPtrAddr, &timer, sizeof(int), 0);
		}

		if (settings::walkHack)
		{
			if (GetAsyncKeyState('W') & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				WriteProcessMemory(processHandle, (LPVOID*)speedPtrAddr, &walkHack, sizeof(int), 0);
			}
		}
	}

	CloseHandle(processHandle);
	system("cls");
	std::cout << "Uninjected Successfully." << std::endl;
	system("pause");
}