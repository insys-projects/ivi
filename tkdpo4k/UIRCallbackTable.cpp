//------------------------------------------------------------------------------
//     LabWindows/CVI User Interface Resource (UIR) Callback Table File         
//         Copyright (c) National Instruments. All Rights Reserved              
//                                                                              
// The LabWindows/CVI Add-In will maintain this file as you make changes to     
// your application's UIR files. This file is equivalent to the one generated   
// by LabWindows/CVI when you select External Compiler Support->Generate UI     
// Callbacks Object File.  Do not modify this file in any way.                  
//------------------------------------------------------------------------------

#define _CVI			
#define _CUNTINUOUS		

#ifdef _CVI // for LabWindows/CVI

//#define _NI_mswin32_
#define _NI_mswin64_	

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#pragma pack(4)

typedef struct
{
    char* name;
    void* address;
    unsigned long isFunction : 1;
    unsigned long reserved : 31;
} ExeSymbol;

int __cdecl QuitCallback(int panel, int control, int event, void* callbackData, int eventData1, int eventData2);
int __cdecl PlotWaveTime(int panel, int control, int event, void* callbackData, int eventData1, int eventData2);

int __UICallbackSymbolCount = 2;

ExeSymbol __UICallbackSymbols[2] =
{
    {"_QuitCallback", QuitCallback, 1, 0},
    {"_PlotWaveTime", PlotWaveTime, 1, 0}
};

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
