#include<iostream>
#include<Windows.h>
#include<vector>
#include<TlHelp32.h>
#include "proc.h"


using namespace std;



int main()
{
	//These are the standart values for FOV and Zoom
	float neuZoomValue{ 2250 };
	float neuFovValue{ 40 };
	DWORD procId{GetProcId(L"League Of Legends.exe")};
	if (procId == NULL)
	{
		cout << "League need to be started" << '\n';
		Sleep(4000);
		exit(-1);
	}
	else
	{
		uintptr_t modulBase{GetModuleBaseAddress(procId, L"League of Legends.exe")};
		HANDLE hProcess{};
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		uintptr_t dynamicPtrBaseAddrZoom = modulBase + 0x01AFA418;
		uintptr_t dynamicPtrBaseAddrZoom2 = modulBase + 0x0339496C;
		//vector<unsigned int> ZoomOffsets = { 12 , 608 }; //not needen anymore was bugged and did zoom out without controlling it ingame
		vector<unsigned int> ZoomOffsets2 = { 40 };
		vector<unsigned int> FovOffsets = { 12 , 376 };
		uintptr_t FovAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddrZoom, FovOffsets);
		//uintptr_t ZoomAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddrZoom, ZoomOffsets); //outdated method
		uintptr_t ZoomAddr2 = FindDMAAddy(hProcess, dynamicPtrBaseAddrZoom2, ZoomOffsets2);
		if (procId == NULL)
		{
			cout << "seems like something went wrong" << '\n';
			Sleep(4000);
			exit(-1);
		}
		else
		{
			for (;;)
			{
				if (GetAsyncKeyState(VK_ADD))
				{
					//WriteProcessMemory(hProcess, (LPVOID)ZoomAddr, &neuZoomValue, sizeof(neuZoomValue), 0); //outdated
					WriteProcessMemory(hProcess, (LPVOID)ZoomAddr2, &neuZoomValue, sizeof(neuZoomValue), 0);
					neuZoomValue = neuZoomValue + 50;
					cout << neuZoomValue +50 << '\n';
				}
				else if (GetAsyncKeyState(VK_SUBTRACT))
				{
					//WriteProcessMemory(hProcess, (LPVOID)ZoomAddr, &neuZoomValue, sizeof(neuZoomValue), 0); //outdated
					WriteProcessMemory(hProcess, (LPVOID)ZoomAddr2, &neuZoomValue, sizeof(neuZoomValue), 0);
					neuZoomValue = neuZoomValue - 50;
					cout << neuZoomValue - 50 << '\n';
				}
				else if (GetAsyncKeyState(VK_UP))
				{
					WriteProcessMemory(hProcess, (LPVOID)FovAddr, &neuFovValue, sizeof(neuFovValue), 0);
					neuFovValue = neuFovValue + 1;
					cout << neuFovValue + 1 << '\n';
				}
				else if (GetAsyncKeyState(VK_DOWN))
				{
					WriteProcessMemory(hProcess, (LPVOID)FovAddr, &neuFovValue, sizeof(neuFovValue), 0);
					neuFovValue = neuFovValue - 1;
					cout << neuFovValue - 1 << '\n';
				}
				Sleep(100);
			}while (true);
		}
		return 0;
	}
	return 0;
}
