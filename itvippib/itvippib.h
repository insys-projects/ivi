//=********************************************************
//
// ITVIPPIB.H
//
// VISA PXI Plug-In API Definition 
//
// (C) Instr.Sys. by Do Jan 2017
//
// Modifications:
//   
//   
//
//=********************************************************

#ifndef	__ITVIPPIB_H_
#define	__ITVIPPIB_H_

#include	"visa.h"
#include	"isdll.h"
#include	"brd.h"

typedef struct DEVDATA
{
	PCI_DEVICE	pciDev;
	PCI_HEADER	pciHdr;
	BRD_Info	Info;
	U32			lid;
	ViAddr		userSpaceMem[6];
} DEVDATA;

/*************************************************
*
* Entry Point types
*
*/
#if !defined(WIN32) && !defined(__WIN32__)
#define FENTRY
#define STDCALL
#else
#include <windows.h>
#define DllExport	__declspec( dllexport )
#define FENTRY		DllExport
   #ifdef _WIN64
   #define STDCALL		
   #else
   #define STDCALL	 __stdcall
   #endif
#define	huge
#endif  // WIN32


#ifdef	__cplusplus
extern "C" {
#endif

typedef ViBusSize PpiLength;
typedef ViAddr PpiHandle;
typedef enum
{
	Bar0 = 0,
	Bar1,
	Bar2,
	Bar3,
	Bar4,
	Bar5,
	Config
} PpiSpace;

DllExport ViStatus	STDCALL PpiInitializePlugin(void);
DllExport ViStatus	STDCALL PpiGetDeviceIDs(
						ViBoolean includeNonPrimary,
						ViInt32 arrayElementCount,
						ViAUInt64 deviceIdArray,
						ViABoolean isPrimaryArray,
						ViPInt32 deviceCount
					);
DllExport ViStatus	STDCALL PpiOpen(
						ViInt32 intfc,
						ViInt32 bus,
						ViInt32 device,
						ViInt32 function,
						PpiHandle* handle
					);
DllExport ViStatus	STDCALL PpiGetSpaceInfo(
						PpiHandle handle,
						PpiSpace space,
						ViPInt16 spaceType,
						ViPUInt64 spaceBase,
						ViPUInt64 spaceSize
					);
DllExport	ViStatus STDCALL 	PpiGetDeviceAttribute(
						PpiHandle handle,
						ViAttr attributeID,
						void * attributeValue
					);
DllExport	ViStatus	STDCALL PpiMapMemory(
						PpiHandle handle,
						PpiSpace space,
						ViUInt64 offset,
						PpiLength length,
						void ** userSpaceMem
					);
DllExport	ViStatus	STDCALL PpiUnmapMemory(
						PpiHandle handle,
						ViAddr userSpaceMem
					);
DllExport	ViStatus	STDCALL PpiBlockWrite(
						PpiHandle handle,
						ViInt32 flags,
						PpiSpace space,
						ViUInt64 offset,
						ViUInt32 width,
						ViBoolean increment,
						void * writeBuffer,
						PpiLength count,
						ViUInt32 timeoutMilliseconds
					);
DllExport	ViStatus	STDCALL PpiBlockRead(
						PpiHandle handle,
						ViInt32 flags,
						PpiSpace space,
						ViUInt64 offset,
						ViUInt32 width,
						ViBoolean increment,
						void * readBuffer,
						PpiLength count,
						ViUInt32 timeoutMilliseconds
					);
DllExport	ViStatus	STDCALL PpiEnableInterrupts(
						PpiHandle handle,
						ViUInt16 queueLength
					);
DllExport	ViStatus	STDCALL PpiWaitInterrupt(
						PpiHandle handle,
						ViUInt32 timeoutMilliseconds,
						ViPInt16 interruptSequence,
						ViPUInt32 interruptData
					);
DllExport	ViStatus	STDCALL PpiDisableAndAbortWaitInterrupt(
						PpiHandle handle
					);
DllExport	ViStatus	STDCALL PpiTerminateIO(
						PpiHandle handle,
						void* buffer
					);
DllExport	ViStatus	STDCALL PpiClose(
						PpiHandle handle
					);
DllExport	ViStatus	STDCALL PpiFinalizePlugin(void);


//
//
//

#ifdef	__cplusplus
};  //extern "C"
#endif


#endif	// __ITVIPPIB_H_

	//
	// End of File
	//
