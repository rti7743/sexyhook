// dlltest.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extern "C" __declspec(dllexport) int CALLBACK Dll2Mul(int a , int b)
{
	return a * b;
}

