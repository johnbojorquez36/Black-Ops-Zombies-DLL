#include <iostream>
#include <string>
#include <Windows.h>
#include "constants.h"

using namespace std;

BOOL ammoOn = false;
BOOL godOn = false;
BOOL noclipOn = false;
BOOL lottoPointsOn = false;


void playOn() {
	Beep(523, 200);
	Beep(659, 200);
}

void playOff() {
	Beep(659, 200);
	Beep(523, 200);
}

// Writes char array to the given address in memory
void writeStringToMemory(DWORD addressToWrite, char* valueToWrite, int numBytes) {
	unsigned long oldProtection;

	VirtualProtect((LPVOID)addressToWrite, numBytes, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy((LPVOID)addressToWrite, valueToWrite, numBytes);
	VirtualProtect((LPVOID)addressToWrite, numBytes, oldProtection, NULL);
}

void writeIntToMemory(DWORD addressToWrite, int valueToWrite) {
	unsigned long oldProtection;

	VirtualProtect((LPVOID)addressToWrite, 4, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(int*)addressToWrite = valueToWrite;
	VirtualProtect((LPVOID)addressToWrite, 4, oldProtection, NULL);
}

DWORD getBaseAddress() {
	return (DWORD)GetModuleHandleA("BlackOps.exe");
}

void enableInfAmmo() {
	writeStringToMemory(getBaseAddress() + AMMO_DEC_ADDR_OFF, INF_AMMO_OP_CODE, 3);
	playOn();
}

void disableInfAmmo() {
	writeStringToMemory(getBaseAddress() + AMMO_DEC_ADDR_OFF, ORIG_AMMO_OP_CODE, 3);
	playOff();
}

void enableGodMode() {
	writeStringToMemory(getBaseAddress() + HEALTH_DEC_ADDR_OFF, INF_HEALTH_OP_CODE, 6);
	playOn();
}

void disableGodMode() {
	writeStringToMemory(getBaseAddress() + HEALTH_DEC_ADDR_OFF, ORIG_HEALTH_OP_CODE, 6);
	playOff();
}

void giveLottoPoints() {
	writeIntToMemory(getBaseAddress() + POINTS_ADDR_OFF, POINTS_LOTTO);
	playOn();
}

void removeLottoPoints() {
	writeIntToMemory(getBaseAddress() + POINTS_ADDR_OFF, 500);
	playOff();
}

void enableNoclip() {
	writeIntToMemory(getBaseAddress() + NOCLIP_ADDR_OFF, NOCLIP_ON);
	playOn();
}

void disableNoclip() {
	writeIntToMemory(getBaseAddress() + NOCLIP_ADDR_OFF, NOCLIP_OFF);
	playOff();
}

void loop() {

	while (1) {
		if (GetAsyncKeyState(VK_F1) & 0x8000) {
			if (!ammoOn) {
				ammoOn = true;
				enableInfAmmo();
			} else {
				ammoOn = false;
				disableInfAmmo();
			}
		}
		else if (GetAsyncKeyState(VK_F2) & 0x8000) {
			if (!godOn) {
				godOn = true;
				enableGodMode();
			} else {
				godOn = false;
				disableGodMode();
			}
		} else if (GetAsyncKeyState(VK_F3) & 0x8000) {
			if (!lottoPointsOn) {
				lottoPointsOn = true;
				giveLottoPoints();
			} else {
				lottoPointsOn = false;
				removeLottoPoints();
			}
		} else if (GetAsyncKeyState(VK_F4) & 0x8000) {
			if (!noclipOn) {
				noclipOn = true;
				enableNoclip();
			} else {
				noclipOn = false;
				disableNoclip();
			}
		}

		Sleep(100);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		MessageBoxA(NULL, "Zombies Mod by chr0m3playa.", "", MB_OK);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&loop, 0, 0, 0);
	}
	return TRUE;
}

