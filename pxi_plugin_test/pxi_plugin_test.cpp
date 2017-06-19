#include <stdio.h>

#include "visa.h"

//#include "isvisappi.h"

static ViSession defaultRM, vi;

int main()
{
	//open resource manager

	ViStatus status = viOpenDefaultRM(&defaultRM);
	if (status != VI_SUCCESS)
		return 0; // cannot open resource manager

	ViFindList find_list;
	ViUInt32 retCnt = 0;
	char instrDesc[100] = { 0 };

	status = viFindRsrc(defaultRM, "?*INSTR",
		&find_list, &retCnt, instrDesc);
	if (status == VI_SUCCESS)
		printf("%s\n", instrDesc);

	for (int i = 0; i < retCnt; i++) {
		status = viFindNext(find_list, instrDesc);
		if(status== VI_SUCCESS)
			printf("%s\n", instrDesc);
	}

	//status = viOpen(defaultRM, instrDesc, VI_NULL, VI_NULL, &vi);
	status = viOpen(defaultRM, "PXI15::15::INSTR", VI_NULL, VI_NULL, &vi);
	char dName[256];
	viGetAttribute(vi, VI_ATTR_MODEL_NAME, dName);
	printf("%s\n", dName);
	
	status = viOpen(defaultRM, "PXI15::12::INSTR", VI_NULL, VI_NULL, &vi);
	viGetAttribute(vi, VI_ATTR_MODEL_NAME, dName);
	printf("%s\n", dName);

	status = viOpen(defaultRM, "PXI33::0::INSTR", VI_NULL, VI_NULL, &vi);
	viGetAttribute(vi, VI_ATTR_MODEL_NAME, dName);
	printf("%s\n", dName);

	status = viOpen(defaultRM, "PXI25::0::INSTR", VI_NULL, VI_NULL, &vi);
	viGetAttribute(vi, VI_ATTR_MODEL_NAME, dName);
	printf("%s\n", dName);

	if (status != VI_SUCCESS) {
		printf("Can not find device!\n");
		viClose(vi);
		viClose(defaultRM);
		return -1;
	}
	printf("1111\n");

	viClose(defaultRM);
/*
	ViStatus stat = PpiInitializePlugin();
	printf("PXI Plug-in initialize\n");*/
	return 0;
}
