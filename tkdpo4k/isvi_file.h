#ifndef _ISVI_FILE_H
#define	_ISVI_FILE_H

#include <utypes.h>


typedef struct _SCOPE_PARAM {
	char	DevName[64];			// device name
	U32     NumSamples;				// number of samples
	U32		MaxNumChan;				// maximum num channels
	U32		NumChan;				// num channels
	U32		SampleRate;				// sample rate 
	U16		AdcBitRange;			// adc resolution (bits)
	double	InpRange;				// input range (volts)
	U08		SampleSize;				// 1 sample size (bytes)
	U08		SamplePerBytes;			// samples in 1 bytes
	U08		isComplexSig;			// 1 - complex signal, 0 - single
	U08		rsv[1];					// alignment to 8 bytes
} SCOPE_PARAM, *PSCOPE_PARAM;

void	DoubleToShort(double *in, short *out, SCOPE_PARAM *scope_param);
void	WriteFlagSinc(int flg, int isNewParam);
int		ReadFlagSinc(void);
void    WriteDataFile(short *pBuf[], U32 bufnum, U32 sizew);
void    PrepareFileHeader(SCOPE_PARAM *scope_param, int *pChanNums, double *pGains, double *pFreqOffsets);


#endif	_ISVI_FILE_H

