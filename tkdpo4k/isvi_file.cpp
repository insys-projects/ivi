//********************************************************
//
// Write file for ISVI functions
//
// (C) InSys Jan, 2017
//

#include <stdio.h>
#include <stdlib.h>

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <utypes.h>
#include <gipcy.h>

#include "isvi_file.h"

int     g_fileHeaderLen;
char    g_fileHeader[1024];

int 	ChanNums[64];			// 
double	Gains[64];				// 
double	FreqOffsets[64];		// 

//=********************** double2short *******************
//=********************************************************
void DoubleToShort(double *in, short *out, SCOPE_PARAM *scope_param)
{
	for (int i = 0; i < (int)scope_param->NumSamples; i++) {
		out[i] = (short) ((in[i] * (1 << scope_param->AdcBitRange)) / scope_param->InpRange);
	}

}

//=********************** WriteFlagSinc *******************
//=********************************************************
void WriteFlagSinc(int flg, int isNewParam)
{
	int fs = -1;
	long val[2];
	while(fs==-1)
		fs = BRDC_sopen(_BRDC("data.flg"), _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _SH_DENYNO, _S_IWRITE);

	val[0] = flg;
	val[1] = isNewParam;
	int ret = _write(fs, val, 8);
	_close(fs);
	_flushall();
}

//=*********************** ReadFlagSinc *******************
//=********************************************************
int ReadFlagSinc(void)
{
	int fs = -1;
	int flg;

	while (fs == -1)
		fs = BRDC_sopen(_BRDC("data.flg"), _O_RDWR | _O_BINARY , _SH_DENYNO, _S_IREAD);

	int ret = _read(fs, &flg, 4);
	_close(fs);

	return flg;
}

//=************************** WriteDataFile *******************
//=********************************************************
void     WriteDataFile(short *pBuf[], U32 bufnum, U32 sizew)
{
	// sizew - number of U32
	int fl = -1;
	while(fl==-1)
		fl = BRDC_sopen(_BRDC("data.bin"), _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _SH_DENYNO, _S_IWRITE);

	U32     ii;

	for (ii = 0; ii<bufnum; ii++)
		_write(fl, pBuf[ii], sizew);

	_write(fl, g_fileHeader, g_fileHeaderLen);

	_close(fl);

	_flushall();

}

//********************************************************
//=*********************** PrepareFileHeader ***********
//=*****************************************************
void    PrepareFileHeader(SCOPE_PARAM *scope_param, int *pChanNums, double *pGains, double *pFreqOffsets)
{
	int		ii;
	char	linChans[128], linTmp[10], linGains[128], linOffset[128], linBias[128];
	char    g_str[256];

	//
	// Make string sequence
	//
	linChans[0] = '\0';
	linGains[0] = '\0';
	linOffset[0] = '\0';
	linBias[0] = '\0';

	for (ii = 0; ii<(int)scope_param->NumChan; ii++)
	{
		sprintf(linTmp, "%d,", pChanNums[ii]);
		strcat(linChans, linTmp);
		sprintf(linTmp, "%3.1f,", (float)pGains[ii]);
		strcat(linGains, linTmp);
		sprintf(linTmp, "%3.1f,", (float)pFreqOffsets[ii]);
		strcat(linOffset, linTmp);
		sprintf(linTmp, "%3.1f,", (float)0.0);
		strcat(linBias, linTmp);
	}

	//
	// File Header
	//
	g_fileHeader[0] = '\0';
	sprintf(g_str, "\r\nDEVICE_NAME_________ %s", scope_param->DevName);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nNUMBER_OF_CHANNELS__ %d", scope_param->NumChan);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nNUMBERS_OF_CHANNELS_ %s", linChans);
	strcat(g_fileHeader, g_str);

	// sprintf( g_str, "\r\nNUMBER_OF_SAMPLES___ %d", scope_param->NumSamples);
	sprintf(g_str, "\r\nNUMBER_OF_SAMPLES___ %d", (scope_param->NumSamples * 4) / (scope_param->NumChan*scope_param->SampleSize));
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nSAMPLING_RATE_______ %d", scope_param->SampleRate);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nBYTES_PER_SAMPLES___ %d", scope_param->SampleSize);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nSAMPLES_PER_BYTES___ %d", scope_param->SamplePerBytes);
	strcat(g_fileHeader, g_str);

	if (scope_param->isComplexSig == 0) 		strcat(g_str, "\r\nIS_COMPLEX_SIGNAL?__ NO");
	else if (scope_param->isComplexSig == 1) 	strcat(g_str, "\r\nIS_COMPLEX_SIGNAL?__ YES");
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nSHIFT_FREQUENCY_____ %s", linOffset);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nGAINS_______________ %s", linGains);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nVOLTAGE_OFFSETS_____ %s", linBias);
	//    sprintf( g_str, "\r\nVOLTAGE_OFFSETS_____ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0," );
	//
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nVOLTAGE_RANGE_______ %f", (double)scope_param->InpRange);
	strcat(g_fileHeader, g_str);

	sprintf(g_str, "\r\nBIT_RANGE___________ %d", scope_param->AdcBitRange);
	strcat(g_fileHeader, g_str);

	strcat(g_fileHeader, "\r\nPRETRIGGER_SAMPLE___ 0\r\n");

	g_fileHeaderLen = (int)strlen(g_fileHeader) + 1;

}


//
// End of file
//
