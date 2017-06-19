// dllmain.cpp : Defines the entry point for the DLL application.
//=********************** DllMain ***********************
//=******************************************************

// ----- include files -------------------------
#include	<malloc.h>
#include	<stdio.h>
#include	<string.h>

#define	DEV_API_EXPORTS
#define	DEVS_API_EXPORTS

#include "utypes.h"
#include "brdapi.h"


#ifdef _WIN32
BOOL __stdcall DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}


//=********************** DllEntryPoint *****************
//=******************************************************
BOOL __stdcall DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	// Included for compatibility with Borland 
	return DllMain(hinstDLL, fdwReason, lpvReserved);
}
#endif
