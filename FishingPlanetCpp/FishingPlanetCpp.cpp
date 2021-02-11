// FishingPlanetCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <WinUser.h>
#include <tchar.h>
#include <string>


DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void simulateReelFast(HWND targetWnd, int hold)
{
	HWND foreWnd = GetForegroundWindow();
	DWORD windowThreadProcessId = GetCurrentThreadId();
	DWORD wakfuThreadID = GetWindowThreadProcessId(targetWnd, 0);
	BringWindowToTop(targetWnd);
	SetFocus(FindWindowA(NULL, "Fishing Planet"));
	AttachThreadInput(windowThreadProcessId, wakfuThreadID, true); // switch input to targetWnd and set focust on to it



	HWND hwndWindowNotepad = FindWindow(NULL, L"Fishing Planet");
	SetFocus(hwndWindowNotepad);

	if (hwndWindowNotepad)
	{
		// Find the target Edit window within Notepad.
		if (hwndWindowNotepad)
		{
		

			PostMessage(hwndWindowNotepad, WM_KEYDOWN, VK_LSHIFT, VK_LSHIFT);

			Sleep(10);

			PostMessage(hwndWindowNotepad, WM_KEYDOWN, VK_SPACE, VK_SPACE);
	
			Sleep(hold);
	
			PostMessage(hwndWindowNotepad, WM_KEYUP, VK_SPACE, VK_SPACE);

			Sleep(10);

			PostMessage(hwndWindowNotepad, WM_KEYUP, VK_LSHIFT, VK_LSHIFT);
			keybd_event(0xa0, 0, KEYEVENTF_KEYUP, 0);

			Sleep(15);

		
		//	SetFocus(foreWnd);

		}
	}

	//AttachThreadInput(windowThreadProcessId, wakfuThreadID, false);

}

void simulateConfirmFish(HWND targetWnd, int hold)
{
	HWND foreWnd = GetForegroundWindow();
	DWORD windowThreadProcessId = GetCurrentThreadId();
	DWORD wakfuThreadID = GetWindowThreadProcessId(targetWnd, 0);
	SetFocus(targetWnd);
	BringWindowToTop(targetWnd);
	AttachThreadInput(windowThreadProcessId, wakfuThreadID, true); // switch input to targetWnd and set focust on to it

	SetFocus(targetWnd);
	BringWindowToTop(targetWnd);

	HWND hwndWindowNotepad = FindWindow(NULL, L"Fishing Planet");
	if (hwndWindowNotepad)
	{
		// Find the target Edit window within Notepad.
		if (hwndWindowNotepad)
		{
			
			keybd_event(0x20, 0, 0, 0);
			keybd_event(0x20, 0, KEYEVENTF_KEYUP, 0);

			keybd_event(0x08, 0, 0, 0);
			keybd_event(0x08, 0, KEYEVENTF_KEYUP, 0);
	
			//SetFocus(foreWnd);

		}
	}

	//AttachThreadInput(windowThreadProcessId, wakfuThreadID, false);

}



void simulateCastRod(HWND targetWnd,  int hold)
{
	HWND foreWnd = GetForegroundWindow();
	DWORD windowThreadProcessId = GetCurrentThreadId();
	DWORD wakfuThreadID = GetWindowThreadProcessId(targetWnd, 0);
	SetFocus(targetWnd);
	BringWindowToTop(targetWnd);
	SetFocus(FindWindowA(NULL, "Fishing Planet"));
	AttachThreadInput(windowThreadProcessId, wakfuThreadID, true); // switch input to targetWnd and set focust on to it


		
	Sleep(200);
		

		keybd_event(0x20, 0, 0, 0);


		Sleep(hold);

		keybd_event(0x20, 0, KEYEVENTF_KEYUP, 0);
		//SetFocus(foreWnd);



		Sleep(10);

	

	//AttachThreadInput(windowThreadProcessId, wakfuThreadID, false);

}


void simulateMouseClickOnWnd(HWND targetWnd, int clickX, int clickY, int clickType, int hold)
{
	HWND foreWnd = GetForegroundWindow();
	DWORD windowThreadProcessId = GetCurrentThreadId();
	DWORD wakfuThreadID = GetWindowThreadProcessId(targetWnd, 0);

	AttachThreadInput(windowThreadProcessId, wakfuThreadID, true); // switch input to targetWnd and set focust on to it


	DWORD mousePos; // Im setting desired click position and send it throught lParam
	*(WORD*)&mousePos = clickX;
	*((WORD*)&mousePos + 1) = clickY;


	if (clickType == 0) //        LEFT CLICK
	{
		SetFocus(targetWnd);
		SendMessage(targetWnd, WM_LBUTTONDOWN, MK_LBUTTON, mousePos);

		Sleep(hold);

		SendMessage(targetWnd, WM_LBUTTONUP, MK_LBUTTON, mousePos);
	
		//SetFocus(foreWnd);
	}
	else if(clickType == 1) //                        RIGHT CLICK
	{
		SetFocus(targetWnd);
		SendMessage(targetWnd, WM_RBUTTONDOWN, MK_RBUTTON, mousePos);

		Sleep(hold);

		SendMessage(targetWnd, WM_RBUTTONUP, MK_LBUTTON, mousePos);
		//SetFocus(foreWnd);
	}

	//AttachThreadInput(windowThreadProcessId, wakfuThreadID, false);

	
}

int rodCasted = 0;
int fishCatched = 0;
int fishHookedFloat = 0;
int dist = 0;

void checkExit() {
	if (GetAsyncKeyState(VK_F1)) {
		exit(0);
		abort();
	}
}






int main()
{
	//Get ProcId of the target process
	DWORD procId = GetProcId(L"FishingPlanet.exe");


	while (!procId) {
		system("cls");
		std::cout << "Searching for the process...";
		procId = GetProcId(L"FishingPlanet.exe");
		Sleep(1800);
	}
	                
	//Getmodulebaseaddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"GameAssembly.dll");

	//Get Handle to Process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);



	Sleep(1000);

	// ROD CASTED OFFSET

	//Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x029B1100;

	//Resolve the pointer chain
	std::vector<unsigned int> offsets = { 0xb8, 0xa8, 0x180, 0xb8, 0x10, 0xf8, 0x48 };

	uintptr_t addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, offsets);
	// END ROD CASTED



	// FISH CATCHED OFFSET

	//Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddrFishCatched = moduleBase + 0x02997BA0;

	//Resolve the pointer chain
	std::vector<unsigned int> fishCatchedOffsets = { 0x78, 0xb8, 0x8, 0x80, 0x110, 0xa8, 0xe48 };

	uintptr_t addrFishCatched = FindDMAAddy(hProcess, dynamicPtrBaseAddrFishCatched, fishCatchedOffsets);
	// END FISH CATCHED


	// DISTANCE OFFSET

	//Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddrDist = moduleBase + 0x02997070;

	//Resolve the pointer chain
	std::vector<unsigned int> distOffsets = { 0x80, 0x30, 0xb8, 0x10, 0xc0, 0x60, 0x34 };

	uintptr_t addrDist = FindDMAAddy(hProcess, dynamicPtrBaseAddrDist, distOffsets);
	// END DISTANCE


	// FloatHooked OFFSET

	//Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddrFloatHooked = moduleBase + 0x02998F58;

	//Resolve the pointer chain
	std::vector<unsigned int> FloatHookedOffsets = { 0xb8, 0x8, 0x328, 0x20, 0x68, 0x50, 0x28 };

	uintptr_t addrFloatHooked = FindDMAAddy(hProcess, dynamicPtrBaseAddrFloatHooked, FloatHookedOffsets);
	// END FloatHooked






	//std::cout << "addr = " << "0x" << std::hex << addr;


	int floatRod = 0;

	std::cout << "0-Spin | 1-Float: ";
	std::cin >> floatRod;


	

	while (floatRod = 0) {
		system("cls");
		checkExit();
		ReadProcessMemory(hProcess, (BYTE*)addr, &rodCasted, sizeof(rodCasted), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)addrDist, &dist, sizeof(dist), nullptr);
		BringWindowToTop(FindWindowA(NULL, "Fishing Planet"));
		SetFocus(FindWindowA(NULL, "Fishing Planet"));
		if (fishCatched == 1) {
		
			system("cls");
			std::cout << "A fish has been catched!";
			Sleep(200);
			while (fishCatched == 1) {
				checkExit();
				ReadProcessMemory(hProcess, (BYTE*)addr, &rodCasted, sizeof(rodCasted), nullptr);
				ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
				ReadProcessMemory(hProcess, (BYTE*)addrDist, &dist, sizeof(dist), nullptr);
				simulateConfirmFish(FindWindowA(NULL, "Fishing Planet"), 50);
				Sleep(300);
			}

			Sleep(1000);
			
		}
		else {
			if (rodCasted == 1) {
				if (dist <= 14) {
					while (rodCasted == 1) {
						checkExit();
						ReadProcessMemory(hProcess, (BYTE*)addr, &rodCasted, sizeof(rodCasted), nullptr);
						ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
						ReadProcessMemory(hProcess, (BYTE*)addrDist, &dist, sizeof(dist), nullptr);
						simulateReelFast(FindWindowA(NULL, "Fishing Planet"), 1000);
					}
				}
				system("cls");      
				std::cout << "The rod is Casted. Reeling." << std::endl;
				simulateMouseClickOnWnd(FindWindowA(NULL, "Fishing Planet"), 1, 1, 0, 560);
				if (dist > 10) {
					simulateMouseClickOnWnd(FindWindowA(NULL, "Fishing Planet"), 1, 1, 1, 390);
				}
			
			}
			else {
				std::cout << "The rod is NOT Casted. Waiting 1.2 sec and casting again.." << std::endl;
				SetFocus(FindWindowA(NULL, "Fishing Planet"));
				
				ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
				if (fishCatched == 1) {
					
				}
				else {

					Sleep(1200);
					simulateCastRod(FindWindowA(NULL, "Fishing Planet"), 1900);
					system("cls");
					std::cout << "The rod has been casted. Waiting for the bait to fall off..";

					for (int i = 0; i < 7; i++) {
						ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
						if (fishCatched == 1) {

						}
						else {
							Sleep(1000);
						}
						ReadProcessMemory(hProcess, (BYTE*)addr, &rodCasted, sizeof(rodCasted), nullptr);
						if (rodCasted == 0) {
							
						}
						else {
							Sleep(1000);
						}


					}
				}
				

				

			}

		}
		
	

	}


	while (floatRod = 1) {
		system("cls");
		checkExit();
		ReadProcessMemory(hProcess, (BYTE*)addr, &rodCasted, sizeof(rodCasted), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)addrFishCatched, &fishCatched, sizeof(fishCatched), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)addrDist, &dist, sizeof(dist), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)addrFloatHooked, &fishHookedFloat, sizeof(fishHookedFloat), nullptr);


		std::cout << "fish hooked: " << std::dec << fishHookedFloat << std::endl;
		std::cout << "rod casted: " << std::dec << rodCasted << std::endl;
		std::cout << "distance: " << std::dec << dist << std::endl;
		std::cout << "fishCatched: " << std::dec << fishCatched << std::endl;

		if (fishCatched == 1) {
			Sleep(50);
			simulateConfirmFish(FindWindowA(NULL, "Fishing Planet"), 200);
		}

		if (rodCasted == 0 && fishCatched == 0) {
			simulateCastRod(FindWindowA(NULL, "Fishing Planet"), 1000);
			Sleep(500);
		}

		if (fishHookedFloat == 1 && rodCasted == 1 && fishCatched == 0) {
			Sleep(8000);
			simulateReelFast(FindWindowA(NULL, "Fishing Planet"), 1000);
		}



		BringWindowToTop(FindWindowA(NULL, "Fishing Planet"));
		SetFocus(FindWindowA(NULL, "Fishing Planet"));
		


	}
	

	

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
