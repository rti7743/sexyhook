#ifdef WIN32
	//for VC++
	#pragma once
#endif
#ifndef ____SEXYHOOK_ADDRUTIL____72
#define ____SEXYHOOK_ADDRUTIL____72
//
//SEXYHOOK addrutil
//関数のアドレスを デバッグ情報から取得する.
//
//
// by rti
//
//

#ifdef WIN32
	#include <windows.h>
	#include <imagehlp.h>
	#ifdef _MSC_VER 
		#if _MSC_VER <= 1200 
			typedef int intptr_t; 
			typedef unsigned int uintptr_t; 
		#endif 
	#else 
		#include <inttypes.h>
	#endif 
	#pragma comment(lib, "imagehlp.lib") 
	#pragma comment(lib, "dbghelp.lib") 
#else
	#include <inttypes.h>
	#include <sys/mman.h>
	#include <string.h>
	typedef void* PROC;
#endif

class SEXYHOOKAddrUtil
{
public:

#ifdef WIN32
	//参考 :pdb2map.
	static void* strstr_addr(const char * inFunctionName)
	{
		BOOL r;

		char filename[MAX_PATH] = {0};
		r = ::GetModuleFileNameA(NULL,filename, MAX_PATH);
		if ( !r )
		{
			//自分のファイル名を取得できません
			SEXYHOOK_BREAKPOINT;
		}

		//シンボルエンジンの初期化
		r = ::SymInitialize(NULL , NULL , FALSE);
		if (!r)
		{
			//エンジンを初期化できません.
			SEXYHOOK_BREAKPOINT;
		}

		//自分のファイルを開く. 
		IMAGEHLP_MODULE imageModule = { sizeof(IMAGEHLP_MODULE) };
        HANDLE fileHandle = CreateFileA ( filename,
                             GENERIC_READ ,
                             FILE_SHARE_READ,
                             NULL,
                             OPEN_EXISTING          ,
                             FILE_ATTRIBUTE_NORMAL  ,
                             NULL                    ) ;
        if ( INVALID_HANDLE_VALUE == fileHandle )
        {
			//自分のファイルを開けません
			SEXYHOOK_BREAKPOINT;
        }

        DWORD64 baseAddress = SymLoadModule64 (NULL,fileHandle,filename,NULL,0,0);
		if (!baseAddress)
		{
			CloseHandle(fileHandle);
			SEXYHOOK_BREAKPOINT;
		}
		
		class EnumSymProcCallbackClass
		{
		public:
			EnumSymProcCallbackClass(const char* inFunctionName)
			{
				this->FunctionName = inFunctionName;
				this->FoundAddress = NULL;
			}
			//めんどいからここでいいw
			const char * FunctionName;
			void* FoundAddress;
/*
			static BOOL CALLBACK EnumSymProc( 
				PSTR  SymbolName,
				ULONG SymbolAddress,
				ULONG SymbolSize,
				PVOID UserContext
				)
			{
////debug			    printf("%08X %4u %s\n", SymbolAddress, SymbolSize, SymbolName);
				EnumSymProcCallbackClass* myThis = (EnumSymProcCallbackClass*)UserContext;
				if ( strstr(SymbolName ,myThis->FunctionName) == NULL )
				{
					//見つからないので次へ
				    return TRUE;
				}
				//見つかった!
				myThis->FoundAddress = (void*)SymbolAddress;
				return FALSE;
			}
*/
			static BOOL CALLBACK EnumSymProc2( 
					PSYMBOL_INFO pSymInfo,
					ULONG SymbolSize,
					PVOID UserContext
				)
			{
////debug			    printf("%08X %4u %s\n", pSymInfo->Address, SymbolSize, pSymInfo->Name);
				EnumSymProcCallbackClass* myThis = (EnumSymProcCallbackClass*)UserContext;
				if ( strstr(pSymInfo->Name ,myThis->FunctionName) == NULL )
				{
					//見つからないので次へ
				    return TRUE;
				}
				//見つかった!
				myThis->FoundAddress = (void*)pSymInfo->Address;
				return FALSE;
			}

		} enumcallback(inFunctionName);
		
		//シンボルの列挙
//		r = SymEnumerateSymbols(NULL,baseAddress,(PSYM_ENUMSYMBOLS_CALLBACK)EnumSymProcCallbackClass::EnumSymProc,(void*)&enumcallback);
		r = SymEnumSymbols(NULL,baseAddress,NULL,(PSYM_ENUMERATESYMBOLS_CALLBACK)EnumSymProcCallbackClass::EnumSymProc2,(void*)&enumcallback);

		if (!r)
		{
			CloseHandle(fileHandle);
			SEXYHOOK_BREAKPOINT;
		}

		SymCleanup(NULL);
		CloseHandle(fileHandle);

		//見つからなかった？
		if (enumcallback.FoundAddress == NULL)
		{
			SEXYHOOK_BREAKPOINT;
		}

		return enumcallback.FoundAddress;
	}
#endif	// WIN32
};

#endif //____SEXYHOOK_ADDRUTIL____72
