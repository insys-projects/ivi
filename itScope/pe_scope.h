/*****************************************************************************
* (c) 2017, Instrumental Systems, Corporation.  All Rights Reserved. *
*****************************************************************************/

/*****************************************************************************
*  PE_SCOPE - Digital Oscilloscope PE Instrument Driver header
*  Original Release: March, 2017
*  By: Do, InSys
*      do.insys@gmail.com
*
*  Modification History:
*
*       April, 2017 - Instrument Driver Created.
*
*****************************************************************************/

#ifndef __PE_SCOPE_H
#define __PE_SCOPE_H


#include <string.h>
#include <stdio.h>  
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "brd.h"
#include "extn.h"
#include "utypes.h"
#include "gipcy.h"
#include "ctrladc.h"
#include "ctrlstrm.h"

#include "itScope.h"

/* Global variables*/
typedef struct SCOPE_ATTRS {
	ViAttr attributeId;
	ViSession	vi;
}SCOPE_ATTRS;

typedef struct SCOPE_DEVICES {
	BRD_Handle	brdHandle;
	BRD_Handle	hADC;
	ViSession	vi;
	U32			iDev;
	BRDctrl_StreamCBufAlloc buf_dscr; // описание буфера стрима
	U32			MemAsFifo;				// 1 - использовать динамическую память на модуле в качестве FIFO
}SCOPE_DEVICES;

typedef struct DEV_NAMES
{
	U32 baseId; U32 subId; char *Name;
}DEV_NAMES;


#define		MAX_NAME	64		// считаем, что имя устройства/ПЛИС/службы может быть не более 64 символов 
#define		MAX_SRV		16		// считаем, что служб на одном модуле может быть не больше MAX_SRV
#define		MAX_PU		8		// считаем, что PU-устройств (ПЛИС, ППЗУ) на одном модуле может быть не больше MAX_PU
#pragma pack(push,1)
									// информация об устройстве
typedef struct
{
	U32			size;			// sizeof(DEV_INFO)
	U16			devID;			// Device ID
	U16			rev;			// Revision
	BRDCHAR		devName[MAX_NAME];	// Device Name
	U32			pid;			// Board Phisical ID
	S32			bus;			// Bus Number
	S32			dev;			// Dev Number
	S32			slot;			// Slot Number
	U08			pwrOn;			// FMC power on/off
	U32			pwrValue;		// FMC power value
	U16			pldVer;			// ADM PLD version
	U16			pldMod;			// ADM PLD modification
	U16			pldBuild;		// ADM PLD build
	BRDCHAR		pldName[MAX_NAME];	// ADM PLD Name
	U16			subType;		// Subunit Type Code
	U16			subVer;			// Subunit version
	U32			subPID;			// Subunit serial number
	BRDCHAR		subName[MAX_NAME];	// Submodule Name
	BRDCHAR		srvName[MAX_NAME][MAX_SRV];	// massive of Service Names
} DEV_INFO, *PDEV_INFO;

// информация о рабочих параметрах АЦП
typedef struct
{
	U32		size;			// sizeof(ADC_PARAM)
	U32		chmask;			// mask of ON channels
	U32		clkSrc;			// clock source
	REAL64	clkValue;		// clock value
	REAL64	rate;			// sampling rate
} ADC_PARAM, *PADC_PARAM;


ViStatus _VI_FUNCC _viPrintf(ViSession vi, ViString writeFmt, ...);
ViStatus _VI_FUNC  _viRead(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus _VI_FUNCC _viQueryf(ViSession vi, ViString writeFmt, ViString readFmt, ...);
ViStatus _VI_FUNC  _viBufWrite(ViSession vi, ViBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus _VI_FUNC  _viBufRead(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
ViStatus _VI_FUNCC _viScanf(ViSession vi, ViString readFmt, ...);
ViStatus _VI_FUNCC _viClose(ViSession vi);
ViStatus _VI_FUNC  _viSetAttribute(ViObject vi, ViAttr attrName, ViAttrState attrValue);
ViStatus _VI_FUNC  _viGetAttribute(ViObject vi, ViAttr attrName, void _VI_PTR attrValue);

ViStatus	scpi_hwInit(ViSession vi, ViChar *resourceName);
ViStatus	scpi_getcmdstr(char *str, char *buf);
ViStatus	scpi_getvalstr(char *str, char *buf);
ViStatus	scpi_maskcmp(const char *str, const char *mask);
ViStatus	scpi_hwInit(ViSession vi, ViChar *resourceName);
ViStatus	scpi_getIDN(ViSession vi, ViChar *idnStr);
ViStatus	scpi_self_test(ViSession vi);
ViStatus	scpi_reset(ViSession vi);
ViStatus	scpi_TRG(ViSession vi);
ViStatus	scpi_sav_wave(ViSession vi, ViChar *str);
ViStatus	scpi_DDT(ViSession vi, ViChar *str);
ViStatus	scpi_autos_exec(ViSession vi);
ViStatus	scpi_CLS(ViSession vi);
ViStatus	scpi_getEVM(ViSession vi, ViUInt32 *err, ViChar *str);
ViStatus	scpi_getESR(ViSession vi, ViUInt32 *val);
ViStatus	scpi_get_acq_nume(ViSession vi, ViChar *str);

ViStatus	pe_open_default(ViSession vi, ViInt32   model);
ViStatus	pe_close(ViSession vi);
ViStatus	pe_read(ViSession vi, PVOID pSig, ULONG bBufSize);
ViStatus	set_param_default(ViSession vi, ViInt32   model);

BRD_Handle get_brd_handle(ViSession vi);
U32	get_brd_idev(ViSession vi);
SCOPE_DEVICES	*get_scopeDev(ViSession vi);

S32 DEV_info(ViSession vi, BRD_Handle hDEV, int iDev, DEV_INFO* pdevcfg);
S32	adc_open_default(ViSession vi, ViInt32   model);
S32	adc_create_ini_file(ViSession vi, BRDCHAR *adcsrv, BRDCHAR *iniFilePath, BRDCHAR *iniSectionName);

BRD_Handle ADC_open(ViSession vi, BRD_Handle hDEV, BRDCHAR* adcsrv, BRD_AdcCfg* adcfg);
// установить рабочие параметры АЦП
int ADC_set(ViSession vi, int idev, BRDCHAR* adcsrv, BRDCHAR* inifile, ADC_PARAM* adcpar);
// размещение буфера для получения данных с АЦП через Стрим
S32 ADC_allocbuf(ViSession vi);
// выполнить сбор данных в FIFO с ПДП-методом передачи в ПК
int ADC_read(ViSession vi);
// освобождение буфера стрима
S32 ADC_freebuf(ViSession vi);
// закрыть АЦП
int ADC_close(ViSession vi);


S32	adc_set_channel(ViSession vi, int chan, int stat);
S32	adc_get_channel(ViSession vi, int chan, int *stat);

#endif
