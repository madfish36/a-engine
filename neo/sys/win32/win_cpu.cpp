/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma hdrstop
#include "../../idlib/precompiled.h"

#include "win_local.h"
#include <intrin.h>

/*
================
	FXSAVE AREA
================
*/
static alignas(16) unsigned char fxState[512], * fxStatePtr = fxState;
unsigned int  FCW = static_cast<unsigned int>(fxState[0]);
unsigned int  FSW = static_cast<unsigned int>(fxState[2]);
unsigned char FTW = static_cast<unsigned char>(fxState[4]);
unsigned int  FOP = static_cast<unsigned int>(fxState[6]);
unsigned long FIP = static_cast<unsigned long>(fxState[8]);
unsigned int  FCS = static_cast<unsigned int>(fxState[12]);
unsigned long FDP = static_cast<unsigned long>(fxState[16]);
unsigned int  FDS = static_cast<unsigned int>(fxState[20]);
unsigned long MXCSR = static_cast<unsigned long>(fxState[24]);
unsigned long MXCSR_MASK = static_cast<unsigned long>(fxState[28]);
unsigned char ST[8][10] = {
	{ fxState[32],fxState[33],fxState[34],fxState[35],fxState[36],fxState[37],fxState[38],fxState[39],fxState[40],fxState[41] },
	{ fxState[48],fxState[49],fxState[50],fxState[51],fxState[52],fxState[53],fxState[54],fxState[55],fxState[56],fxState[57] },
	{ fxState[64],fxState[65],fxState[66],fxState[67],fxState[68],fxState[69],fxState[70],fxState[71],fxState[72],fxState[73] },
	{ fxState[96],fxState[97],fxState[98],fxState[99],fxState[100],fxState[101],fxState[102],fxState[103],fxState[104],fxState[105] },
	{ fxState[112],fxState[113],fxState[114],fxState[115],fxState[116],fxState[117],fxState[118],fxState[119],fxState[120],fxState[121] },
	{ fxState[128],fxState[129],fxState[130],fxState[131],fxState[132],fxState[133],fxState[134],fxState[135],fxState[136],fxState[137] },
	{ fxState[144],fxState[145],fxState[146],fxState[147],fxState[148],fxState[149],fxState[150],fxState[151],fxState[152],fxState[153] }
};
unsigned long long XMM[16][2] = {
	{fxState[160],fxState[168]},
	{fxState[176],fxState[184]},
	{fxState[192],fxState[200]},
	{fxState[208],fxState[216]},
	{fxState[224],fxState[232]},
	{fxState[240],fxState[248]},
	{fxState[256],fxState[264]},
	{fxState[272],fxState[280]},
	{fxState[288],fxState[296]},
	{fxState[304],fxState[312]},
	{fxState[320],fxState[328]},
	{fxState[336],fxState[344]},
	{fxState[352],fxState[360]},
	{fxState[368],fxState[376]},
	{fxState[384],fxState[392]},
	{fxState[400],fxState[408]}
};

/*
==============================================================

	Clock ticks

==============================================================
*/

/*
================
Sys_GetClockTicks
================
*/
double Sys_GetClockTicks() {
	unsigned int aux;
	return __rdtscp(&aux);
}

/*
================
Sys_ClockTicksPerSecond
================
*/
double Sys_ClockTicksPerSecond() {
	static double ticks = 0;
	if ( !ticks ) {
		unsigned int aux;
		__int64 cyclesStart = 0, cyclesStop = 0;
	
		cyclesStart = __rdtscp(&aux);

		Sleep(100);

		cyclesStop = __rdtscp(&aux);

		ticks =  (cyclesStop - cyclesStart)*10;

		//LARGE_INTEGER li;
		//QueryPerformanceFrequency( &li );
		//ticks = li.QuadPart;
	}
	return ticks;
}


/*
==============================================================

	CPU

==============================================================
*/

/*
================
HasCPUID
================
*/
static bool HasCPUID() {
	return true;
}

#define _REG_EAX		0
#define _REG_EBX		1
#define _REG_ECX		2
#define _REG_EDX		3

/*
================
CPUID
================
*/
static void CPUID( int func,  int regs[4] ) {
	__cpuid(regs, func);
}


/*
================
IsAMD
================
*/
static bool IsAMD() {
	char pstring[16];
	char processorString[13];

	// get name of processor
	CPUID( 0, (  int * ) pstring );
	processorString[0] = pstring[4];
	processorString[1] = pstring[5];
	processorString[2] = pstring[6];
	processorString[3] = pstring[7];
	processorString[4] = pstring[12];
	processorString[5] = pstring[13];
	processorString[6] = pstring[14];
	processorString[7] = pstring[15];
	processorString[8] = pstring[8];
	processorString[9] = pstring[9];
	processorString[10] = pstring[10];
	processorString[11] = pstring[11];
	processorString[12] = 0;

	if ( strcmp( processorString, "AuthenticAMD" ) == 0 ) {
		return true;
	}
	return false;
}

/*
================
HasCMOV
================
*/
static bool HasCMOV() {
	int regs[4];

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 15 of EDX denotes CMOV existence
	if ( regs[_REG_EDX] & ( 1 << 15 ) ) {
		return true;
	}
	return false;
}

/*
================
Has3DNow
================
*/
static bool Has3DNow() {
	int regs[4];

	// check AMD-specific functions
	CPUID( 0x80000000, regs );
	if ( regs[_REG_EAX] < 0x80000000 ) {
		return false;
	}

	// bit 31 of EDX denotes 3DNow! support
	CPUID( 0x80000001, regs );
	if ( regs[_REG_EDX] & ( 1 << 31 ) ) {
		return true;
	}

	return false;
}

/*
================
HasMMX
================
*/
static bool HasMMX() {
	int regs[4];

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 23 of EDX denotes MMX existence
	if ( regs[_REG_EDX] & ( 1 << 23 ) ) {
		return true;
	}
	return false;
}

/*
================
HasSSE
================
*/
static bool HasSSE() {
	int regs[4];

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 25 of EDX denotes SSE existence
	if ( regs[_REG_EDX] & ( 1 << 25 ) ) {
		return true;
	}
	return false;
}

/*
================
HasSSE2
================
*/
static bool HasSSE2() {
	int regs[4];

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 26 of EDX denotes SSE2 existence
	if ( regs[_REG_EDX] & ( 1 << 26 ) ) {
		return true;
	}
	return false;
}

/*
================
HasSSE3
================
*/
static bool HasSSE3() {
	int regs[4];

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 0 of ECX denotes SSE3 existence
	if ( regs[_REG_ECX] & ( 1 << 0 ) ) {
		return true;
	}
	return false;
}

/*
================
LogicalProcPerPhysicalProc
================
*/
#define NUM_LOGICAL_BITS   0x00FF0000     // EBX[23:16] Bit 16-23 in ebx contains the number of logical
                                          // processors per physical processor when execute cpuid with 
                                          // eax set to 1
static unsigned char LogicalProcPerPhysicalProc() {
	int regs[4];
	CPUID(1,regs);
	return static_cast<unsigned char>((regs[_REG_EBX] & NUM_LOGICAL_BITS) >> 16);
}

/*
================
GetAPIC_ID
================
*/
#define INITIAL_APIC_ID_BITS  0xFF000000  // EBX[31:24] Bits 24-31 (8 bits) return the 8-bit unique 
                                          // initial APIC ID for the processor this code is running on.
                                          // Default value = 0xff if HT is not supported
static unsigned char GetAPIC_ID() {
	int regs[4];
	CPUID(1, regs);
	return static_cast<unsigned char>((regs[_REG_EBX] & INITIAL_APIC_ID_BITS) >> 24);
}

/*
================
CPUCount

	logicalNum is the number of logical CPU per physical CPU
    physicalNum is the total number of physical processor
	returns one of the HT_* flags
================
*/
#define HT_NOT_CAPABLE				0
#define HT_ENABLED					1
#define HT_DISABLED					2
#define HT_SUPPORTED_NOT_ENABLED	3
#define HT_CANNOT_DETECT			4

int CPUCount( int &logicalNum, int &physicalNum ) {
	int statusFlag;
	SYSTEM_INFO info;

	physicalNum = 1;
	logicalNum = 1;
	statusFlag = HT_NOT_CAPABLE;

	info.dwNumberOfProcessors = 0;
	GetSystemInfo (&info);

	// Number of physical processors in a non-Intel system
	// or in a 32-bit Intel system with Hyper-Threading technology disabled
	physicalNum = info.dwNumberOfProcessors;  

	unsigned char HT_Enabled = 0;

	logicalNum = LogicalProcPerPhysicalProc();

	if ( logicalNum >= 1 ) {	// > 1 doesn't mean HT is enabled in the BIOS
		HANDLE hCurrentProcessHandle;
		DWORD64  dwProcessAffinity;
		DWORD64  dwSystemAffinity;
		DWORD64  dwAffinityMask;

		// Calculate the appropriate  shifts and mask based on the 
		// number of logical processors.

		unsigned char i = 1, PHY_ID_MASK  = 0xFF, PHY_ID_SHIFT = 0;

		while( i < logicalNum ) {
			i *= 2;
 			PHY_ID_MASK  <<= 1;
			PHY_ID_SHIFT++;
		}
		
		hCurrentProcessHandle = GetCurrentProcess();
		GetProcessAffinityMask( hCurrentProcessHandle, &dwProcessAffinity, &dwSystemAffinity );

		// Check if available process affinity mask is equal to the
		// available system affinity mask
		if ( dwProcessAffinity != dwSystemAffinity ) {
			statusFlag = HT_CANNOT_DETECT;
			physicalNum = -1;
			return statusFlag;
		}

		dwAffinityMask = 1;
		while ( dwAffinityMask != 0 && dwAffinityMask <= dwProcessAffinity ) {
			// Check if this CPU is available
			if ( dwAffinityMask & dwProcessAffinity ) {
				if ( SetProcessAffinityMask( hCurrentProcessHandle, dwAffinityMask ) ) {
					unsigned char APIC_ID, LOG_ID, PHY_ID;

					Sleep( 0 ); // Give OS time to switch CPU

					APIC_ID = GetAPIC_ID();
					LOG_ID  = APIC_ID & ~PHY_ID_MASK;
					PHY_ID  = APIC_ID >> PHY_ID_SHIFT;

					if ( LOG_ID != 0 ) {
						HT_Enabled = 1;
					}
				}
			}
			dwAffinityMask = dwAffinityMask << 1;
		}
	        
		// Reset the processor affinity
		SetProcessAffinityMask( hCurrentProcessHandle, dwProcessAffinity );
	    
		if ( logicalNum == 1 ) {  // Normal P4 : HT is disabled in hardware
			statusFlag = HT_DISABLED;
		} else {
			if ( HT_Enabled ) {
				// Total physical processors in a Hyper-Threading enabled system.
				physicalNum /= logicalNum;
				statusFlag = HT_ENABLED;
			} else {
				statusFlag = HT_SUPPORTED_NOT_ENABLED;
			}
		}
	}
	return statusFlag;
}

/*
================
HasHTT
================
*/
static bool HasHTT() {
	int regs[4];
	int logicalNum, physicalNum, HTStatusFlag;

	// get CPU feature bits
	CPUID( 1, regs );

	// bit 28 of EDX denotes HTT existence
	if ( !( regs[_REG_EDX] & ( 1 << 28 ) ) ) {
		return false;
	}

	HTStatusFlag = CPUCount( logicalNum, physicalNum );
	if ( HTStatusFlag != HT_ENABLED ) {
		return false;
	}
	return true;
}


/*
================================================================================================

	CPU

================================================================================================
*/

/*
========================
CountSetBits 
Helper function to count set bits in the processor mask.
========================
*/
DWORD CountSetBits( ULONG_PTR bitMask ) {
	DWORD LSHIFT = sizeof( ULONG_PTR ) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    

	for ( DWORD i = 0; i <= LSHIFT; i++ ) {
		bitSetCount += ( ( bitMask & bitTest ) ? 1 : 0 );
		bitTest /= 2;
	}

	return bitSetCount;
}

typedef BOOL (WINAPI *LPFN_GLPI)( PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD );

enum LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL {
    localRelationProcessorCore,
    localRelationNumaNode,
    localRelationCache,
	localRelationProcessorPackage
};

struct cpuInfo_t {
	int processorPackageCount;
	int processorCoreCount;
	int logicalProcessorCount;
	int numaNodeCount;
	struct cacheInfo_t {
		int count;
		int associativity;
		int lineSize;
		int size;
	} cacheLevel[3];
};

/*
========================
GetCPUInfo
========================
*/
bool GetCPUInfo( cpuInfo_t & cpuInfo ) {
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	PCACHE_DESCRIPTOR Cache;
	LPFN_GLPI	glpi;
	BOOL		done = FALSE;
	DWORD		returnLength = 0;
	DWORD		byteOffset = 0;

	memset( & cpuInfo, 0, sizeof( cpuInfo ) );

	glpi = (LPFN_GLPI)GetProcAddress( GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation" );
	if ( NULL == glpi ) {
		idLib::Printf( "\nGetLogicalProcessorInformation is not supported.\n" );
		return 0;
	}

	while ( !done ) {
		DWORD rc = glpi( buffer, &returnLength );

		if ( FALSE == rc ) {
			if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER ) {
				if ( buffer ) {
					free( buffer );
				}

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc( returnLength );
			} else {
				idLib::Printf( "Sys_CPUCount error: %d\n", GetLastError() );
				return false;
			}
		} else {
			done = TRUE;
		}
	}

	ptr = buffer;

	while ( byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength ) {
		switch ( (LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL) ptr->Relationship ) {
			case localRelationProcessorCore:
				cpuInfo.processorCoreCount++;

				// A hyperthreaded core supplies more than one logical processor.
				cpuInfo.logicalProcessorCount += CountSetBits( ptr->ProcessorMask );
				break;

			case localRelationNumaNode:
				// Non-NUMA systems report a single record of this type.
				cpuInfo.numaNodeCount++;
				break;

			case localRelationCache:
				// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
				Cache = &ptr->Cache;
				if ( Cache->Level >= 1 && Cache->Level <= 3 ) {
					int level = Cache->Level - 1;
					if ( cpuInfo.cacheLevel[level].count > 0 ) {
						cpuInfo.cacheLevel[level].count++;
					} else {
						cpuInfo.cacheLevel[level].associativity = Cache->Associativity;
						cpuInfo.cacheLevel[level].lineSize = Cache->LineSize;
						cpuInfo.cacheLevel[level].size = Cache->Size;
					}
				}
				break;

			case localRelationProcessorPackage:
				// Logical processors share a physical package.
				cpuInfo.processorPackageCount++;
				break;

			default:
				idLib::Printf( "Error: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n" );
				break;
		}
		byteOffset += sizeof( SYSTEM_LOGICAL_PROCESSOR_INFORMATION );
		ptr++;
	}

	free( buffer );

	return true;
}

/*
========================
Sys_GetCPUCacheSize
========================
*/
void Sys_GetCPUCacheSize( int level, int & count, int & size, int & lineSize ) {
	assert( level >= 1 && level <= 3 );
	cpuInfo_t cpuInfo;

	GetCPUInfo( cpuInfo );

	count = cpuInfo.cacheLevel[level - 1].count;
	size = cpuInfo.cacheLevel[level - 1].size;
	lineSize = cpuInfo.cacheLevel[level - 1].lineSize;
}

/*
========================
Sys_CPUCount

numLogicalCPUCores	- the number of logical CPU per core
numPhysicalCPUCores	- the total number of cores per package
numCPUPackages		- the total number of packages (physical processors)
========================
*/
void Sys_CPUCount( int & numLogicalCPUCores, int & numPhysicalCPUCores, int & numCPUPackages ) {
	cpuInfo_t cpuInfo;
	GetCPUInfo( cpuInfo );

	numPhysicalCPUCores = cpuInfo.processorCoreCount;
	numLogicalCPUCores = cpuInfo.logicalProcessorCount;
	numCPUPackages = cpuInfo.processorPackageCount;
}

/*
================
HasDAZ
================
*/
static bool HasDAZ() {
	_fxsave64(fxStatePtr);
	return ((MXCSR_MASK & (1 << 6)) == (1 << 6));	// Return if the DAZ bit is set
}


/*
================
Sys_GetCPUId
================
*/
cpuid_t Sys_GetCPUId() {
	int flags;

	// verify we're at least a Pentium or 486 with CPUID support
	if ( !HasCPUID() ) {
		return CPUID_UNSUPPORTED;
	}

	// check for an AMD
	if ( IsAMD() ) {
		flags = CPUID_AMD;
	} else {
		flags = CPUID_INTEL;
	}

	// check for Multi Media Extensions
	if ( HasMMX() ) {
		flags |= CPUID_MMX;
	}

	// check for 3DNow!
	if ( Has3DNow() ) {
		flags |= CPUID_3DNOW;
	}

	// check for Streaming SIMD Extensions
	if ( HasSSE() ) {
		flags |= CPUID_SSE | CPUID_FTZ;
	}

	// check for Streaming SIMD Extensions 2
	if ( HasSSE2() ) {
		flags |= CPUID_SSE2;
	}

	// check for Streaming SIMD Extensions 3 aka Prescott's New Instructions
	if ( HasSSE3() ) {
		flags |= CPUID_SSE3;
	}

	// check for Hyper-Threading Technology
	if ( HasHTT() ) {
		flags |= CPUID_HTT;
	}

	// check for Conditional Move (CMOV) and fast floating point comparison (FCOMI) instructions
	if ( HasCMOV() ) {
		flags |= CPUID_CMOV;
	}

	// check for Denormals-Are-Zero mode
	if ( HasDAZ() ) {
		flags |= CPUID_DAZ;
	}

	return (cpuid_t)flags;
}


/*
===============================================================================

	FPU

===============================================================================
*/

static char fxString[2048];

void Sys_FX_PrintState() {
	int length = 0;
	char* ptr = fxString;
	length += sprintf_s(ptr + length, 4096 - length, "----------------------------------------\n");
	length += sprintf_s(ptr + length, 4096 - length, "FCW  :0x%.4x   ",FCW);
	length += sprintf_s(ptr + length, 4096 - length, "FIP  :0x%.8x\n", FIP);
	length += sprintf_s(ptr + length, 4096 - length, "FOP  :0x%.4x   ",FOP);
	length += sprintf_s(ptr + length, 4096 - length, "FDP  :0x%.8x\n", FDP);
	length += sprintf_s(ptr + length, 4096 - length, "FDS  :0x%.4x\n", FDS);
	length += sprintf_s(ptr + length, 4096 - length, "FSW  :0x%.4x\n", FSW);
	length += sprintf_s(ptr + length, 4096 - length, "FCS  :0x%.4x\n", FCS);
	length += sprintf_s(ptr + length, 4096 - length, "FTW  :0x%.2x\n", FTW);
	length += sprintf_s(ptr + length, 4096 - length, "MXCSR:0x%.8x ", MXCSR);
	length += sprintf_s(ptr + length, 4096 - length, "MXCSR_MASK:0x%.8x\n", MXCSR_MASK);

	length += sprintf_s(ptr + length, 4096 - length, "----------------------------------------\n");
	length += sprintf_s(ptr + length, 4096 - length, "ST0  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[0][0], ST[0][1], ST[0][2], ST[0][3], ST[0][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST1  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[1][0], ST[1][1], ST[1][2], ST[1][3], ST[1][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST2  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[2][0], ST[2][1], ST[2][2], ST[2][3], ST[2][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST3  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[3][0], ST[3][1], ST[3][2], ST[3][3], ST[3][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST4  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[4][0], ST[4][1], ST[4][2], ST[4][3], ST[4][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST5  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[5][0], ST[5][1], ST[5][2], ST[5][3], ST[5][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST6  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[6][0], ST[6][1], ST[6][2], ST[6][3], ST[6][4]);
	length += sprintf_s(ptr + length, 4096 - length, "ST7  :0x%.4x%.4x%.4x%.4x%.4x\n", ST[7][0], ST[7][1], ST[7][2], ST[7][3], ST[7][4]);
	length += sprintf_s(ptr + length, 4096 - length, "----------------------------------------\n");
	length += sprintf_s(ptr + length, 4096 - length, "XMM0 :0x%.16llx%.16llx\n", XMM[0][0], XMM[0][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM1 :0x%.16llx%.16llx\n", XMM[1][0], XMM[1][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM2 :0x%.16llx%.16llx\n", XMM[2][0], XMM[2][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM3 :0x%.16llx%.16llx\n", XMM[3][0], XMM[3][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM4 :0x%.16llx%.16llx\n", XMM[4][0], XMM[4][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM5 :0x%.16llx%.16llx\n", XMM[5][0], XMM[5][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM6 :0x%.16llx%.16llx\n", XMM[6][0], XMM[6][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM7 :0x%.16llx%.16llx\n", XMM[7][0], XMM[7][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM8 :0x%.16llx%.16llx\n", XMM[8][0], XMM[8][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM9 :0x%.16llx%.16llx\n", XMM[9][0], XMM[9][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM10:0x%.16llx%.16llx\n", XMM[10][0], XMM[10][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM11:0x%.16llx%.16llx\n", XMM[11][0], XMM[11][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM12:0x%.16llx%.16llx\n", XMM[12][0], XMM[12][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM14:0x%.16llx%.16llx\n", XMM[13][0], XMM[13][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM14:0x%.16llx%.16llx\n", XMM[14][0], XMM[14][1]);
	length += sprintf_s(ptr + length, 4096 - length, "XMM15:0x%.16llx%.16llx\n", XMM[15][0], XMM[15][1]);

}


/*
===============
Sys_FPU_GetState

  gets the FPU state without changing the state
===============
*/
const char *Sys_FPU_GetState() {
	_fxsave64(fxStatePtr);

	Sys_FX_PrintState();

	return fxString;
}

/*
===============
Sys_FPU_EnableExceptions
===============
*/
void Sys_FPU_EnableExceptions( int exceptions ) {
	_fxsave64(fxStatePtr);
	FCW = (FCW | 63) & (~(exceptions & 63));
	_fxrstor64(fxStatePtr);
}

/*
===============
Sys_FPU_SetPrecision
===============
*/
void Sys_FPU_SetPrecision( int precision ) {
	short precisionBitTable[4] = { 0, 1, 3, 0 };
	short precisionBits = precisionBitTable[precision & 3] << 8;
	_fxsave64(fxStatePtr);
	FCW = (FCW & (~(3 << 9))) | precisionBits;
	_fxrstor64(fxStatePtr);
}

/*
================
Sys_FPU_SetRounding
================
*/
void Sys_FPU_SetRounding( int rounding ) {
	_fxsave64(fxStatePtr);
	FCW = (FCW & (~(3 << 11))) | ((rounding & 3)<<11);
	_fxrstor64(fxStatePtr);
}

/*
================
Sys_FPU_SetDAZ
================
*/
void Sys_FPU_SetDAZ( bool enable ) {
	unsigned long flag = 0;
	if (enable) { flag = 1 << 6; }
	_fxsave64(fxStatePtr);
	MXCSR = MXCSR & (~((1 << 6)))| flag;
	_fxrstor64(fxStatePtr);
}

/*
================
Sys_FPU_SetFTZ
================
*/
void Sys_FPU_SetFTZ( bool enable ) {
	unsigned long flag = 0;
	if (enable) { flag = 1 << 15; }
	_fxsave64(fxStatePtr);
	MXCSR = MXCSR & (~((1 << 15))) | flag;
	_fxrstor64(fxStatePtr);
}
