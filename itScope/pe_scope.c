/*****************************************************************************
* (c) 2017, Instrumental Systems, Corporation.  All Rights Reserved. *
*****************************************************************************/

/*****************************************************************************
*  PE_SCOPE - Digital Oscilloscope PE Instrument Driver
*  Original Release: March, 2017
*  By: Do, InSys
*      do.insys@gmail.com
*
*  Modification History:
*
*       April, 2017 - Instrument Driver Created.
*
*****************************************************************************/

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
#include "ctrlsdram.h"

#include "itScope.h"
#include "pe_scope.h"

/* Global variables*/
SCOPE_DEVICES scopeDev[MAXDEV];
static DEV_NAMES devnames[] =
{
	{ 0x5511, 0x1030 , "INSYS,PE510K1" }, // FMC107P+FM412x500M - digitizer PE510K1
	{ 0x53B5, 0x1030 , "INSYS,X510A1" }, // FMC121cP+FM412x500M - digitizer X510A1
	{ 0,0,NULL }
};

BRDCHAR	*AdcSrvName[] = {
	_BRDC("FM412x500M0"),			// PE510K1
	_BRDC("FM412x500M0")			// PE520K1
};

char	*iniStrTemplate[] = {
	"[Option]\n\
AdcServiceName = FM412X500M; Имя службы АЦП(не изменять)\n\
//PldFileName=ambpcd_v10_adm214x200m.mcs	//Файл прошивки ПЛИС ADM\n\
\n\
isPldLoadAlways = 0	// Как загружать прошивку ПЛИС ADM: 0 - если она не загружена, 1 - всегда при запуске программы\n\
BusMasterEnable = 1	//Режим для отладки: 1 - всегда (0 - только для отладки)\n\
\n\
Cycle = 1			//Циклический режим ввода в буфер: 0 - однократный, 1 - циклический (для завершения - ESC)\n\
					// если число циклов > 1, то сбор осуществляется заданное число раз и записывается в один файл (IsWriteFile=1 или 2)\n\
DaqIntoMemory = 0	// Сбор данных в память: 0-сразу в ПЭВМ, 1-сбор в память базового модуля, 2-память как FIFO (если поддержено в прошивке)\n\
IsWriteFile = 1		//Режим формирования файла data.bin\n\
					// 0 - ничего не пишется в файл\n\
					// 1 - все что собирается пишется в файл как есть + параметры для ISVI\n\
					// 2 - все что собирается пишется в файл как есть\n\
					// 4 - все что собирается пишется в разделяемую память + параметры для ISVI\n\
SamplesPerChannel = 131072		// Число собираемых отсчётов на канал при сборе не в память модуля,\n\
								// а при сборе в память модуля - размер промежуточного буфера в памяти ПК\n\
IsSystemMemory = 0	//Тип используемой памяти для составного буфера\n\
					// 0-пользовательская память (доступен максимальный размер)\n\
					// 1-системная память (доступна максимальная скорость)\n\
MemSamplesPerChan = 32768		// Число собираемых отсчётов на канал для сбора в память\n\
BaseClk2Src = BGEN	// Источник тактовой частоты, подаваемой с базмодуля на субмодуль: BGEN, CLKIN, DDSCLK, DIVCLK\n\
\n\
AdcDrqFlag = 2		// Флаг FIFO для формирования запроса ПДП для АЦП: 0-почти пустое, 1-пустое, 2-половина FIFO\n\
MemDrqFlag = 2		// Флаг FIFO для формирования запроса ПДП для памяти : 0-почти пустое, 1-пустое, 2-половина FIFO\n\
\n\
DirFileName = exam.bin; имя файла для прямой записи\n\
DirFileBufSize = 4096; размер промежуточного буфера(Кбайт) при прямой записи в файл\n\
DirNumBlock = 4; число промежуточных буферов при прямой записи в файл\n\
DirNumBufWrite = 0; число буферов записываемых в файл(если больше нуля, то режим прямой записи в файл, \n\
				; если - 1, то режим зацикленного сбора в память с передачей 1 - го буфера в разделяемую память)\n\
				; размер файла равен DirFileBufSize*DirNumBufWrite(Кбайт)\n\
\n\n\
[device0_fm412x500m0]\n\
MasterMode = 1		// 1 - всегда\n\
ChannelMask = 0x1		// Маска каналов АЦП: 1-к0, 2-к1, 4-к2, 8-к3, 3-к0-1, 0xF-к0-3\n\
ClockSource = 1		// Тактовый генератор: 0-выкл., 1-внутренний, 2-внутренний+PLL, 4-от базмодуля, 0x81-внешний, 0x82-внешний+PLL\n\
ExternalClockValue = 100000000		// Частота внешнего тактового генератора (Гц)\n\
BaseClockValue = 100000000		// Частота тактового генератора на базовом модуле (Гц)\n\
SubClockValue = 100000000		// Частота генератора Si571 на субмодуле, если выбран ClockSource=2 (Гц)\n\
SamplingRate = 500000000		// Частота дискретизации (Гц). (от 10 до 500 МГц)\n\
DataFormat = 0		// Формат отсчетов сигнала: 0 - 16 бит, 1 - 8 бит\n\
\n\
StartSource = 3		// Источник старта на субмодуле: 2 - внешний. 3 - программный\n\
StartLevel = 0		// Уровень старта (В): от -2.5 В до +2.5 В – при внешнем старте\n\
StartResistance = 0		// Входное сопротивление входа внешнего старта: 0 - 2.5 кOм, 1 - 50 Oм\n\
\n\
StartBaseSource = 0		// Базовый источник старта: 0 - программный, 7 - от субмодуля, 10 - от DDS\n\
StartBaseInverting = 0		// Инверсия базового источника старта: 0-нет, 1-есть\n\
StartMode = 1		// Тип старта: 1 – тригерный, 0 – потенциальный\n\
StopSource = 0			// Базовый источник останова при тригерном старте: 0 - программный, 7 - от субмодуля, 10 - от DDS\n\
\n\
StopInverting = 0			// Инверсия базового источника останова: 0-нет, 1-есть\n\
ReStart = 0				// Режим автоматического рестарта: 0-нет, 1-есть\n\
\n\
IsPreTriggerMode = 0		// Режим работы претриггера: 0-выкл., 1-простой, 2-гарантированный, 3-от SDRAM\n\
PreTriggerSamples = 128		// Размер претриггера в отсчетах\n\
\n\
StartDelayEnable = 0		// Разрешение счетчика CNT0: 0 - не разрешен, 1 - разрешен\n\
StartDelayCounter = 0		// Содержимое счетчика CNT0\n\
AcquiredSampleEnable = 0	// Разрешение счетчика CNT1: 0 - не разрешен, 1 - разрешен\n\
AcquiredSampleCounter = 0	// Содержимое счетчика CNT1\n\
SkipSampleEnable = 0		// Разрешение счетчика CNT2: 0 - не разрешен, 1 - разрешен\n\
SkipSampleCounter = 0		// Содержимое счетчика CNT2\n\
\n\
TitleEnable = 0			// Разрешение заголовка в кадрах: 0 - не разрешен, 1 - разрешен\n\
TitleSize = 0			// Размер заголовка в 32-разрядных словах\n\
TitleData = 0			// Данные заголовка: целое 32-разрядное число без знака\n\
\n\
//\n\
// Настройка делителей PLL AD9518 для режима 2 (внутренний генератор + ФАПЧ)\n\
//\n\
@AD9518DividerA = 0\n\
@AD9518DividerB = 5\n\
@AD9518DividerP = 8\n\
@AD9518DividerR = 4\n\
@AD9518DividerDo = 4\n\
@AD9518DividerPll = 2\n\
@AD9518DividerRate = 250000000		// Частота дискретизации, соответствующая данным делителям (Гц)\n\
\n"
};


typedef struct UTYPESTR {
	ViUInt32	typeVal;
	char		typeStr[32];
}UTYPESTR;

static UTYPESTR acq_type[] =
{
	{ ITSCOPE_VAL_NORMAL, "SAMPLE" },
	{ ITSCOPE_VAL_HI_RES, "HIRES" },
	{ ITSCOPE_VAL_PEAK_DETECT,"PEAKDETECT" },
	{ ITSCOPE_VAL_ENVELOPE, "ENVELOPE" },
	{ ITSCOPE_VAL_AVERAGE, "AVERAGE" },
	{ VI_NULL, VI_NULL }
};

static UTYPESTR trig_type[] =
{
	{ ITSCOPE_VAL_EDGE_TRIGGER, "EDGE" },
	{ ITSCOPE_VAL_RUNT_TRIGGER, "RUNT" },
	{ ITSCOPE_VAL_GLITCH_TRIGGER, "GLITCH" },
	{ ITSCOPE_VAL_TRANSITION_TRIGGER, "TRANSITION" },
	{ ITSCOPE_VAL_STATE_TRIGGER, "STATE" },
	{ ITSCOPE_VAL_PATTERN_TRIGGER, "PATTERN" },
	{ ITSCOPE_VAL_SETHOLD_TRIGGER, "SETHOLD" },
	{ ITSCOPE_VAL_TV_TRIGGER, "VIDEO" },
	{ ITSCOPE_VAL_AC_LINE_TRIGGER, "AC" },
	{ VI_NULL, VI_NULL }
};

typedef struct ITSCOPE_PARAM {
	ViUInt32	ACQ_MOD;
	ViUInt32	ACQ_STATE;
	ViUInt32	ACQ_STOPA;
	ViUInt32	ACQ_NUME;
	ViUInt32	OPC_STATE;
	ViReal64	HOR_SCA;
	ViReal64	HOR_DEL_TIM;
	ViUInt64	HOR_RECORDL;
	ViReal64	HOR_MAI_SAMPLER;
	ViUInt32	MATH_EN;
	ViUInt32	REF_EN[MAXCHAN];
	ViUInt32	CH_EN[MAXCHAN];
	ViReal64	CH_PRO_GAIN[MAXCHAN];
	ViUInt32	CH_COUP[MAXCHAN];
	ViReal64	CH_SCA[MAXCHAN];
	ViReal64	CH_OFFS[MAXCHAN];
	ViReal64	CH_IMP[MAXCHAN];
	ViReal64	CH_BAN[MAXCHAN];
	ViUInt32	TRIG_A_TYP;
	ViUInt32	TRIG_A_EDGE_COUP;
	ViReal64	TRIG_A_HOLD_TIM;
	ViUInt32	TRIG_A_EDGE_SOU;
	ViReal64	TRIG_A_LEV;
	ViUInt32	TRIG_A_EDGE_SLO;
	ViReal64	TRIG_B_LEV;
	ViReal64	TRIG_A_PUL_WID_WID;
	ViUInt32	TRIG_A_PUL_WID_POL;
	ViUInt32	TRIG_A_PUL_WID_WHE;
	ViUInt32	TRIG_A_PUL_RUNT_POL;
	ViUInt32	TRIG_A_VID_STAN;
	ViUInt32	TRIG_A_VID_FIELD;
	ViUInt32	TRIG_A_VID_LINE;
	ViUInt32	TRIG_A_VID_POL;
	ViUInt32	DAT_SOU_CH;
	ViUInt32	DAT_WID;
	ViUInt32	DAT_STAR;
	ViUInt32	DAT_STOP;
	ViReal64	WFMPRE_YZERO;
	ViReal64	WFMPRE_XZE;
	ViReal64	WFMPRE_XINCR;
	ViReal64	WFMPRE_YOFF;
	ViReal64	WFMPRE_YMULT;
	ViUInt32	ESR;
	ViChar		EVM[256];
	ViUInt32	ESE;
	ViUInt32	SRE;
	ViUInt32	VERBOSE;
	ViUInt32	HEAD;
	ViUInt32	measu_refl_meth;
	ViUInt32	hor_del_state;
	ViUInt32	dat_enc;
}ITSCOPE_PARAM;
ITSCOPE_PARAM itscope_param[MAXDEV];


char		retStr[MAXDEV][256];
ViUInt32	retCnt[MAXDEV];
#define		RET_STATE_NORMAL		0
#define		RET_STATE_CURVE1		11
#define		RET_STATE_CURVE2		12
#define		RET_STATE_CURVE3		13
ViUInt32	retState[MAXDEV];

// вспомогательная функция для DEV_info
//=***************************************************************************************
void SubmodName(ULONG id, BRDCHAR * str)
{
	switch (id)
	{
	case 0x1010:    BRDC_strcpy(str, _BRDC("FM814x125M")); break;
	case 0x1012:    BRDC_strcpy(str, _BRDC("FM814x250M")); break;
	case 0x1020:    BRDC_strcpy(str, _BRDC("FM214x250M")); break;
	case 0x1030:    BRDC_strcpy(str, _BRDC("FM412x500M")); break;
	case 0x1040:    BRDC_strcpy(str, _BRDC("FM212x1G")); break;
	default: BRDC_strcpy(str, _BRDC("UNKNOW")); break;
	}
}

void DisplayError(S32 status, const char* funcName, const BRDCHAR* cmd_str)
{
	S32	real_status = BRD_errext(status);
	BRDCHAR func_name[MAX_PATH];
#ifdef _WIN64
	mbstowcs(func_name, funcName, MAX_PATH);
#else
	BRDC_strcpy(func_name, funcName);
#endif 
	BRDCHAR msg[255];

#ifdef _WIN64	
	switch (real_status)
	{
	case BRDerr_OK:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_OK\n"), func_name, cmd_str);
		break;
	case BRDerr_BAD_MODE:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_BAD_MODE\n"), func_name, cmd_str);
		break;
	case BRDerr_INSUFFICIENT_SERVICES:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_INSUFFICIENT_SERVICES\n"), func_name, cmd_str);
		break;
	case BRDerr_BAD_PARAMETER:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_BAD_PARAMETER\n"), func_name, cmd_str);
		break;
	case BRDerr_BUFFER_TOO_SMALL:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_BUFFER_TOO_SMALL\n"), func_name, cmd_str);
		break;
	case BRDerr_WAIT_TIMEOUT:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: BRDerr_WAIT_TIMEOUT\n"), func_name, cmd_str);
		break;
	default:
		BRDC_sprintf_s(msg, 256, _BRDC("%s - %s: Unknown error, status = %8X\n"), func_name, cmd_str, real_status);
		break;
	}
#else
	switch (real_status)
	{
	case BRDerr_OK:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_OK\n"), func_name, cmd_str);
		break;
	case BRDerr_BAD_MODE:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_BAD_MODE\n"), func_name, cmd_str);
		break;
	case BRDerr_INSUFFICIENT_SERVICES:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_INSUFFICIENT_SERVICES\n"), func_name, cmd_str);
		break;
	case BRDerr_BAD_PARAMETER:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_BAD_PARAMETER\n"), func_name, cmd_str);
		break;
	case BRDerr_BUFFER_TOO_SMALL:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_BUFFER_TOO_SMALL\n"), func_name, cmd_str);
		break;
	case BRDerr_WAIT_TIMEOUT:
		BRDC_sprintf(msg, _BRDC("%s - %s: BRDerr_WAIT_TIMEOUT\n"), func_name, cmd_str);
		break;
	default:
		BRDC_sprintf(msg, _BRDC("%s - %s: Unknown error, status = %8X\n"), func_name, cmd_str, real_status);
		break;
	}
#endif

	BRDC_printf(_BRDC("%s"), msg);
}

// получить информацию об открытом устройстве
// hDEV - (IN) дескриптор открытого устройства
// iDev - (IN) порядковый номер LID (в массиве лидов) c описанием нужного устройства
// pdevcfg - (OUT) заполняемая информацией об устройстве структура
// srv_name - (OUT) массив имен служб
S32 DEV_info(ViSession vi, BRD_Handle hDEV, int iDev, DEV_INFO* pdevcfg)
{
	S32		status;

	BRD_Info	info;
	info.size = sizeof(info);

	// получить список LID (каждая запись соответствует устройству)
	BRD_LidList lidList;
	lidList.item = MAXDEV;
	lidList.pLID = (U32*)malloc(MAXDEV * sizeof(U32));
	status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	BRD_getInfo(lidList.pLID[iDev], &info); // получить информацию об устройстве
	pdevcfg->devID = info.boardType >> 16;
	pdevcfg->rev = info.boardType & 0xff;
	BRDC_strcpy(pdevcfg->devName, info.name);
	pdevcfg->pid = info.pid;
	pdevcfg->bus = info.bus;
	pdevcfg->dev = info.dev;
	pdevcfg->slot = info.slot;
	pdevcfg->subType = info.subunitType[0];

	BRDCHAR subName[MAX_NAME];
	BRDC_strcpy(pdevcfg->subName, _BRDC("NONE"));
	if (info.subunitType[0] != 0xffff)
	{
		SubmodName(info.subunitType[0], subName);
		BRDC_strcpy(pdevcfg->subName, subName);
	}

	BRDC_strcpy(pdevcfg->pldName, _BRDC("ADM PLD"));
	U32 ItemReal;
	BRD_PuList PuList[MAX_PU];
	status = BRD_puList(hDEV, PuList, MAX_PU, &ItemReal); // получить список программируемых устройств
	if (ItemReal <= MAX_PU)
	{
		for (U32 j = 0; j < ItemReal; j++)
		{
			U32	PldState;
			status = BRD_puState(hDEV, PuList[j].puId, &PldState); // получить состояние ПЛИС
			if (PuList[j].puId == 0x100 && PldState)
			{// если это ПЛИС ADM и она загружена
				BRDC_strcpy(pdevcfg->pldName, PuList[j].puDescription);
				pdevcfg->pldVer = 0xFFFF;
				BRDextn_PLDINFO pld_info;
				pld_info.pldId = 0x100;
				status = BRD_extension(hDEV, 0, BRDextn_GET_PLDINFO, &pld_info);
				if (BRD_errcmp(status, BRDerr_OK))
				{
					pdevcfg->pldVer = pld_info.version;
					pdevcfg->pldMod = pld_info.modification;
					pdevcfg->pldBuild = pld_info.build;
				}
			}
			if (PuList[j].puId == 0x03)
			{// если это ICR субмодуля
				if (PldState)
				{ // и оно прописано данными
					char subICR[14];
					status = BRD_puRead(hDEV, PuList[j].puId, 0, subICR, 14);
					U16 tagICR = *(U16*)(subICR);
					pdevcfg->subPID = *(U32*)(subICR + 7); // серийный номер субмодуля
					pdevcfg->subType = *(U16*)(subICR + 11);  // тип субмодуля
					pdevcfg->subVer = *(U08*)(subICR + 13);   // версия субмодуля
					SubmodName(pdevcfg->subType, subName);
					BRDC_strcpy(pdevcfg->subName, subName);
				}
			}
		}
	}
	free(lidList.pLID);

	// получаем состояние FMC-питания (если не FMC-модуль, то ошибка)
	pdevcfg->pwrOn = 0xFF;
	BRDextn_FMCPOWER power;
	power.slot = 0;
	status = BRD_extension(hDEV, 0, BRDextn_GET_FMCPOWER, &power);
	if (BRD_errcmp(status, BRDerr_OK))
	{
		pdevcfg->pwrOn = power.onOff;
		pdevcfg->pwrValue = power.value;
	}

	// получить список служб
	BRD_ServList srvList[MAX_SRV];
	status = BRD_serviceList(hDEV, 0, srvList, MAX_SRV, &ItemReal);
	if (ItemReal <= MAX_SRV)
	{
		U32 j = 0;
		for (j = 0; j < ItemReal; j++)
			BRDC_strcpy(pdevcfg->srvName[j], srvList[j].name);
		BRDC_strcpy(pdevcfg->srvName[j], _BRDC(""));
	}
	else
		BRDC_printf(_BRDC("BRD_serviceList: Real Items = %d (> 16 - ERROR!!!)\n"), ItemReal);

	return 0;
}

BRD_Handle get_brd_handle(ViSession vi)
{
	BRD_Handle handle = IVI_ERROR_RESOURCE_UNKNOWN;
	for (int ii = 0; ii < MAXDEV; ii++) {
		if (scopeDev[ii].vi == vi) {
			handle = scopeDev[ii].brdHandle;
			break;
		}
	}
	return handle;
}

U32	get_brd_idev(ViSession vi)
{
	U32	idev = -1;
	for (int ii = 0; ii < MAXDEV; ii++) {
		if (scopeDev[ii].vi == vi) {
			idev = scopeDev[ii].iDev;
			break;
		}
	}
	return idev;
}

SCOPE_DEVICES	*get_scopeDev(ViSession vi)
{
	int ii;
	for (ii = 0; ii < MAXDEV; ii++) {
		if (scopeDev[ii].vi == vi) break;
	}
	if (ii == MAXDEV) return 0;
	return &scopeDev[ii];
}

//BRDCHAR g_AdcSrvName[MAX_NAME] = _BRDC("FM412x500M0"); // имя службы с номером 

S32	adc_open_default(ViSession vi, ViInt32   model)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);


	//====================================================================================
	// открыть АЦП и получить информацию о нем 
	BRD_AdcCfg adc_cfg;
	BRD_Handle hADC = ADC_open(vi, handle, AdcSrvName[model - 1], &adc_cfg);
	if (hADC <= 0)
	{
		BRDC_printf(_BRDC("ERROR by ADC service (%s) capture\n"), AdcSrvName[model - 1]);
		BRD_close(handle); // закрыть устройство
		return IVI_ERROR_RESOURCE_UNKNOWN;
	}
	scope_dev->hADC = hADC;

	//====================================================================================
	// установить параметры работы АЦП
	ADC_PARAM adc_param;
	adc_param.size = sizeof(ADC_PARAM);
	BRDCHAR adcIniFile[128];
#ifdef _WIN64
	BRDC_sprintf_s(adcIniFile, 128, _BRDC("adc_%d.ini"), model);
#else
	BRDC_sprintf(adcIniFile, _BRDC("adc_%d.ini"), model);
#endif

	ADC_set(vi, 0, AdcSrvName[model], adcIniFile, &adc_param);

	BRDC_printf(_BRDC("\nADC Config: %d Bits, FIFO is %d kBytes, %d channels\n"), adc_cfg.Bits, adc_cfg.FifoSize / 1024, adc_cfg.NumChans);
	BRDC_printf(_BRDC("            Min rate = %d kHz, Max rate = %d MHz\n"), adc_cfg.MinRate / 1000, adc_cfg.MaxRate / 1000000);
	BRDC_printf(_BRDC("            Input range = %.3f V\n"), adc_cfg.InpRange / 1000.);

	BRDC_printf(_BRDC("\nADC channel mask = %0X\n"), adc_param.chmask);
	BRDC_printf(_BRDC("ADC clocking: source = %d, value = %.2f MHz, rate = %.3f kHz\n\n"),
		adc_param.clkSrc, adc_param.clkValue / 1000000, adc_param.rate / 1000);

	itscope_param[idx].HOR_MAI_SAMPLER = adc_param.rate;
	for (int ii=0; ii < MAXCHAN; ii++) {
		itscope_param[idx].CH_EN[ii] = (adc_param.chmask>>ii)&1;
	}

	//====================================================================================
	// выделить буфер для сбора данных с АЦП
	scope_dev->buf_dscr.blkNum = 256;			// 256 блоков 
	scope_dev->buf_dscr.blkSize = 1024*1024;	// 1 MB
	scope_dev->buf_dscr.isCont = 0;				// однократный сбор
	scope_dev->buf_dscr.ppBlk = NULL;			// указатель на массив указателей на блоки памяти с сигналом
	S32 ret = ADC_allocbuf(vi);
	if (ret == -1) {
		BRDC_printf(_BRDC("IPC_virtAlloc() by allocating of buffer is error!!!\n"));
		return IVI_ERROR_RESOURCE_UNKNOWN;
	}
	BRDC_printf(_BRDC("Allocated memory for Stream: Number of blocks = %d, Block size = %d kBytes\n"),
		scope_dev->buf_dscr.blkNum, scope_dev->buf_dscr.blkSize / 1024);


	return VI_SUCCESS;
}

// открыть АЦП и получить информацию о нем 
// hDEV - (IN) дескриптор открытого устройства
// adcsrv - (IN) имя службы АЦП
// adcfg - (OUT) заполняемая информацией об АЦП структура
// возвращает дескриптор службы АЦП
BRD_Handle ADC_open(ViSession vi, BRD_Handle hDEV, BRDCHAR* adcsrv, BRD_AdcCfg* adcfg)
{
	S32		status;
	BRD_Handle hADC = -1;
	U32 mode = BRDcapt_EXCLUSIVE;
	hADC = BRD_capture(hDEV, 0, &mode, adcsrv, 10000);
	if (hADC > 0)
	{
		if (mode != BRDcapt_EXCLUSIVE)
			return -1;
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCFG, adcfg);
	}
	return hADC;
}

// установить рабочие параметры АЦП
// hADC - (IN) дескриптор службы АЦП
// adcsrv - (IN) имя службы АЦП
// inifile - (IN) файл инициализации c параметрами работы АЦП
// adcpar - (OUT) заполняемая рабочими параметрами АЦП структура
int ADC_set(ViSession vi, int iDev, BRDCHAR* adcsrv, BRDCHAR* inifile, ADC_PARAM* adcpar)
{
	S32		status;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;


	// задать параметры работы АЦП из секции ini-файла
	BRDCHAR iniFilePath[MAX_PATH];
	BRDCHAR iniSectionName[MAX_PATH];
	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath) / sizeof(BRDCHAR));
	BRDC_strcat(iniFilePath, _BRDC("//"));
	BRDC_strcat(iniFilePath, inifile);
#ifdef _WIN64
	BRDC_sprintf_s(iniSectionName, 256, _BRDC("device%d_%s"), iDev, adcsrv);
#else
	BRDC_sprintf(iniSectionName, _BRDC("device%d_%s"), iDev, adcsrv);
#endif

	//create adc.ini file 
	adc_create_ini_file(vi, adcsrv, iniFilePath, iniSectionName);

	BRD_IniFile ini_file;
	BRDC_strcpy(ini_file.fileName, iniFilePath);
	BRDC_strcpy(ini_file.sectionName, iniSectionName);
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_READINIFILE, &ini_file);

	// получить источник и значение тактовой частоты, а также частоту дискретизации можно отдельной функцией
	BRD_SyncMode sync_mode;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETSYNCMODE, &sync_mode);
	if (!BRD_errcmp(status, BRDerr_OK))
		return -1;
	// получить маску включенных каналов
	ULONG chan_mask = 0;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETCHANMASK, &chan_mask);
	if (!BRD_errcmp(status, BRDerr_OK))
		return -1;

	adcpar->chmask = chan_mask;
	adcpar->clkSrc = sync_mode.clkSrc;
	adcpar->clkValue = sync_mode.clkValue;
	adcpar->rate = sync_mode.rate;

	return 0;
}

// закрыть АЦП
int ADC_close(ViSession vi)
{
	S32		status;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;

	status = BRD_release(hADC, 0);

	return status;
}

#ifdef _WIN32
#define MAX_BLOCK_SIZE 1073741824		// максимальный размер блока = 1 Гбайт 
#else  // LINUX
#define MAX_BLOCK_SIZE 4194304			// максимальный размер блока = 4 Mбайтa 
#endif

// размещение буфера для получения данных с АЦП через Стрим
//	hADC - дескриптор службы АЦП (IN)
//	pSig - указатель на массив указателей (IN), каждый элемент массива является указателем на блок (OUT)
//	pbytesBufSize - общий размер данных (всех блоков составного буфера), которые должны быть выделены (IN/OUT - может меняться внутри функции)
//	pBlkNum - число блоков составного буфера (IN/OUT)
//	memType - тип памяти для данных (IN): 
//		0 - пользовательская память выделяется в драйвере (точнее, в DLL базового модуля)
//		1 - системная память выделяется драйвере 0-го кольца
//		2 - пользовательская память выделяется в приложении
S32 ADC_allocbuf(ViSession vi)
{
	S32		status;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;
	U64		bBufSize = scope_dev->buf_dscr.blkSize;
	U32		blkNum = scope_dev->buf_dscr.blkNum;

	// определяем число блоков составного буфера
	if (bBufSize > MAX_BLOCK_SIZE)
	{
		do {
			blkNum <<= 1;
			bBufSize >>= 1;
		} while (bBufSize > MAX_BLOCK_SIZE);
	}
	scope_dev->buf_dscr.blkSize = (U32)bBufSize;

	void** pBuffer = NULL;
	if (2 == scope_dev->buf_dscr.isCont)
	{
		pBuffer = malloc(sizeof(PVOID)*blkNum);;
		for (ULONG i = 0; i < blkNum; i++)
		{
			pBuffer[i] = IPC_virtAlloc((size_t)bBufSize);
			if (!pBuffer[i])
				return -1; // error
		}
	}
	scope_dev->buf_dscr.dir = BRDstrm_DIR_IN;
	scope_dev->buf_dscr.blkSize = (U32)bBufSize;
	scope_dev->buf_dscr.blkNum = blkNum;
	scope_dev->buf_dscr.ppBlk = malloc(sizeof(PVOID)*scope_dev->buf_dscr.blkNum);
	if (scope_dev->buf_dscr.isCont == 2)
	{
		for (ULONG i = 0; i < scope_dev->buf_dscr.blkNum; i++)
			scope_dev->buf_dscr.ppBlk[i] = pBuffer[i];
		free(pBuffer);
	}
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_ALLOC, &scope_dev->buf_dscr);
	if (BRD_errcmp(status, BRDerr_PARAMETER_CHANGED))
	{ // может быть выделено меньшее количество памяти
		BRDC_printf(_BRDC("Warning!!! BRDctrl_STREAM_CBUF_ALLOC: BRDerr_PARAMETER_CHANGED\n"));
		status = BRDerr_OK;
	}
	else
	{
		if (BRD_errcmp(status, BRDerr_OK))
		{
			//BRDC_printf(_BRDC("BRDctrl_STREAM_CBUF_ALLOC SUCCESS: status = 0x%08X\n"), status);
		}
		else
		{ // при выделении памяти произошла ошибка
			if (2 == scope_dev->buf_dscr.isCont)
			{
				for (ULONG i = 0; i < scope_dev->buf_dscr.blkNum; i++)
					IPC_virtFree(scope_dev->buf_dscr.ppBlk[i]);
			}
			free(scope_dev->buf_dscr.ppBlk);
			return status;
		}
	}

	return status;
}

// освобождение буфера стрима
S32 ADC_freebuf(ViSession vi)
{
	S32		status;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;

	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_FREE, NULL);
	if (scope_dev->buf_dscr.isCont == 2)
	{
		for (ULONG i = 0; i < scope_dev->buf_dscr.blkNum; i++)
			IPC_virtFree(scope_dev->buf_dscr.ppBlk[i]);
	}
	free(scope_dev->buf_dscr.ppBlk);
	return status;
}

// выполнить сбор данных в FIFO с ПДП-методом передачи в ПК
S32 ADC_read(ViSession vi)
{
	S32	status = 0;
	ULONG adc_status = 0;
	S32	wait_status = 0;
	ULONG Enable = 1;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;

	// установить источник для работы стрима
	ULONG tetrad;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETSRCSTREAM, &tetrad); // стрим будет работать с АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETSRC, &tetrad);

	// устанавливать флаг для формирования запроса ПДП надо после установки источника (тетрады) для работы стрима
	//	ULONG flag = BRDstrm_DRQ_ALMOST; // FIFO почти пустое
	//	ULONG flag = BRDstrm_DRQ_READY;
	ULONG flag = BRDstrm_DRQ_HALF; // рекомендуется флаг - FIFO наполовину заполнено
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_SETDRQ, &flag);

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFORESET, NULL); // сброс FIFO АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_RESETFIFO, NULL);

	BRDctrl_StreamCBufStart start_pars;
	start_pars.isCycle = 0; // без зацикливания 
							//start_pars.isCycle = 1; // с зацикливанием
	status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_START, &start_pars); // старт ПДП

	IPC_TIMEVAL start;
	IPC_TIMEVAL stop;
	IPC_getTime(&start);
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП

	ULONG msTimeout = 1000; // ждать окончания передачи данных до 1 сек. (внутри BRDctrl_STREAM_CBUF_WAITBUF)
	int i = 0;	// организуем цикл, чтобы иметь возможность прерваться
	for (i = 0; i < 20; i++)
	{
		wait_status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_WAITBUF, &msTimeout);
		if (BRD_errcmp(wait_status, BRDerr_OK))
			break;	// дождались окончания передачи данных
		if (IPC_kbhit())
		{
			int ch = IPC_getch();
			if (0x1B == ch)
				break;	// прерываем ожидание по Esc
		}
	}
	IPC_getTime(&stop);

	if (BRD_errcmp(wait_status, BRDerr_WAIT_TIMEOUT))
	{	// если вышли по тайм-ауту, то остановимся
		status = BRD_ctrl(hADC, 0, BRDctrl_STREAM_CBUF_STOP, NULL);
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFOSTATUS, &adc_status);
		BRDC_printf(_BRDC("BRDctrl_STREAM_CBUF_WAITBUF is TIME-OUT(%d sec.)\n    AdcFifoStatus = %08X"),
			msTimeout*(i + 1) / 1000, adc_status);
	}

	Enable = 0;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП

	if (BRD_errcmp(wait_status, BRDerr_OK))
	{
		double msTime = IPC_getDiffTime(&start, &stop);
		U64 bBufSize = scope_dev->buf_dscr.blkSize * scope_dev->buf_dscr.blkNum;
		printf("DAQ & Transfer by bus rate is %.2f Mbytes/sec\n", ((double)bBufSize / msTime) / 1000.);

		// проверить переполнение разрядной сетки АЦП
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ISBITSOVERFLOW, &adc_status);
		if (adc_status)
			printf("ADC Bits OVERFLOW %X  ", adc_status);

	}
	return wait_status;
}

S32	adc_create_ini_file(ViSession vi, BRDCHAR *adcsrv, BRDCHAR *iniFilePath, BRDCHAR *iniSectionName)
{
	S32 error = VI_SUCCESS;
	int ii;
	for (ii = 0; ii < sizeof(AdcSrvName); ii++) {
		if (adcsrv == AdcSrvName[ii]) break;
	}
	if (ii == sizeof(AdcSrvName)) return error = IVI_ERROR_RESOURCE_UNKNOWN;

	//create file
	FILE* file = BRDC_fopen(iniFilePath, _BRDC("wt+"));
	char iniString[8192];
	strcpy(iniString, iniStrTemplate[ii]);
	fwrite(iniString, 1, strlen(iniStrTemplate[ii]) + 1, file);
	BRDC_fclose(file);

	return error;
}

// разрешить/запретить канал
S32	adc_set_channel(ViSession vi, int chan, int stat)
{
	S32 error = VI_SUCCESS;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	U32	mask;
	BRD_ctrl(scope_dev->hADC, 0, BRDctrl_ADC_GETCHANMASK, &mask);
	mask &= ~(1 << (chan - 1)); // clear bit 
	mask |= (stat << (chan - 1)); // set/clear bit 
	BRD_ctrl(scope_dev->hADC, 0, BRDctrl_ADC_SETCHANMASK, &mask);
	return error;
}

S32	adc_get_channel(ViSession vi, int chan, int *stat)
{
	S32 error = VI_SUCCESS;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	U32	mask;
	BRD_ctrl(scope_dev->hADC, 0, BRDctrl_ADC_GETCHANMASK, &mask);
	mask >>= (chan - 1);
	*stat = mask & 1;
	return error;
}


//
// Установить параметры стартовой синхронизации
//
S32	adc_set_start(ViSession vi)
{
	S32 status;
	S32 error = VI_SUCCESS;
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);

	// команда BRDctrl_ADC_GETSTARTMODE может получать 2 разные структуры
	// для определения какую из них использует данная служба применяем трюк с массивом )))
	U08 start_struct[40]; // наибольшая из структур имеет размер 40 байт
	BRD_AdcStartMode* start = (BRD_AdcStartMode*)start_struct;
	memset(start_struct, 0x5A, 40);
	status = BRD_ctrl(scope_dev->hADC, 0, BRDctrl_ADC_GETSTARTMODE, &start_struct);
	if (BRD_errcmp(status, BRDerr_OK))
	{
		// стартовая схема на субмодуле (используется большая структура)
		BRD_AdcStartMode* start = (BRD_AdcStartMode*)start_struct;
		// службы: ADC212X200M, ADC10X2G, ADC214X200M, ADC28X1G, ADC214X400M, ADC210X1G,
		// FM814X125M, FM214X250M, FM412X500M, FM212X1G
		BRDC_printf(_BRDC("BRDctrl_ADC_GETSTARTMODE: start source = %d\n"), start->src);
	} else DisplayError(status, __FUNCTION__, _BRDC("BRDctrl_ADC_GETSTARTMODE"));



	return error;
}



// выполнить сбор данных в FIFO с программным методом передачи в ПК
S32 DaqIntoFifo(ViSession vi, PVOID pSig, ULONG bBufSize)
{
	S32		status;
	ULONG Status = 0;
	ULONG Enable = 1;
	BRD_DataBuf data_buf;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);
	BRD_Handle hADC = scope_dev->hADC;

	data_buf.pData = pSig;
	data_buf.size = bBufSize;
	if (bBufSize < (8192*4)) data_buf.size = 8192*4;

	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFORESET, NULL); // сброс FIFO АЦП
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFOSTATUS, &Status);
	if (scope_dev->MemAsFifo)
	{
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFORESET, NULL); // сброс FIFO SDRAM
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // разрешение записи в SDRAM
	}
	BRDC_printf(_BRDC("ADC Start...     \r"));
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // разрешение работы АЦП

	if (scope_dev->MemAsFifo)
	{
		// дожидаемся заполнения половины выходного FIFO (а не самой SDRAM)
		do {
			status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_FIFOSTATUS, &Status);
		} while (Status & 0x10);
	}
	else
	{
		// дожидаемся заполнения FIFO
		do {
			status = BRD_ctrl(hADC, 0, BRDctrl_ADC_FIFOSTATUS, &Status);
		} while (Status & 0x40);
	}

	if (scope_dev->MemAsFifo)
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_GETDATA, &data_buf);

	Enable = 0;
	status = BRD_ctrl(hADC, 0, BRDctrl_ADC_ENABLE, &Enable); // запрет работы АЦП
	BRDC_printf(_BRDC("ADC Stop         \r"));

	if (scope_dev->MemAsFifo)
		status = BRD_ctrl(hADC, 0, BRDctrl_SDRAM_ENABLE, &Enable); // запрет записи в SDRAM

	if (!scope_dev->MemAsFifo)
		status = BRD_ctrl(hADC, 0, BRDctrl_ADC_GETDATA, &data_buf);

	if (scope_dev->MemAsFifo)
		BRDC_printf(_BRDC("DAQ from SDRAM as FIFO is complete!!!\n"));
	else
		BRDC_printf(_BRDC("DAQ from FIFO is complete!!!\n"));
	return status;
}

static	short ttt[1024];
ViStatus	pe_read(ViSession vi, PVOID pSig, ULONG bBufSize)
{
	ViStatus	error = VI_SUCCESS;
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	SCOPE_DEVICES	*scope_dev = get_scopeDev(vi);

	checkErr( ADC_read(vi));
	U08		*dst = (U08*)pSig;
	short	*dst16 = (short*)pSig;
	U08	*src = (U08*)scope_dev->buf_dscr.ppBlk[0];
	U32	size = min(bBufSize, scope_dev->buf_dscr.blkSize);
	for(U32 ii=0;ii<bBufSize;ii++)
		dst[ii] = src[ii];

	for (U32 ii = 0; ii<1024; ii++)
		ttt[ii] = dst16[ii];
/*
	ViReal64	v_range = itscope_param[idx].CH_SCA[0]*10;
	ViReal64	v_amp = v_range/2;
	int			samples;

	if (itscope_param[idx].DAT_WID == 1) { // 8 bit
		samples = bBufSize;
		for (int ii = 0; ii < samples; ii++) 	// samples to volts
			dst[ii] = (U08)((v_amp*(ViReal64)dst[ii]) / 128.0);
	} else if (itscope_param[idx].DAT_WID == 2) { // 16 bit
		samples = bBufSize/2;
		for (int ii = 0; ii < samples; ii++) 	// samples to volts
			dst16[ii] = (short)((v_amp*(ViReal64)dst16[ii]) / 32768.0);
	}
*/		
Error:
	return error;

}

/*---------------------------- SCOPE CURRENT ATTRIBUTES ---------------------------------------- */


ViStatus _VI_FUNCC _viPrintf(ViSession vi, ViString writeFmt, ...)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;
	va_list	ap;
	va_start(ap, writeFmt);

	char	buf[256];
	vsprintf(buf, writeFmt, ap);

	va_end(ap);

	// -- buffer parsing -- 
	char cmdStr[256] = "";
	char valStr[256] = "";
	scpi_getcmdstr(cmdStr, buf);				// get command
	if (strlen(cmdStr) < strlen(buf)) scpi_getvalstr(valStr, buf + strlen(cmdStr) + 1);	// get value


	if (scpi_maskcmp(cmdStr, ":ACQ:MOD?") == 0) {
		for (int ii = 0; ii < sizeof(acq_type); ii++) {
			if (itscope_param[idx].ACQ_MOD == acq_type[ii].typeVal) {
				strcpy(retStr[idx], acq_type[ii].typeStr);
				break;
			}
		}
	}
	else if (scpi_maskcmp(cmdStr, ":ACQ:MOD") == 0) {
		if (strcmp(valStr, "SAMPLE") == 0) itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_NORMAL;
		else if (strcmp(valStr, "HIRES") == 0) itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_HI_RES;
		else if (strcmp(valStr, "PEAKDETECT") == 0) itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_PEAK_DETECT;
		else if (strcmp(valStr, "ENVELOPE") == 0) itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_ENVELOPE;
		else if (strcmp(valStr, "AVERAGE") == 0) itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_AVERAGE;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":HOR:DEL:TIM?") == 0) {
		sprintf(retStr[idx], "%le", itscope_param[idx].HOR_DEL_TIM);
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":HOR:DEL:TIM") == 0) {
		char   *stopstring;
		itscope_param[idx].HOR_DEL_TIM = strtod(valStr, &stopstring);
	} else if (scpi_maskcmp(cmdStr, ":HOR:RECORDL?") == 0) {
		sprintf(retStr[idx], "%I64d", itscope_param[idx].HOR_RECORDL);
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":HOR:RECORDL") == 0) {
		char   *stopstring;
		itscope_param[idx].HOR_RECORDL = strtoul(valStr, &stopstring, 10);
		if(itscope_param[idx].HOR_RECORDL < 4096) itscope_param[idx].HOR_RECORDL = 4096;
	} else if (scpi_maskcmp(cmdStr, ":HOR:SEC") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].HOR_SCA = strtod(valStr, &stopstring);
	} else if (scpi_maskcmp(cmdStr, ":HOR:SCA") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].HOR_SCA = strtod(valStr, &stopstring);
	} else if (scpi_maskcmp(cmdStr, ":SEL:CH*") == 0) {
		char   *stopstring;
		int i = strtoul(&cmdStr[7], &stopstring, 10); // channel number 1..8
		if (strcmp(valStr, "ON") == 0)			itscope_param[idx].CH_EN[i - 1] = 1;
		else if (strcmp(valStr, "OFF") == 0)	itscope_param[idx].CH_EN[i - 1] = 0;
		adc_set_channel(vi, i, itscope_param[idx].CH_EN[i - 1]);
	} else if (scpi_maskcmp(cmdStr, ":SEL:REF*") == 0) {
		char   *stopstring;
		int i = strtoul(&cmdStr[8], &stopstring, 10); // channel number 1..8
		if (strcmp(valStr, "ON") == 0)			itscope_param[idx].REF_EN[i - 1] = 1;
		else if (strcmp(valStr, "OFF") == 0)	itscope_param[idx].REF_EN[i - 1] = 0;
	} else if (scpi_maskcmp(cmdStr, ":SEL:MATH") == 0) {
		if (strcmp(valStr, "ON") == 0)			itscope_param[idx].MATH_EN = 1;
		else if (strcmp(valStr, "OFF") == 0)	itscope_param[idx].MATH_EN = 0;
	} else if (scpi_maskcmp(cmdStr, ":CH*:PRO:GAIN") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		itscope_param[idx].CH_PRO_GAIN[i - 1] = strtod(valStr, &stopstring);
	}
	else if (scpi_maskcmp(cmdStr, ":CH*:COUP") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		if (strcmp(valStr, "AC") == 0) itscope_param[idx].CH_COUP[i - 1] = ITSCOPE_VAL_AC;
		else if (strcmp(valStr, "DC") == 0) itscope_param[idx].CH_COUP[i - 1] = ITSCOPE_VAL_DC;
		else if (strcmp(valStr, "GND") == 0) itscope_param[idx].CH_COUP[i - 1] = ITSCOPE_VAL_GND;
		else if (strcmp(valStr, "DCREJ") == 0) itscope_param[idx].CH_COUP[i - 1] = ITSCOPE_VAL_DCREJ;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
		// only DC support
		itscope_param[idx].CH_COUP[i - 1] = ITSCOPE_VAL_DC;
	}
	else if (scpi_maskcmp(cmdStr, ":CH*:SCA") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		itscope_param[idx].CH_SCA[i - 1] = strtod(valStr, &stopstring);
	}
	else if (scpi_maskcmp(cmdStr, ":CH*:OFFS") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		itscope_param[idx].CH_OFFS[i - 1] = strtod(valStr, &stopstring);
	}
	else if (scpi_maskcmp(cmdStr, ":CH*:IMP") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		if (strcmp(valStr, "FIFTY") == 0) itscope_param[idx].CH_IMP[i - 1] = 50.0;
		else if (strcmp(valStr, "MEG") == 0) itscope_param[idx].CH_IMP[i - 1] = 1.0e+06;
	}
	else if (scpi_maskcmp(cmdStr, ":CH*:BAN") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		if (strcmp(valStr, "TWOFIFTY") == 0) itscope_param[idx].CH_BAN[i - 1] = 250.0e+06;
		else if (strcmp(valStr, "TWENTY") == 0) itscope_param[idx].CH_BAN[i - 1] = 20.0e+06;
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:TYP") == VI_SUCCESS) {
		if (strcmp(valStr, "EDGE") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_EDGE_TRIGGER;		// по фронту
		else if (strcmp(valStr, "RUNT") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_RUNT_TRIGGER; // по уровню
		else if (strcmp(valStr, "GLITCH") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_GLITCH_TRIGGER; // по короткому импульсу (сбой) 
		else if (strcmp(valStr, "TRANSITION") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_TRANSITION_TRIGGER; //по перепаду
		else if (strcmp(valStr, "STATE") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_STATE_TRIGGER; // по состоянию
		else if (strcmp(valStr, "PATTERN") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_PATTERN_TRIGGER; // по шаблону
		else if (strcmp(valStr, "SETHOLD") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_SETHOLD_TRIGGER; // по удержанию
		else if (strcmp(valStr, "VIDEO") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_TV_TRIGGER;
		else if (strcmp(valStr, "AC") == 0) itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_AC_LINE_TRIGGER; // линия ?
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:COUP") == VI_SUCCESS) {
		if (strcmp(valStr, "DC") == 0) itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_DC;
		else if (strcmp(valStr, "LFREJ") == 0) itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_LF_REJECT;
		else if (strcmp(valStr, "HFREJ") == 0) itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_HF_REJECT;
		else if (strcmp(valStr, "NOISEREJ") == 0) itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_NOISE_REJECT;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
		// only DC support
		itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_DC;
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:COUP?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_EDGE_COUP == ITSCOPE_VAL_DC) strcpy(retStr[idx], "DC");
		else if (itscope_param[idx].TRIG_A_EDGE_COUP == ITSCOPE_VAL_LF_REJECT) strcpy(retStr[idx], "LFREJ");
		else if (itscope_param[idx].TRIG_A_EDGE_COUP == ITSCOPE_VAL_HF_REJECT) strcpy(retStr[idx], "HFREJ");
		else if (itscope_param[idx].TRIG_A_EDGE_COUP == ITSCOPE_VAL_NOISE_REJECT) strcpy(retStr[idx], "NOISEREJ");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:HOLD:TIM") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].TRIG_A_HOLD_TIM = strtod(valStr, &stopstring);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:TYP?") == VI_SUCCESS) {
		for (int ii = 0; ii < sizeof(trig_type); ii++) {
			if (itscope_param[idx].TRIG_A_TYP == trig_type[ii].typeVal) {
				strcpy(retStr[idx], trig_type[ii].typeStr);
				break;
			}
		}
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:SOU") == VI_SUCCESS) {
		if (strncmp(valStr, "CH", 2) == 0) {
			char   *stopstring;
			int i = strtoul(&valStr[2], &stopstring, 10); // channel number 1..8
			itscope_param[idx].TRIG_A_EDGE_SOU = i;
		} else {
			itscope_param[idx].TRIG_A_EDGE_SOU = 0;
		}
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:LEV") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].TRIG_A_LEV = strtod(valStr, &stopstring);
		adc_set_start(vi);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:SLO") == VI_SUCCESS) {
		if (strcmp(valStr, "RISE") == 0) itscope_param[idx].TRIG_A_EDGE_SLO = ITSCOPE_VAL_POSITIVE;
		else if (strcmp(valStr, "FALL") == 0) itscope_param[idx].TRIG_A_EDGE_SLO = ITSCOPE_VAL_NEGATIVE;
		else if (strcmp(valStr, "EITHER") == 0) itscope_param[idx].TRIG_A_EDGE_SLO = ITSCOPE_VAL_EITHER;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":TRIG:B:LEV") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].TRIG_B_LEV = strtod(valStr, &stopstring);
	} else if (scpi_maskcmp(cmdStr, ":ACQ:STATE") == VI_SUCCESS) {
		if (strcmp(valStr, "RUN") == 0) itscope_param[idx].ACQ_STATE = ITSCOPE_VAL_ACQUISITION_RUN;
		else if (strcmp(valStr, "STOP") == 0) itscope_param[idx].ACQ_STATE = ITSCOPE_VAL_ACQUISITION_STOP;
		itscope_param[idx].ACQ_STATE = ITSCOPE_VAL_ACQUISITION_STOP; // Ready for read Waveform
	} else if (scpi_maskcmp(cmdStr, ":ACQ:STOPA") == VI_SUCCESS) {
		if (strcmp(valStr, "RUNST") == 0) itscope_param[idx].ACQ_STOPA = 0;
		else if (strcmp(valStr, "SEQ") == 0) itscope_param[idx].ACQ_STOPA = 1;
	} else if (scpi_maskcmp(cmdStr, ":DAT:SOU") == VI_SUCCESS) {
		if (strncmp(valStr, "CH", 2) == 0) {
			char   *stopstring;
			int i = strtoul(&valStr[2], &stopstring, 10); // channel number 1..8
			itscope_param[idx].DAT_SOU_CH = i;
		}
	} else if (scpi_maskcmp(cmdStr, ":DAT:WID") == VI_SUCCESS) {
		char   *stopstring;
		int wid = strtoul(valStr, &stopstring, 10);
		itscope_param[idx].DAT_WID = wid;
	} else if (scpi_maskcmp(cmdStr, ":DAT:STAR") == VI_SUCCESS) {
		char   *stopstring;
		int st = strtoul(valStr, &stopstring, 10);
		itscope_param[idx].DAT_STAR = st;
	} else if (scpi_maskcmp(cmdStr, ":DAT:STOP") == VI_SUCCESS) {
		char   *stopstring;
		int st = strtoul(valStr, &stopstring, 10);
		itscope_param[idx].DAT_STOP = st;
	} else if (scpi_maskcmp(cmdStr, ":CURV?") == VI_SUCCESS) {
		strcpy(retStr[idx], "#4");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
		retState[idx] = RET_STATE_CURVE1;
	} else if (scpi_maskcmp(cmdStr, "*RST") == VI_SUCCESS) {
		scpi_reset(vi);
	} else if (scpi_maskcmp(cmdStr, ":AUTOS EXEC;") == VI_SUCCESS) {
		scpi_autos_exec(vi);
	} else if (scpi_maskcmp(cmdStr, "*TRG") == VI_SUCCESS) {
		scpi_TRG(vi);
	} else if (scpi_maskcmp(cmdStr, ":SAV:WAVE") == VI_SUCCESS) {
		scpi_sav_wave(vi, valStr);
	} else if (scpi_maskcmp(cmdStr, "*CLS;") == VI_SUCCESS) {
		scpi_CLS(vi);
	} else if (scpi_maskcmp(cmdStr, "*ESE") == VI_SUCCESS) { //This command specifies the bits in the Event Status Enable Register
		char   *stopstring;
		ViUInt32 bit = strtoul(valStr, &stopstring, 10);
		itscope_param[idx].ESE |= (1 << (bit - 1));
	} else if (scpi_maskcmp(cmdStr, "*SRE") == VI_SUCCESS) { //This command specifies the bits in the Event Status Enable Register
		char   *stopstring;
		ViUInt32 bit = strtoul(valStr, &stopstring, 10);
		itscope_param[idx].SRE |= (1 << (bit - 1));
	} else if (scpi_maskcmp(cmdStr, "VERBOSE") == VI_SUCCESS) {
		if (strncmp(valStr, "ON", 2) == 0) itscope_param[idx].VERBOSE = 1;
		else if (strncmp(valStr, "OFF", 3) == 0) itscope_param[idx].VERBOSE = 0;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, "HEAD") == VI_SUCCESS) {
		if (strncmp(valStr, "ON", 2) == 0) itscope_param[idx].HEAD = 1;
		else if (strncmp(valStr, "OFF", 3) == 0) itscope_param[idx].HEAD = 0;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":MEASU:REFL:METH") == VI_SUCCESS) {
		if (strncmp(valStr, "PERC", 4) == 0) itscope_param[idx].measu_refl_meth = 1;
		else if (strncmp(valStr, "ABS", 3) == 0) itscope_param[idx].measu_refl_meth = 0;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":CURS:VBA:UNITS") == VI_SUCCESS) {
		if (strncmp(valStr, "SEC", 3) == 0) itscope_param[idx].measu_refl_meth = 0;
		else if (strncmp(valStr, "PERC", 4) == 0) itscope_param[idx].measu_refl_meth = 1;
		else if (strncmp(valStr, "DEG", 3) == 0) itscope_param[idx].measu_refl_meth = 2;
		else if (strncmp(valStr, "HER", 3) == 0) itscope_param[idx].measu_refl_meth = 3;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":HOR:DEL:STATE") == VI_SUCCESS) {
		if (strncmp(valStr, "ON", 2) == 0) itscope_param[idx].hor_del_state = 1;
		else if (strncmp(valStr, "OFF", 3) == 0) itscope_param[idx].hor_del_state = 0;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, ":DAT:ENC") == VI_SUCCESS) {
		if (strncmp(valStr, "ASC", 3) == 0) itscope_param[idx].dat_enc = 0;
		else if (strncmp(valStr, "FAS", 3) == 0) itscope_param[idx].dat_enc = 1;
		else if (strncmp(valStr, "RIB", 3) == 0) itscope_param[idx].dat_enc = 2;
		else if (strncmp(valStr, "RPB", 3) == 0) itscope_param[idx].dat_enc = 3;
		else if (strncmp(valStr, "SRI", 3) == 0) itscope_param[idx].dat_enc = 4;
		else if (strncmp(valStr, "SRP", 3) == 0) itscope_param[idx].dat_enc = 5;
		else if (strncmp(valStr, "FP", 2) == 0) itscope_param[idx].dat_enc = 6;
		else if (strncmp(valStr, "SFP", 3) == 0) itscope_param[idx].dat_enc = 7;
		else error = IVI_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN;
	} else if (scpi_maskcmp(cmdStr, "*DDT") == VI_SUCCESS) {
		scpi_DDT(vi, valStr);
	} else if (scpi_maskcmp(cmdStr, ":ACQ:NUME") == VI_SUCCESS) {
		if (strncmp(valStr, "INFI", 4) == 0) itscope_param[idx].ACQ_NUME = -1;
		else {
			char   *stopstring;
			itscope_param[idx].ACQ_NUME = strtoul(valStr, &stopstring, 10);
		}
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:PUL:WID:WID?") == VI_SUCCESS) {
		sprintf(retStr[idx], "%le", itscope_param[idx].TRIG_A_PUL_WID_WID );
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:PUL:WID:POL?") == VI_SUCCESS) {
		if(itscope_param[idx].TRIG_A_PUL_WID_POL == ITSCOPE_VAL_RUNT_NEGATIVE) strcpy(retStr[idx], "NEGATIVE");
		else if (itscope_param[idx].TRIG_A_PUL_WID_POL == ITSCOPE_VAL_RUNT_POSITIVE) strcpy(retStr[idx], "POSITIVE");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:PUL:WID:WHE?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_PUL_WID_WHE == ITSCOPE_VAL_GLITCH_GREATER_THAN) strcpy(retStr[idx], "MORETHAN");
		else if (itscope_param[idx].TRIG_A_PUL_WID_WHE == ITSCOPE_VAL_GLITCH_LESS_THAN) strcpy(retStr[idx], "LESSTHAN");
		else if (itscope_param[idx].TRIG_A_PUL_WID_WHE == ITSCOPE_VAL_GLITCH_WITHIN_5_PERCENT) strcpy(retStr[idx], "EQUAL");
		else if (itscope_param[idx].TRIG_A_PUL_WID_WHE == ITSCOPE_VAL_GLITCH_OUTSIDE_5_PERCENT) strcpy(retStr[idx], "UNEQUAL");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:PUL:RUNT:POL?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_PUL_RUNT_POL == ITSCOPE_VAL_RUNT_POSITIVE) strcpy(retStr[idx], "POSITIVE");
		else if (itscope_param[idx].TRIG_A_PUL_RUNT_POL == ITSCOPE_VAL_RUNT_NEGATIVE) strcpy(retStr[idx], "NEGATIVE");
		else if (itscope_param[idx].TRIG_A_PUL_RUNT_POL == ITSCOPE_VAL_RUNT_EITHER) strcpy(retStr[idx], "EITHER");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:VID:STAN") == VI_SUCCESS) {
		if (strcmp(valStr, "NTSC") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_NTSC;
		else if (strcmp(valStr, "PAL") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_PAL;
		else if (strcmp(valStr, "SECAM") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_SECAM;
		else if (strcmp(valStr, "BILEVELCUSTOM") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_BILEVELCUSTOM;
		else if (strcmp(valStr, "TRILEVELCUSTOM") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_TRILEVELCUSTOM;
		else if (strcmp(valStr, "HD480P60") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD480P60;
		else if (strcmp(valStr, "HD576P50") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD576P50;
		else if (strcmp(valStr, "HD720P30") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD720P30;
		else if (strcmp(valStr, "HD720P50") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD720P50;
		else if (strcmp(valStr, "HD720P60") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD720P60;
		else if (strcmp(valStr, "HD875I60") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD875I60;
		else if (strcmp(valStr, "HD1080P24") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080P24;
		else if (strcmp(valStr, "HD1080SF24") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080SF24;

		else if (strcmp(valStr, "HD1080I50") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080I50;
		else if (strcmp(valStr, "HD1080I60") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080I60;
		else if (strcmp(valStr, "HD1080P25") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080P25;
		else if (strcmp(valStr, "HD1080P30") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080P30;
		else if (strcmp(valStr, "HD1080P50") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080P50;
		else if (strcmp(valStr, "HD1080P60") == 0) itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_HD1080P60;
	} else if (scpi_maskcmp(cmdStr, "TRIGger:A:VIDeo:FIELD?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_VID_FIELD == ITSCOPE_VAL_TV_EVENT_FIELD1 ) strcpy(retStr[idx], "ODD");
		else if (itscope_param[idx].TRIG_A_VID_FIELD == ITSCOPE_VAL_TV_EVENT_FIELD2) strcpy(retStr[idx], "EVEN");
		else if (itscope_param[idx].TRIG_A_VID_FIELD == ITSCOPE_VAL_TV_EVENT_ANY_FIELD) strcpy(retStr[idx], "ALLFIELDS");
		else if (itscope_param[idx].TRIG_A_VID_FIELD == ITSCOPE_VAL_TV_EVENT_ANY_LINE) strcpy(retStr[idx], "ALLLINES");
		else if (itscope_param[idx].TRIG_A_VID_FIELD == ITSCOPE_VAL_TV_EVENT_LINE_NUMBER) strcpy(retStr[idx], "NUMERIC");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, "TRIGger:A:VIDeo:FIELD") == VI_SUCCESS) {
		if (strcmp(valStr, "ODD") == 0) itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_FIELD1;
		else if (strcmp(valStr, "EVEN") == 0) itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_FIELD2;
		else if (strcmp(valStr, "ALLFIELDS") == 0) itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_ANY_FIELD;
		else if (strcmp(valStr, "ALLLINES") == 0) itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_ANY_LINE;
		else if (strcmp(valStr, "NUMERIC") == 0) itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_LINE_NUMBER;
	} else if (scpi_maskcmp(cmdStr, "TRIG:A:VID:LINE") == VI_SUCCESS) {
		char   *stopstring;
		itscope_param[idx].TRIG_A_VID_LINE = strtoul(valStr, &stopstring, 10);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:VID:POL?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_VID_POL == ITSCOPE_VAL_TV_NEGATIVE) strcpy(retStr[idx], "NEGATIVE");
		else if (itscope_param[idx].TRIG_A_VID_POL == ITSCOPE_VAL_TV_POSITIVE) strcpy(retStr[idx], "POSITIVE");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:SLO?") == VI_SUCCESS) {
		if (itscope_param[idx].TRIG_A_EDGE_SLO == ITSCOPE_VAL_POSITIVE) strcpy(retStr[idx], "RISE");
		if (itscope_param[idx].TRIG_A_EDGE_SLO == ITSCOPE_VAL_NEGATIVE) strcpy(retStr[idx], "FALL");
		if (itscope_param[idx].TRIG_A_EDGE_SLO == ITSCOPE_VAL_EITHER) strcpy(retStr[idx], "EITHER");
		retCnt[idx] = (ViUInt32)strlen(retStr[idx]);
	}

	return error;
}

ViStatus _VI_FUNC  _viRead(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retcnt)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;
	if (retState[idx] == RET_STATE_NORMAL) {
		strcpy(buf, retStr[idx]);
		if (retcnt != VI_NULL) *retcnt = retCnt[idx];
	}
	else if (retState[idx] == RET_STATE_CURVE1) {
		strcpy(buf, "#4");	// size for bytes
		if (retcnt != VI_NULL) *retcnt = 2;
		retState[idx] = RET_STATE_CURVE2;
	}
	else if (retState[idx] == RET_STATE_CURVE2) {
		sprintf(buf, "%I64d", itscope_param[idx].HOR_RECORDL*itscope_param[idx].DAT_WID);
		if (retcnt != VI_NULL) *retcnt = 8;
		retState[idx] = RET_STATE_CURVE3;
	}
	else if (retState[idx] == RET_STATE_CURVE3) {
		// read scope data to buf;
		static ViInt8 cnt = 0;
		ViInt8 *p = (ViInt8*)buf;
/*		for (int i = 0; i < itscope_param[idx].HOR_RECORDL; i++) {
			p[i] = i + cnt;// / 10;
		}
		cnt++;
*/
		pe_read(vi, buf, (ViUInt32)itscope_param[idx].HOR_RECORDL*(ViUInt32)itscope_param[idx].DAT_WID);

		if (retcnt != VI_NULL) *retcnt = (ViUInt32)itscope_param[idx].HOR_RECORDL*(ViUInt32)itscope_param[idx].DAT_WID; // return bytes
		retState[idx] = RET_STATE_NORMAL;
	}

	return error;
}

// get scpi command word from buf 
ViStatus	scpi_getcmdstr(char *str, char *buf)
{
	ViStatus	error = VI_SUCCESS;

	for (int ii = 0; ii < (int)strlen(buf) + 1; ii++)
	{
		if (buf[ii] == ' ') {	// is space
			str[ii] = 0; break;
		}
		str[ii] = buf[ii];
	}

	return error;

}

// get scpi value word from buf 
ViStatus	scpi_getvalstr(char *str, char *buf)
{
	ViStatus	error = VI_SUCCESS;

	for (int ii = 0; ii < (int)strlen(buf) + 1; ii++)
	{
		if (!isalnum(buf[ii]) &&
			(buf[ii] != '.') &&
			(buf[ii] != '-'))
		{
			str[ii] = 0; break;
		}
		str[ii] = buf[ii];
	}

	return error;
}

static	int _get_arg_type(const char *writeFmt)
{
	int type = 0;
	int	i;
	for (i = 0; i < (int)strlen(writeFmt); i++) {
		if (writeFmt[i] == '%') {	// is types
			if (writeFmt[i+1] == 's')  type = 1;		// %s - is char string
			else if ((writeFmt[i+1] == 'l') && (writeFmt[i + 1] == 'd')) type = 2;	// %hd - is U32
			else if ((writeFmt[i+1] == 'h') && (writeFmt[i + 1] == 'd')) type = 2;	// %ld - is U32
			else if ((writeFmt[i+1] == 'l') && (writeFmt[i + 1] == 'e')) type = 4;	// %le - is double
			else if ((writeFmt[i+1] == 'l') && (writeFmt[i + 1] == 'f')) type = 4;	// %lf - is double
			break;
		}
	}
	return type;
}

ViStatus _VI_FUNCC _viQueryf(ViSession vi, ViString writeFmt, ViString readFmt, ...)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;
	va_list	ap1, ap2;
/*
	va_start(ap1, writeFmt);
	char	cmdStr[256];
	vsprintf(cmdStr, writeFmt, ap1);
	va_end(ap1);

	va_start(ap2, readFmt);
	*/

	char	cmdStr[256];

	va_start(ap1, writeFmt);
	int type = _get_arg_type(writeFmt);
	if (type == 0) {
		vsprintf(cmdStr, writeFmt, ap1);
	} else if(type==1) {	// %s
		va_arg(ap1, char*);
		vsprintf(cmdStr, writeFmt, ap1);
	}
	va_end(ap1);

	va_start(ap2, readFmt);
	if(type==1) va_arg(ap2, char*);
	else if (type == 2) va_arg(ap2, U32);
	else if (type == 4) va_arg(ap2, double);

	if (scpi_maskcmp(cmdStr, ":HOR:RECORDL?") == VI_SUCCESS) {
		ViUInt32 *val = va_arg(ap2, ViUInt32*);
		*val = (ViUInt32)itscope_param[idx].HOR_RECORDL;
	} else if (scpi_maskcmp(cmdStr, ":HOR:SEC?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].HOR_SCA;
	} else if (scpi_maskcmp(cmdStr, ":HOR:SCA?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].HOR_SCA;
	} else if (scpi_maskcmp(cmdStr, ":HOR:DEL:TIM?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].HOR_DEL_TIM;
	} else if (scpi_maskcmp(cmdStr, ":HOR:MAI:SAMPLER?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].HOR_MAI_SAMPLER;
	} else if (scpi_maskcmp(cmdStr, "*IDN?") == VI_SUCCESS) {
		ViChar *val = va_arg(ap2, ViChar*);
		scpi_getIDN(vi, val);
	} else if (scpi_maskcmp(cmdStr, "*OPC?") == VI_SUCCESS) {
		ViBoolean *val = va_arg(ap2, ViBoolean*);
		*val = (ViBoolean)itscope_param[idx].OPC_STATE;
	} else if (scpi_maskcmp(cmdStr, ":DAT:STAR?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = itscope_param[idx].DAT_STAR;
	} else if (scpi_maskcmp(cmdStr, ":DAT:STOP?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = itscope_param[idx].DAT_STOP;
	} else if (scpi_maskcmp(cmdStr, ":DAT:WID?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = itscope_param[idx].DAT_WID;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:YZERO?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].WFMPRE_YZERO;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:XZERO?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].WFMPRE_XZE;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:XINCR?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		itscope_param[idx].WFMPRE_XINCR = 2.0e-9;
		*val = itscope_param[idx].WFMPRE_XINCR;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:YOFF?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].WFMPRE_YOFF;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:YMULT?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		//itscope_param[idx].WFMPRE_YMULT = 1.0;
		*val = itscope_param[idx].WFMPRE_YMULT;
	} else if (scpi_maskcmp(cmdStr, ":WFMPRE:NR_PT?") == VI_SUCCESS) {
		ViUInt32 *val = va_arg(ap2, ViUInt32*);
		*val = (ViUInt32)itscope_param[idx].HOR_RECORDL;
		if (itscope_param[idx].HOR_RECORDL < 4096) itscope_param[idx].HOR_RECORDL = 4096;
	} else if (scpi_maskcmp(cmdStr, "*TST?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = scpi_self_test(vi);
	} else if (scpi_maskcmp(cmdStr, "*EVM?") == VI_SUCCESS) {
		ViUInt32 *err = va_arg(ap2, ViUInt32*);
		ViChar *str = va_arg(ap2, ViChar*);
		scpi_getEVM(vi, err, str);
	} else if (scpi_maskcmp(cmdStr, "*ESR?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = scpi_getESR(vi,val);
	} else if (scpi_maskcmp(cmdStr, ":ACQ:STATE?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = itscope_param[idx].ACQ_STATE;
	} else if (scpi_maskcmp(cmdStr, ":ACQ:NUME?") == VI_SUCCESS) {
		ViChar *str = va_arg(ap2, ViChar*);
		scpi_get_acq_nume(vi, str);
	} else if (scpi_maskcmp(cmdStr, ":CH*:OFFS?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].CH_OFFS[i - 1];
	} else if (scpi_maskcmp(cmdStr, ":SEL:CH*?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[7], &stopstring, 10); // channel number 1..8
		ViUInt32 *val = va_arg(ap2, ViUInt32*);
		*val = itscope_param[idx].CH_EN[i - 1];
	} else if (scpi_maskcmp(cmdStr, ":CH*:PRO:GAIN?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].CH_PRO_GAIN[i - 1];
	} else if (scpi_maskcmp(cmdStr, ":CH*:SCA?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].CH_SCA[i - 1];
	} else if (scpi_maskcmp(cmdStr, ":CH*:COUP?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViChar *val = va_arg(ap2, ViChar*);
		if (itscope_param[idx].CH_COUP[i - 1] == ITSCOPE_VAL_AC) strcpy(val, "AC");
		else if (itscope_param[idx].CH_COUP[i - 1] == ITSCOPE_VAL_DC) strcpy(val, "DC");
		else if (itscope_param[idx].CH_COUP[i - 1] == ITSCOPE_VAL_GND) strcpy(val, "GND");
		else if (itscope_param[idx].CH_COUP[i - 1] == ITSCOPE_VAL_DCREJ) strcpy(val, "DCREJ");
		else strcpy(val, "UNDEFINED");
	} else if (scpi_maskcmp(cmdStr, ":CH*:BAN?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].CH_BAN[i - 1];
	} else if (scpi_maskcmp(cmdStr, ":CH*:IMP?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[3], &stopstring, 10); // channel number 1..8
		ViChar *val = va_arg(ap2, ViChar*);
		if (itscope_param[idx].CH_IMP[i - 1] == 50.0) strcpy(val, "FIFTY");
		else if (itscope_param[idx].CH_IMP[i - 1] == 1.0e+06) strcpy(val, "MEG");
		else strcpy(val, "UNDEFINED");
	} else if (scpi_maskcmp(cmdStr, ":SEL:MATH?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[7], &stopstring, 10); // channel number 1..8
		ViUInt32 *val = va_arg(ap2, ViUInt32*);
		*val = itscope_param[idx].MATH_EN;
	} else if (scpi_maskcmp(cmdStr, ":SEL:REF*?") == VI_SUCCESS) {
		char   *stopstring;
		int i = strtoul(&cmdStr[8], &stopstring, 10); // ref channel number 1..8
		ViUInt32 *val = va_arg(ap2, ViUInt32*);
		*val = itscope_param[idx].REF_EN[i-1];
	} else if (scpi_maskcmp(cmdStr, ":EVM?") == VI_SUCCESS) {
		//char   *stopstring;
		ViUInt32 *val1 = va_arg(ap2, ViUInt32*);
		*val1 = -1;
		ViChar *val2 = va_arg(ap2, ViChar*);
		strcpy(val2,"##ERROR"); 
	} else if (scpi_maskcmp(cmdStr, ":TRIG:B:LEV?") == VI_SUCCESS) {
		ViReal64 *val = va_arg(ap2, ViReal64*);
		*val = itscope_param[idx].TRIG_B_LEV;
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:EDGE:SOU?") == VI_SUCCESS) {
		ViChar *val = va_arg(ap2, ViChar*);
		if (itscope_param[idx].TRIG_A_EDGE_SOU == 1) strcpy(val, "CH1");
		else if (itscope_param[idx].TRIG_A_EDGE_SOU == 2) strcpy(val, "CH2");
		else if (itscope_param[idx].TRIG_A_EDGE_SOU == 3) strcpy(val, "CH3");
		else if (itscope_param[idx].TRIG_A_EDGE_SOU == 4) strcpy(val, "CH4");
		else if (itscope_param[idx].TRIG_A_EDGE_SOU == 0) strcpy(val, "LINE");
	}
	else if (scpi_maskcmp(cmdStr, ":TRIG:A:VID:STAN?") == VI_SUCCESS) {
		ViChar *val = va_arg(ap2, ViChar*);
		if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_NTSC) strcpy(val, "NTSC");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_PAL) strcpy(val, "PAL");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_SECAM) strcpy(val, "SECAM");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_BILEVELCUSTOM) strcpy(val, "BILEVELCUSTOM");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_TRILEVELCUSTOM) strcpy(val, "TRILEVELCUSTOM");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD480P60) strcpy(val, "HD480P60");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD576P50) strcpy(val, "HD576P50");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD720P30) strcpy(val, "HD720P30");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD720P50) strcpy(val, "HD720P50");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD720P60) strcpy(val, "HD720P60");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD875I60) strcpy(val, "HD875I60");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080P24) strcpy(val, "HD1080P24");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080SF24) strcpy(val, "HD1080SF24");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080I50) strcpy(val, "HD1080I50");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080I60) strcpy(val, "HD1080I60");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080P25) strcpy(val, "HD1080P25");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080P30) strcpy(val, "HD1080P30");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080P50) strcpy(val, "HD1080P50");
		else if (itscope_param[idx].TRIG_A_VID_STAN == ITSCOPE_VAL_HD1080P60) strcpy(val, "HD1080P60");
	} else if (scpi_maskcmp(cmdStr, ":TRIG:A:VID:LINE?") == VI_SUCCESS) {
		ViInt32 *val = va_arg(ap2, ViInt32*);
		*val = itscope_param[idx].TRIG_A_VID_LINE;
	}


	va_end(ap2);

	return error;
}

ViStatus _VI_FUNC  _viBufWrite(ViSession vi, ViBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;

	return error;
}
ViStatus _VI_FUNC  _viBufRead(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;

	return error;
}

ViStatus _VI_FUNCC _viScanf(ViSession vi, ViString readFmt, ...)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	ViStatus	error = VI_SUCCESS;
	va_list	ap;
	va_start(ap, readFmt);
	char	buf[256];
	vsprintf(buf, readFmt, ap);
	va_end(ap);

	return error;
}

ViStatus _VI_FUNCC _viClose(ViSession vi)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	ViStatus	error = VI_SUCCESS;
	error = BRD_close(handle);
	return error;
}

ViStatus _VI_FUNC  _viSetAttribute(ViObject vi, ViAttr attrName, ViAttrState attrValue)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	ViStatus	error = VI_SUCCESS;
	return error;
}

ViStatus _VI_FUNC  _viGetAttribute(ViObject vi, ViAttr attrName, void _VI_PTR attrValue)
{
	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;
	ViStatus	error = VI_SUCCESS;
	return error;
}

ViStatus	scpi_maskcmp(const char *str, const char *mask)
{
	ViStatus	error = VI_SUCCESS;
	int			ii;

	size_t len = max(strlen(str), strlen(mask));

	for (ii = 0; ii < (int)len; ii++) {
		if ((str[ii] != mask[ii]) && (mask[ii] != '*')) break;
	}
	if (ii != len)  error = VI_ERROR_NSUP_LINE;

	return error;
}


ViStatus	parse_bus_dev(ViChar *resourceName, ViUInt32 *nBus, ViUInt32 *nDev)
{
	ViStatus    error = VI_SUCCESS;

	// first number - BUS
	int ii;
	for (ii = 0; ii < (int)strlen(resourceName); ii++)
	{
		if (isdigit(resourceName[ii])) {
			char *stopstring;
			*nBus = strtoul(&resourceName[ii], &stopstring, 10);
			break;
		}
	}
	ii += 3;
	// next number - DEV
	for (; ii < (int)strlen(resourceName); ii++)
	{
		if (isdigit(resourceName[ii])) {
			char *stopstring;
			*nDev = strtoul(&resourceName[ii], &stopstring, 10);
			break;
		}
	}

	return error;
}

ViStatus	scpi_hwInit(ViSession vi, ViChar *resourceName)
{
	ViStatus    error = VI_SUCCESS;

	// Search and Init Device (BUS:DEV)
	ViUInt32 busnum, devnum;
	parse_bus_dev(resourceName, &busnum, &devnum);
	// Init BRD library
	BRDCHAR	ini_str[256] = _BRDC("[BASE ENUM]\nbambpex\nb6678pex\n[SUB ENUM]\nmfm412x500m=0x1030");
	S32 bNum;
	S32 err = BRD_initEx(BRDinit_STRING | BRDinit_AUTOINIT, ini_str, NULL, &bNum);

	BRD_LidList lidList;
	lidList.item = MAXDEV;
	lidList.pLID = (U32*)malloc(MAXDEV * sizeof(U32));
	S32 status = BRD_lidList(lidList.pLID, lidList.item, &lidList.itemReal);

	// get scope information
	BRD_Info Info;
	S32 lid;
	int ii;
	for (ii = 0; ii < (int)lidList.itemReal; ii++)
	{
		Info.size = sizeof(BRD_Info);
		BRD_getInfo(lidList.pLID[ii], &Info);
		if ((Info.bus == busnum) && (Info.dev == devnum))
		{
			break;
		}
	}
	if (ii == lidList.itemReal) {
		error = VI_ERROR_RSRC_NFOUND;
		free(lidList.pLID);
		return error;
	}
	lid = lidList.pLID[ii];
	free(lidList.pLID);

	// open device (lid - sequence 1..N)
	scopeDev[lid - 1].vi = vi;
	scopeDev[lid - 1].brdHandle = BRD_open(lid, 0, NULL);
	scopeDev[lid - 1].iDev = lid - 1;

	//====================================================================================
	// получить информацию об открытом устройстве
	DEV_INFO dev_info;
	dev_info.size = sizeof(DEV_INFO);
	DEV_info(vi,scopeDev[lid - 1].brdHandle, lid - 1, &dev_info);

	return error;
}

ViStatus	scpi_reset(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	BRD_reset(handle, 0);
	return error;
}

ViStatus	scpi_get_acq_nume(ViSession vi, ViChar *str)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	return error;
}

ViStatus	scpi_CLS(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	return error;
}


ViStatus	scpi_autos_exec(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	return error;
}

ViStatus	scpi_self_test(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	return error;
}

ViStatus	scpi_TRG(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	return error;
}

ViStatus	scpi_getESR(ViSession vi, ViUInt32 *val)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	*val = itscope_param[idx].ESR;

	return error;
}

ViStatus	scpi_sav_wave(ViSession vi, ViChar *str)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	return error;
}

ViStatus	scpi_DDT(ViSession vi, ViChar *str)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	return error;
}


ViStatus	scpi_getEVM(ViSession vi, ViUInt32 *err, ViChar *str)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	*err = itscope_param[idx].ESR;
	strcpy(str, itscope_param[idx].EVM);

	return error;
}

ViStatus	scpi_getIDN(ViSession vi, ViChar *idnStr)
{
	ViStatus    error = VI_SUCCESS;
	int			ii;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	U32	iDev = get_brd_idev(vi);
	int	idx = handle - 1;

	//====================================================================================
	// получить информацию об открытом устройстве
	DEV_INFO dev_info;
	dev_info.size = sizeof(DEV_INFO);
	DEV_info(vi,handle, iDev, &dev_info);

	for (ii = 0; ii < sizeof(devnames); ii++) {
		if ((devnames[ii].baseId == dev_info.devID) &&
			(devnames[ii].subId == dev_info.subType)) {
			strcpy(idnStr, devnames[ii].Name);
			break;
		}
	}
	if (ii == sizeof(devnames)) {
		//strcpy(idnString, MODEL_NAME);
	}
	else {
		strcat(idnStr, ",");
		char	devVer[128];
		char	devPid[128];
		char	fpgaName[128];
		char	fpgaVer[128];

		sprintf(devVer, "DV:%d.%d,", dev_info.rev >> 4, dev_info.rev & 0xf);
		sprintf(devPid, "S/N:%d,", dev_info.pid);
		strcpy(fpgaName, (char*)dev_info.pldName);
		sprintf(fpgaVer, "FV:%d.%d,", dev_info.pldVer >> 8, dev_info.pldVer & 0xff);
#ifdef _IVI_mswin64_
		BRDC_bcstombs(fpgaName, dev_info.pldName, 128);
#endif
		strcat(idnStr, devVer);
		strcat(idnStr, devPid);
		//strcat(idnStr, fpgaName);
		strcat(idnStr, ",");
		strcat(idnStr, fpgaVer);
	}

	return error;
}


ViStatus	set_param_default(ViSession vi, ViInt32   model)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;
	int	idx = handle - 1;

	itscope_param[idx].ACQ_MOD = ITSCOPE_VAL_NORMAL;
	itscope_param[idx].ACQ_STATE = ITSCOPE_VAL_ACQUISITION_STOP;
	itscope_param[idx].ACQ_STOPA = 1;
	itscope_param[idx].ACQ_NUME = -1;
	itscope_param[idx].OPC_STATE = 0;
	itscope_param[idx].HOR_SCA = (8192/10)*2.0e-9; // на одно деление осциллографа (10 делений)
	itscope_param[idx].HOR_DEL_TIM = 0.0;
	itscope_param[idx].HOR_RECORDL = 8192;
	itscope_param[idx].HOR_MAI_SAMPLER = 500.0e+6;
	
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_EN[ii] = 0;
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_PRO_GAIN[ii] = 1.0;
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_COUP[ii] = ITSCOPE_VAL_DC;
	if (model == ITSCOPE_VAL_PE510K1) {
		for (int ii = 0; ii < MAXCHAN; ii++)
			itscope_param[idx].CH_SCA[ii] = 2.7/10.0; // на одно деление осциллографа (считаем 10 вертикальных делений
	} else {
		for (int ii = 0; ii < MAXCHAN; ii++) 
			itscope_param[idx].CH_SCA[ii] = 1.0/10.0; // на одно деление осциллографа
	}
	
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_OFFS[ii] = 0.0;
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_IMP[ii] = 50.0;
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].CH_BAN[ii] = 500.0e+06;

	itscope_param[idx].MATH_EN = 0;
	for (int ii = 0; ii<MAXCHAN; ii++) itscope_param[idx].REF_EN[ii] = 0;

	itscope_param[idx].TRIG_A_TYP = ITSCOPE_VAL_EDGE_TRIGGER;
	itscope_param[idx].TRIG_A_EDGE_COUP = ITSCOPE_VAL_DC;
	itscope_param[idx].TRIG_A_HOLD_TIM = 0.0;
	itscope_param[idx].TRIG_A_EDGE_SOU = 1;
	itscope_param[idx].TRIG_A_LEV = 0.0;
	itscope_param[idx].TRIG_A_EDGE_SLO = ITSCOPE_VAL_POSITIVE;
	itscope_param[idx].TRIG_A_PUL_WID_WID = 4.0e-09;
	itscope_param[idx].TRIG_A_PUL_WID_POL = ITSCOPE_VAL_GLITCH_POSITIVE; 
	itscope_param[idx].TRIG_A_PUL_WID_WHE = ITSCOPE_VAL_GLITCH_WITHIN_5_PERCENT;
	itscope_param[idx].TRIG_A_PUL_RUNT_POL = ITSCOPE_VAL_RUNT_POSITIVE;

	itscope_param[idx].TRIG_A_VID_STAN = ITSCOPE_VAL_NTSC;
	itscope_param[idx].TRIG_A_VID_FIELD = ITSCOPE_VAL_TV_EVENT_FIELD1;
	itscope_param[idx].TRIG_A_VID_LINE = 1;
	itscope_param[idx].TRIG_A_VID_POL = ITSCOPE_VAL_TV_POSITIVE;

	itscope_param[idx].TRIG_B_LEV = 0.0;
	
	itscope_param[idx].DAT_SOU_CH = 0;
	itscope_param[idx].DAT_WID = 2;
	itscope_param[idx].DAT_STAR = 1;
	itscope_param[idx].DAT_STOP = 8192;
	
	itscope_param[idx].WFMPRE_YZERO = 0.0;
	itscope_param[idx].WFMPRE_XZE = 0.0;
	itscope_param[idx].WFMPRE_XINCR = 1;
	itscope_param[idx].WFMPRE_YOFF = 0.0;
	itscope_param[idx].WFMPRE_YMULT = 1.0;
	
	itscope_param[idx].ESR = 0;
	strcpy(itscope_param[idx].EVM, (ViChar*)"");

	itscope_param[idx].ESE = 0;
	itscope_param[idx].SRE = 0;
	itscope_param[idx].VERBOSE = 1;
	itscope_param[idx].HEAD = 1;
	itscope_param[idx].measu_refl_meth = 0;
	itscope_param[idx].hor_del_state = 0;
	itscope_param[idx].dat_enc = 0;

	return error;
}

ViStatus	pe_open_default(ViSession vi, ViInt32   model)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	checkErr(set_param_default(vi, model));
	checkErr(adc_open_default(vi, model));

Error:
	return error;
}

ViStatus	pe_close(ViSession vi)
{
	ViStatus    error = VI_SUCCESS;

	BRD_Handle handle = get_brd_handle(vi);
	if (handle <= 0) return IVI_ERROR_RESOURCE_UNKNOWN;

	checkErr(ADC_freebuf(vi));
	checkErr(ADC_close(vi));
	checkErr(BRD_close(handle));

Error:
	return error;
}

/*****************************************************************************
*------------------- End PE Instrument Driver Source Code -------------------*
*****************************************************************************/
