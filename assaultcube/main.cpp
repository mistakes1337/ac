// Noni alotetaas tekemällä assault cubeen muistista lukeminen
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <thread>

int main() {
	// Haetaan ekaks suoraan health addressi muistista
	uintptr_t healthaddress = 0x00823F74;
	// Tehään temp muuttuja johon tallennetaan healthin arvo!
	int health; // int sen takia koska pelin muistissa health on 4 bittinen arvo ja int on myös!

	// Alotetaan prosessin hakeminen ja snapshotin teko
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	// Copilot osaa tuottaa hyvin valmiiksi mitkä olisi tullut muutenkin joten hyödynnetään sitä
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create snapshot" << std::endl;
		return 1;
	}

	if (Process32First(snapshot, &entry)) {
		do {
			if (_stricmp(entry.szExeFile, "ac_client.exe") == 0) {
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID); // Avataan prosessi kaikilla oikeuksilla
				if (hProcess) {
					while (true) {
						if (GetAsyncKeyState(VK_END) & 1) {
							break; // Kun painetaan end nappia näppäimistöstä lopetetaan looppi
						}
						// Luetaan muistista healthin arvo ja tulostetaan se konsoliin
						// Nää kommentit tulee tosi helpolla joten käytetään niitä samal ni aikaa säästyy :)
						if (ReadProcessMemory(hProcess, (LPVOID)healthaddress, &health, sizeof(health), 0)) {
							std::cout << "Health: " << health << std::endl; 
						}
						else {
							std::cout << "Failed to read memory" << std::endl;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100)); // hitusen hidastetaan konsolin vauhtia
					}
					CloseHandle(hProcess); // Suljetaan prosessi
				}
				else {
					std::cout << "Failed to open process" << std::endl;

				}
				break;
			}
		} while (Process32Next(snapshot, &entry));
	}
	CloseHandle(snapshot); // Suljetaan snapshot
	return 0;
}