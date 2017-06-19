/***************************************************
*
* ITVIPPIB.CPP
*
*
* (C) Instr.Sys. by Do Jan 2017
*
****************************************************/
#include <stdio.h>

#include "itvippib.h"
#include "isdll.h"
#include "brd.h"

#define	MAXDEV		0x100
#define MANF_ID		0x4953
#define MANF_NAME	"Instrumental Systems"
#define MODEL_NAME	"IT Device"

#define NONE_SPACE	0
#define MEM_SPACE	1
#define IO_SPACE	2

#pragma pack(push,1)
static	HANDLE	drvHandle;		// Driver Handle
static	DEVDATA devData[MAXDEV];

typedef struct DEV_NAMES
{ U32 baseId; U32 subId; char *Name; } 
_DEV_NAMES;

static DEV_NAMES devnames[] = 
{
	{0x6620, 0x0000 , "IT DSP6678PEX"},
	{0x5511, 0x1030 , "IT PE510K1" }, // FMC107P+FM412x500M - digitizer PE510K1
	{0x5511, 0xFFFF , "IT FMC107P"},
	{0x53B5, 0x1030 , "IT PE510A1" }, // FMC121cP+FM412x500M - digitizer PE510A1
	{0x53B5, 0xFFFF , "IT FMC121cP" },
	{0,0,NULL}
};

static int GetModelName(int idx, char *value)
{
	int i;
	for (i = 0; i < sizeof(devnames); i++) {
		if ((devnames[i].baseId == devData[idx].pciHdr.DID) &&
			(devnames[i].subId == devData[idx].Info.subunitType[0])) {
			strcpy(value, devnames[i].Name);
			break;
		}
	}
	if (i == sizeof(devnames)) {
		strcpy(value, MODEL_NAME);
		return 1;
	}
	return 0;
}


DllExport ViStatus	STDCALL PpiInitializePlugin(void)
{
	// Open IS_DRV
	drvHandle = UniDrv_Open(0);
	if (drvHandle == 0)
	{
		//printf("\nITVIPPI ERROR: Can't Open ISDRV\n\n");
		return VI_ERROR_SYSTEM_ERROR;
	}
	
	return VI_SUCCESS;
}

DllExport ViStatus	STDCALL PpiOpen(
	ViInt32 intfc,
	ViInt32 bus,
	ViInt32 device,
	ViInt32 function,
	PpiHandle* handle
)
{
	// find PCI device
	for (int i=0; i < MAXDEV; i++) 
	{
		if( (bus    == devData[i].pciDev.bus) &&
			(device == devData[i].pciDev.tar.ByBits.dno) &&
			(function == devData[i].pciDev.tar.ByBits.fno) )
		{ 
			*handle = (PpiHandle)i;
		}
	}

	return VI_SUCCESS;
}



DllExport ViStatus	STDCALL PpiGetSpaceInfo(
	PpiHandle handle,
	PpiSpace space,
	ViPInt16 spaceType,
	ViPUInt64 spaceBase,
	ViPUInt64 spaceSize
)
{
	int idx = (int)handle;
	int sp = (int)space;

	*spaceType = MEM_SPACE;
	switch (space) 
	{
		case Bar0:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[0] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[0]);
			break;
		}
		case Bar1:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[1] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10+4, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10+4, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10+4, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[1]);
			break;
		}
		case Bar2:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[2] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 8, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 8, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 8, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[2]);
			break;
		}
		case Bar3:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[3] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 12, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 12, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 12, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[3]);
			break;
		}
		case Bar4:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[4] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 16, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 16, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 16, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[4]);
			break;
		}
		case Bar5:
		{
			*spaceBase = devData[idx].pciHdr.Hdr.Type00h.BADR[5] & (~0xF);
			ULONG size_bar = 0xFFFFFFFF;
			int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 20, 4, &size_bar);
			ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 20, 4, &size_bar);
			size_bar &= ~0xFF;
			size_bar = ~size_bar + 1;
			*spaceSize = size_bar;
			ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, 0x10 + 20, 4, &devData[idx].pciHdr.Hdr.Type00h.BADR[5]);
			break;
		}
	}

	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiGetDeviceAttribute(
	PpiHandle handle,
	ViAttr attributeID,
	void * attributeValue
)
{
	int idx = (int)handle;
	switch (attributeID) {
	case VI_ATTR_PXI_DEV_NUM:
		{
			int *value = (int*)attributeValue;
			*value = devData[idx].pciDev.tar.ByBits.dno;
			break;
		}
	case VI_ATTR_PXI_FUNC_NUM:
		{
			int *value = (int*)attributeValue;
			*value = devData[idx].pciDev.tar.ByBits.fno;
			break;
		}
	case VI_ATTR_PXI_BUS_NUM:
		{
			int *value = (int*)attributeValue;
			*value = devData[idx].pciDev.bus;
			break;
		}
	case VI_ATTR_PXI_ALLOW_WRITE_COMBINE:
		{
			ViBoolean *value = (ViBoolean*)attributeValue;
			*value = VI_TRUE;// FALSE;
			break;
		}
	case VI_ATTR_DMA_ALLOW_EN:
		{
			ViBoolean *value = (ViBoolean*)attributeValue;
			*value = VI_FALSE;
			break;
		}
	case VI_ATTR_MANF_ID:
		{
			ViUInt16 *value = (ViUInt16*)attributeValue;
			*value = devData[idx].pciHdr.VID;
			break;
		}
	case VI_ATTR_MANF_NAME:
		{
			ViChar *value = (ViChar*)attributeValue;
			strcpy(value, MANF_NAME);
			break;
		}
	case VI_ATTR_MODEL_NAME:
		{
			ViChar *value = (ViChar*)attributeValue;
			GetModelName(idx,value);
//			strcpy(value, MODEL_NAME);
			break;
		}
	case VI_ATTR_MODEL_CODE:
		{
			ViUInt16 *value = (ViUInt16*)attributeValue;
			*value = devData[idx].pciHdr.DID;
			break;
		}

	default: 
		return VI_ERROR_INV_PARAMETER;
	}
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiMapMemory(
	PpiHandle handle,
	PpiSpace space,
	ViUInt64 offset,
	PpiLength length,
	void ** userSpaceMem
)
{
	int idx = (int)handle;
	int sp = (int)space;
	ULONG64 addr;

	addr = devData[idx].pciHdr.Hdr.Type00h.BADR[space] & (~0xF);
	addr += offset;
	
	if (0 > UniDrv_Mem_Map(drvHandle, length, addr, userSpaceMem))
	{
		return VI_ERROR_SYSTEM_ERROR;
	}
	devData[idx].userSpaceMem[space] = *userSpaceMem;

	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiUnmapMemory(
	PpiHandle handle,
	ViAddr userSpaceMem
)
{
	UniDrv_Mem_UnMap(drvHandle, userSpaceMem);

	return VI_SUCCESS;
}

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
)
{
	int idx = (int)handle;
	int sp = (int)space;

	if (sp == 6) // config space
	{
		if (width == 1) {
			UINT8 *buf = (UINT8*)writeBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
		else if (width == 2) {
			UINT16 *buf = (UINT16*)writeBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
		else if (width == 4) {
			UINT32 *buf = (UINT32*)writeBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
		else if (width == 8) {
			UINT64 *buf = (UINT64*)writeBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_WriteCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
	}
	else {
		return VI_ERROR_INV_PARAMETER;
	}

	return VI_SUCCESS;
}

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
)
{
	int idx = (int)handle;
	int sp = (int)space;

	if (sp == 6) // config space
	{
		if (width == 1) {
			UINT8 *buf = (UINT8*)readBuffer;
			for (int i=0; i < (int)count; i++) {
				int ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset+i, (USHORT)width, buf+i);
			}
		} else if (width == 2) {
			UINT16 *buf = (UINT16*)readBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
		else if (width == 4) {
			UINT32 *buf = (UINT32*)readBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
		else if (width == 8) {
			UINT64 *buf = (UINT64*)readBuffer;
			for (int i = 0; i < (int)count; i++) {
				int ret = UniDrv_PCI_ReadCfgData(drvHandle, &devData[idx].pciDev, (USHORT)offset + i, (USHORT)width, buf + i);
			}
		}
	} else {
		return VI_ERROR_INV_PARAMETER;
	}

	return VI_SUCCESS;
}
  
DllExport	ViStatus	STDCALL PpiEnableInterrupts(
	PpiHandle handle,
	ViUInt16 queueLength
)
{
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiWaitInterrupt(
	PpiHandle handle,
	ViUInt32 timeoutMilliseconds,
	ViPInt16 interruptSequence,
	ViPUInt32 interruptData
)
{
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiDisableAndAbortWaitInterrupt(
	PpiHandle handle
)
{
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiTerminateIO(
	PpiHandle handle,
	void* buffer
)
{
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiClose(
	PpiHandle handle
)
{
	return VI_SUCCESS;
}

DllExport	ViStatus	STDCALL PpiFinalizePlugin(void)
{
//	UniDrv_Close(drvHandle);
	return VI_SUCCESS;
}


#pragma pack(push,1)

static	PCI_DEVICE	pciDev;
static	PCI_HEADER	pciHdr;

DllExport ViStatus	STDCALL PpiGetDeviceIDs(
	ViBoolean includeNonPrimary,
	ViInt32 arrayElementCount,
	ViAUInt64 deviceIdArray,
	ViABoolean isPrimaryArray,
	ViPInt32 deviceCount
)
{

	int         nbus = 0, ndev = 0, nfunc = 0;
	int         ret;
	int			count = 0;

	// Get ALL INSYS PCI(e) Devices
	*deviceCount = 0;
	for (nbus = 0; nbus < 256; nbus++)
	{
		for (ndev = 0; ndev < 32; ndev++)
		{
			pciDev.bus = nbus;
			pciDev.tar.ByBits.fno = 0;
			pciDev.tar.ByBits.dno = ndev;
			ret = UniDrv_PCI_ReadCfgSpace(drvHandle, &pciDev, &pciHdr);
			if (ret != 0)
				continue;
			if ((pciHdr.VID == 0) || (pciHdr.VID == 0xFFFF))
				continue;
			if ((pciHdr.VID != 0x4953)) continue;
			unsigned __int64 visa_interface = 0;// VI_INTF_PXI;
			//unsigned __int64 visa_interface = VI_INTF_PXI;
			unsigned __int64 busnum = pciDev.bus;
			unsigned __int64 devnum = pciDev.tar.ByBits.dno;
			unsigned __int64 devfunc = pciDev.tar.ByBits.fno;
			deviceIdArray[count] = (visa_interface << 48) | (busnum << 32) | (devnum << 16) | (devfunc << 0);
			devData[count].pciDev = pciDev;
			devData[count].pciHdr = pciHdr;
			isPrimaryArray[count] = VI_TRUE;
			count++;
			if (count >= arrayElementCount) break;
		}
	}

	*deviceCount = count;

	if (count > arrayElementCount)
		return VI_ERROR_INV_LENGTH;

	// BARDY Initialize
	// make init string
	//const char ini_str[256] = "[BASE ENUM]\nbambpex\nb6678pex\n";
	BRDCHAR	ini_str[256] = _BRDC("[BASE ENUM]\nbambpex\nb6678pex\n"); 
	S32 bNum;
	S32 err = BRD_initEx(BRDinit_STRING | BRDinit_AUTOINIT, ini_str, NULL, &bNum);

	BRD_LidList lidList;
	lidList.item = MAXDEV;
	lidList.pLID = new U32[MAXDEV];
	S32 status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	BRD_Info Info;
	for (int i = 0; i < lidList.itemReal; i++)
	{
		Info.size = sizeof(BRD_Info);
		BRD_getInfo(lidList.pLID[i], &Info);
		for (int j = 0; j < lidList.itemReal; j++) {
			if ((devData[j].pciDev.bus == Info.bus) &&
				(devData[j].pciDev.tar.ByBits.dno == Info.dev) ) {
				devData[j].Info = Info;
				devData[j].lid = lidList.pLID[i];
			}
		}
	}
	delete lidList.pLID;

	BRD_cleanup();

	return VI_SUCCESS;
}


