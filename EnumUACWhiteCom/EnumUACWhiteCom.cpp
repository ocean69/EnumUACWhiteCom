// EnumUACWhiteCom.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

//HKEY_CLASSES_ROOT\CLSID\{3ad05575 - 8857 - 4850 - 9277 - 11b85bdb8e09}\Elevation

int main()
{
	HKEY clsid;
	LRESULT r = RegOpenKey(HKEY_CLASSES_ROOT, L"CLSID\\", &clsid);
	if (r != ERROR_SUCCESS)
		printf("Open HCR\CLSID failed.\n");
	
	DWORD enumIndex = 0;
	WCHAR keyName[1024];
	WCHAR elevationName[1024];
	WCHAR path[MAX_PATH];
	do {
		r = RegEnumKey(clsid, enumIndex, keyName, 1024);
		HKEY elevationKey;
		if (r == ERROR_SUCCESS) {
			lstrcpy(elevationName, keyName);
			lstrcat(elevationName, L"\\Elevation");
			LRESULT r1 = RegOpenKey(clsid, elevationName, &elevationKey);
			if (r1 == ERROR_SUCCESS) {
				RegCloseKey(elevationKey);
				HKEY comKey;
				r1 = RegOpenKey(clsid, keyName, &comKey);
				if (r1 == ERROR_SUCCESS) {
					LONG cb = MAX_PATH * sizeof(WCHAR);
					r1 = RegQueryValue(comKey, L"InprocServer32\\", path, &cb);
					if (r1 != ERROR_SUCCESS) {
						r1 = RegQueryValue(comKey, L"LocalServer32\\", path, &cb);
					}

					if (r1 == ERROR_SUCCESS) {
						wprintf(L"White Com Path:%s\n", path);
						RegCloseKey(comKey);
					}
					
				}
				wprintf(L"White Com GUID:%s\n\n", keyName);
			}
		}
		enumIndex++;
	} while (r == ERROR_SUCCESS);
	RegCloseKey(clsid);
	if (r != ERROR_NO_MORE_ITEMS)
		printf("Enum failed.\n");
	getchar();
    return 0;
}

