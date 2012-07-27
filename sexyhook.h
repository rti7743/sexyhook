#ifdef _MSC_VER 
	//for VC++
	#pragma once
#endif
#ifndef ____SEXYHOOK____72
#define ____SEXYHOOK____72
//
//接合部を強引に作成する
//SEXYHOOK
//
//
// by rti
//
//
// new BSD ライセンス / NYSL ライセンス 好きに使えばいいんぢゃない？ ^q^
//

//includeとか定義とか.
#ifdef _WINDOWS_
	//windows
	#include <windows.h>
	#include <imagehlp.h>
	#ifdef _MSC_VER 
		#if _MSC_VER <= 1200 
			typedef int intptr_t; 
			typedef unsigned int uintptr_t; 
		#endif 
	#else 
	#endif 
#else
	//linux
	#include <stdarg.h>
	#include <inttypes.h>
	#include <sys/mman.h>
	#include <string.h>
	typedef void* PROC;
#endif

//強制的にポインタにする(邪道)
#ifdef __GNUC__
	// ... を悪用した技
	static void* SEXYHOOK_DARKCAST(int dummy, ... )
	{
//		return *(void**)(&dummy + 1);
		va_list ap;
		va_start(ap, dummy);
		void* p = va_arg(ap, void*);
		va_end(ap);

		return p;
	}
#else
	//msvc のみ gcc だとエラーがでるけど、アーキテクチャ依存しないので好きだ!
	template<typename _T> static void* SEXYHOOK_DARKCAST(int dummy, _T p)
	{
		return *reinterpret_cast<void**>(&p);
	}
#endif

//ブレークポイントとアサーション
#ifdef __GNUC__
	//gcc
	#define SEXYHOOK_BREAKPOINT	{ asm ("int $3") ; }
	#define SEXYHOOK_ASSERT(f) \
		if (!(f) ) \
		{ \
			asm ("int $3") ; \
		} 
#else
	//msvc
	#define SEXYHOOK_BREAKPOINT	{ DebugBreak(); }
	#define SEXYHOOK_ASSERT(f) \
		if (!(f) ) \
		{ \
			MSG msg;	\
			BOOL bQuit = PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);	\
			if (bQuit)	PostQuitMessage((int)msg.wParam);	\
			DebugBreak(); \
		} 
#endif

//呼び出し規約
#ifdef __GNUC__
	//gcc
	#define SEXYHOOK_STDCALL			__attribute__ ((stdcall))
	#define SEXYHOOK_CDECL				__attribute__ ((cdecl))
	#define SEXYHOOK_FASTCALL			__attribute__ ((regparm(3)))
	#define SEXYHOOK_FASTCALLN(N)		__attribute__ ((regparm(N)))
	#define SEXYHOOK_CLASS											//__thiscall __cdecl とは違うらしい.
	#define SEXYHOOK_CLASS_STDCALL		__attribute__ ((stdcall))
	#define SEXYHOOK_CLASS_CDECL		__attribute__ ((cdecl))
	#define SEXYHOOK_CLASS_FASTCALL		__attribute__ ((regparm(3)))
	#define SEXYHOOK_CLASS_FASTCALLN(N)	__attribute__ ((regparm(N)))
#else
	//msvc
	#define SEXYHOOK_STDCALL		__stdcall
	#define SEXYHOOK_CDECL			__cdecl
	#define SEXYHOOK_FASTCALL		__fastcall
	#define SEXYHOOK_CLASS										//__thiscall __cdecl とは違うらしい.
	#define SEXYHOOK_CLASS_STDCALL	__stdcall
	#define SEXYHOOK_CLASS_CDECL	__cdecl
	#define SEXYHOOK_CLASS_FASTCALL	__fastcall
#endif

//__LINE__を文字列にするためのマクロ
//参考:http://oshiete1.goo.ne.jp/qa1219076.html
#define SEXYHOOK_LINE_TOSTR(x) SEXYHOOK_LINE_TOSTR_(x)
#define SEXYHOOK_LINE_TOSTR_(x) #x

#define SEXYHOOK_LINE_STRCAT(x,y) SEXYHOOK_LINE_STRCAT_(x,y)
#define SEXYHOOK_LINE_STRCAT_(x,y) x##y

//マクロを再度展開する.
#define SEXYHOOK_REMACRO(MACRO)	MACRO

//クラスメソッドでない場合は、 static 指定にしないといけないので、強引に static文字列を作り出す.
#define SEXYHOOK_STDCALL__STATIC			static
#define SEXYHOOK_CDECL__STATIC				static
#define SEXYHOOK_FASTCALL__STATIC			static
#define SEXYHOOK_FASTCALLN__STATIC			static
#define SEXYHOOK_CLASS__STATIC	
#define SEXYHOOK_CLASS_STDCALL__STATIC	
#define SEXYHOOK_CLASS_CDECL__STATIC	
#define SEXYHOOK_CLASS_FASTCALL__STATIC	
#define SEXYHOOK_CLASS_FASTCALLN__STATIC	

//クラスメソッドの場合は this を。そうでなければ NULL を作成する.
#define SEXYHOOK_STDCALL__THIS			NULL
#define SEXYHOOK_CDECL__THIS			NULL
#define SEXYHOOK_FASTCALL__THIS			NULL
#define SEXYHOOK_FASTCALLN__THIS		NULL
#define SEXYHOOK_CLASS__THIS			this
#define SEXYHOOK_CLASS_STDCALL__THIS	this
#define SEXYHOOK_CLASS_CDECL__THIS		this
#define SEXYHOOK_CLASS_FASTCALL__THIS	this

//関数なのかメソッドなのかで typedef の所を替える.
#define SEXYHOOK_STDCALL__HOOKDEF			
#define SEXYHOOK_CDECL__HOOKDEF			
#define SEXYHOOK_FASTCALL__HOOKDEF			
#define SEXYHOOK_FASTCALLN__HOOKDEF			
#define SEXYHOOK_CLASS__HOOKDEF				SEXYHOOKFuncBase::
#define SEXYHOOK_CLASS_STDCALL__HOOKDEF		SEXYHOOKFuncBase::
#define SEXYHOOK_CLASS_CDECL__HOOKDEF		SEXYHOOKFuncBase::
#define SEXYHOOK_CLASS_FASTCALL__HOOKDEF	SEXYHOOKFuncBase::
#define SEXYHOOK_CLASS_FASTCALLN__HOOKDEF	SEXYHOOKFuncBase::

//const同名
#define SEXYHOOK_STDCALL__CONST			
#define SEXYHOOK_CDECL__CONST			
#define SEXYHOOK_FASTCALL__CONST			
#define SEXYHOOK_FASTCALLN__CONST			
#define SEXYHOOK_CLASS__CONST				const
#define SEXYHOOK_CLASS_STDCALL__CONST		const
#define SEXYHOOK_CLASS_CDECL__CONST			const
#define SEXYHOOK_CLASS_FASTCALL__CONST		const
#define SEXYHOOK_CLASS_FASTCALLN__CONST		const


//SEXYHOOK本体 クラス内クラスとして実装する.
#define SEXYHOOK_BEGIN(RET,CALLTYPE,FUNCADDRESS,ARGS) \
	class SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__) : public SEXYHOOKFuncBase { \
	public: \
		SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)() \
		{ \
		} \
		void Hook(void* inVCallThisPointer = NULL,void* inFuncAddress2 = NULL) \
		{ \
			*(getSexyhookThisPointer()) =  (uintptr_t)this; \
			void* p = inFuncAddress2; \
			if (!p) p = EvalLoad(#FUNCADDRESS); \
			if (!p) p = SEXYHOOK_DARKCAST(0,(HookDef)FUNCADDRESS); \
			FunctionHookFunction( \
					 p \
					,SEXYHOOK_DARKCAST(0,&SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)::HookFunction) \
					,SEXYHOOK_DARKCAST(0,&SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)::CallOriginalFunction) \
					,inVCallThisPointer \
					); \
		} \
		void UnHook() \
		{ \
			FunctionUnHookFunction(); \
		} \
		virtual ~SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)() \
		{ \
			FunctionUnHookFunction(); \
		} \
		typedef RET ( SEXYHOOK_REMACRO(CALLTYPE##__HOOKDEF)* HookDef) ARGS ; \
		typedef RET ( SEXYHOOK_REMACRO(CALLTYPE##__HOOKDEF)* HookDefConst) ARGS SEXYHOOK_REMACRO(CALLTYPE##__CONST); \
		static uintptr_t* getSexyhookThisPointer() \
		{ \
			static uintptr_t thisSaver = 0; \
			return &thisSaver ; \
		} \
		SEXYHOOK_REMACRO(CALLTYPE##__STATIC) RET  CALLTYPE CallOriginalFunction ARGS \
		{ \
			volatile int mizumashi = 1 ; for(volatile int i = 0 ; i < 1000 ; i++){ mizumashi= mizumashi * i + 1; }; \
			throw 0;\
		} \
		SEXYHOOK_REMACRO(CALLTYPE##__STATIC) RET  CALLTYPE HookFunction ARGS \
		{ \
			SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)* sexyhookThis = ((SEXYHOOK_LINE_STRCAT(SEXYHOOKFunc,__LINE__)*)(*getSexyhookThisPointer()) ); \
			void * sexyhookOrignalThis = (void*) SEXYHOOK_REMACRO(CALLTYPE##__THIS);

#define SEXYHOOK_END \
	} } SEXYHOOK_LINE_STRCAT(objectFUNCHook,__LINE__);\
	SEXYHOOK_LINE_STRCAT(objectFUNCHook,__LINE__).Hook

#define SEXYHOOK_END_AS() \
	} } 



//一時的にフックをやめる 不要になりました。
#define SEXYHOOK_UNHOOK()	

//オリジナルの this pointer
#define SEXYHOOK_THIS(cast)	((cast) sexyhookOrignalThis )

//同名がいたりして、名前があいまいなった場合の解決策をユーザーに提供する.
#define SEXYHOOK_AUTOCAST_CONST(func)	(HookDefConst)(func)

//DLLをロードする構文。
//あれ、NULL返しているだけに見えるよね。うん。だけどね、ちゃんと動くから安心してほしい。
#define SEXYHOOK_DYNAMICLOAD(DLLNAME,FUNCNAME)	NULL

//関数フックを行う際に書き換える領域のサイズ
typedef  char FUNCTIONHOOK_ASM[14*3] ;

//関数/メソッドをフックする.
class SEXYHOOKFuncBase
{
	//書き換えたアドレスの先頭
	void*			 OrignalFunctionAddr;
	//書き換えた領域を保持する領域
	FUNCTIONHOOK_ASM OrignalAsm;
	//フックするのに使ったコード
	FUNCTIONHOOK_ASM HookAsm;
	//トランポリンを生成するコードのサイズ
	int              HookAsmSize;
	//オペコード長さを検討して決定した、上書きされるサイズ assert(SaveAsmSize >= HookAsmSize)
	int	             SaveAsmSize;

	//DLLインスタンス
	void*			 APIHookDLLHandle;

public:
	enum SEXYHOOK_CPU_ARCHITECTURE
	{
		 SEXYHOOK_CPU_ARCHITECTURE_X86
		,SEXYHOOK_CPU_ARCHITECTURE_X64
	};

	//関数フックを開始する.
	void FunctionHookFunction(void * inUkeFunctionProc , void * inSemeFunctionProc ,void * inCallOriginalFunctionProc , void * inVCallThisPointer )
	{
		//フックする関数(攻め)が開始するアドレスを求める
		uintptr_t semeFunctionAddr = CalcSemeFunctionAddress(inSemeFunctionProc );
		//フックされる関数(受け)が開始するアドレスを求める
		uintptr_t ukeFunctionAddr =  CalcUkeFunctionAddress(inUkeFunctionProc , inVCallThisPointer);


		//書き換えるマシン語
		//トランポリンフックを行うオペコードを生成し、長さを返す.
		this->HookAsmSize = MakeTrampolineHookAsm( &this->HookAsm , ukeFunctionAddr , semeFunctionAddr) ;
		//オペランド調を考慮して、破壊するバイトサイズを決定する.
		this->SaveAsmSize = CalcSeparateOpecodeLength(ukeFunctionAddr,this->HookAsmSize);

		this->OrignalFunctionAddr = (void*)ukeFunctionAddr;
		//フックされる関数の先頭を書き換えて、フックルーチンへ制御を移すようにする。
		BackupFunction(this->OrignalFunctionAddr ,&this->OrignalAsm,this->SaveAsmSize);
		OverraideFunction(this->OrignalFunctionAddr , this->HookAsm,this->HookAsmSize);

		//フック用に退避させたルーチンの末尾に元のコードに戻るjmpコードを埋め込む
		uintptr_t orignalCallFunctionAddr = CalcSemeFunctionAddress(inCallOriginalFunctionProc );
		int orginalCallAsmSize = MakeTrampolineHookAsm(
			 (FUNCTIONHOOK_ASM*) (((uintptr_t)&this->OrignalAsm) + this->SaveAsmSize)
			, orignalCallFunctionAddr + this->SaveAsmSize 
			, ukeFunctionAddr + this->SaveAsmSize ) ;
		OverraideFunction((void*)orignalCallFunctionAddr , OrignalAsm ,this->SaveAsmSize + orginalCallAsmSize);
		return ;
	}

	//フックするに使われた構文を見て、
	//SEXYHOOK_APILOAD だったら、dllのロードを仕込む。 evalならぬ、evil。
	void* EvalLoad(const char * inAddress)
	{
		//初期化する.
		this->APIHookDLLHandle = NULL;

		const char * p = inAddress;

		p = strstr(p , "SEXYHOOK_DYNAMICLOAD");
		if (!p) return NULL;

		//第1引数 DLL名
		const char * dll1 = strstr(p , "\"");
		if (!dll1) return NULL;
		const char * dll2 = strstr(dll1+1 , "\"");
		if (!dll2) return NULL;

		//第2引数 関数名
		const char * func1 = strstr(dll2+1 , "\"");
		if (!func1) return NULL;
		const char * func2 = strstr(func1+1 , "\"");
		if (!func2) return NULL;


		//文字列をバッファに複製する.
		char buffer[1024];
		int len = (int)(func2 - inAddress) + 1;
		if (len >= sizeof(buffer) )
		{
			//文字列デカすぎ.
			return NULL;
		}
		memcpy(buffer , inAddress , len );

		//終端埋め込んで、
		buffer[(int)(dll2 - inAddress) ] = '\0';
		buffer[(int)(func2 - inAddress) ] = '\0';
		//先頭の保持
		const char * apiHookDLLName = buffer + (int)(dll1 - inAddress + 1);
		const char * apiHookFuncName = buffer + (int)(func1 - inAddress + 1);
#ifdef _WINDOWS_
		//DLL読み込み
		HMODULE mod = LoadLibraryA(apiHookDLLName);
		if ( mod == NULL )
		{
			//DLLが読み込めません!
			SEXYHOOK_BREAKPOINT;
			return NULL;
		}
		//APIのアドレスを求める.
		void* orignalProc = (void*)GetProcAddress(mod , apiHookFuncName );
		if ( orignalProc == NULL )
		{
			//APIがロードできません。
			SEXYHOOK_BREAKPOINT;
			FreeLibrary(mod);
			return NULL;
		}
		//開放のための記録.
		this->APIHookDLLHandle = (void*)mod;
		
		return orignalProc;
#else
		return NULL;
#endif
	}

	//関数フックをやめる
	void FunctionUnHookFunction()
	{
		if (this->OrignalFunctionAddr == NULL)
		{
			return ;
		}
		OverraideFunction(this->OrignalFunctionAddr , this->OrignalAsm , this->SaveAsmSize);
		this->OrignalFunctionAddr = NULL;

		//APIをアンロードしなくてはいけない場合は開放する.
		if(this->APIHookDLLHandle)
		{
#ifdef _WINDOWS_
			FreeLibrary((HMODULE)this->APIHookDLLHandle);
#endif
			this->APIHookDLLHandle = NULL;
		}
	}
private:
	//フックした関数のアドレスを返す.
	void* getOrignalFunctionAddr()  const
	{
		return this->OrignalFunctionAddr;
	}

	//フックする関数(攻め)が開始するアドレスを求める
	uintptr_t CalcSemeFunctionAddress(void * inSemeFunctionProc )
	{
		//フック関数が開始される絶対アドレス
		if (*((unsigned char*)inSemeFunctionProc+0) == 0xe9)
		{
			//フック関数も ILT経由で飛んでくる場合
			//0xe9 call [4バイト相対アドレス]          4バイト相対は64bit でも変わらず
			uintptr_t jmpaddress = *((unsigned long*)((unsigned char*)inSemeFunctionProc+1));
			return (((uintptr_t)inSemeFunctionProc) + jmpaddress) + 5;
		}
		else
		{
			//即、プログラム領域に飛んでくる場合
			return (uintptr_t)inSemeFunctionProc ;
		}
	}

	//MSVC++ の vcallを解析する.
	uintptr_t CalcVCall(uintptr_t overraideFunctionAddr , void * inVCallThisPointer )
	{
		//仮想関数の vcallだった場合...
		int vcallhead = 0;
		if (
				*((unsigned char*)overraideFunctionAddr+0) == 0x8B
			&&	*((unsigned char*)overraideFunctionAddr+1) == 0x01
			&&	*((unsigned char*)overraideFunctionAddr+2) == 0xFF
			)
		{
			vcallhead = 3;
		}
		else if (
				*((unsigned char*)overraideFunctionAddr+0) == 0x48
			&&	*((unsigned char*)overraideFunctionAddr+1) == 0x8B
			&&	*((unsigned char*)overraideFunctionAddr+2) == 0x01
			&&	*((unsigned char*)overraideFunctionAddr+3) == 0xFF
			)
		{
			vcallhead = 4;
		}
		else
		{
			//vcallではない.
			return 0;
		}

		int plusAddress = 0;
		if (*((unsigned char*)overraideFunctionAddr+vcallhead ) == 0x20)
		{
			//[[this] + 0] にジャンプ
			plusAddress = 0;
		}
		else if (*((unsigned char*)overraideFunctionAddr+vcallhead ) == 0x60)
		{
			//[[this] + ?] にジャンプ
			plusAddress = (int) *((unsigned char*)overraideFunctionAddr+ (vcallhead + 1) );	//4バイト目の1バイト分が加算する値
		}
		else
		{
			//[[this] + ?] にジャンプを計算出来ませんでした...
			SEXYHOOK_BREAKPOINT;
		}
		//C言語のおせっかいで、ポインタは型分プラスしてしまうので、ポインタのサイズで割っとく.
		plusAddress = plusAddress / sizeof(void*);

		//このような関数に一時的に飛ばされている場合...
		//			vcall:
		//			00402BA0   mov         eax,dword ptr [ecx]
		//			00402BA2   jmp         dword ptr [eax]
		//8B 01 FF 20
		//
		// or
		//
		//004025D0   mov         eax,dword ptr [ecx]
		//004025D2   jmp         dword ptr [eax+4]
		//8B 01 FF 60 04
		if ( inVCallThisPointer == NULL )
		{
			//vcallのフックには、 thisポインタが必要です。
			//SEXYHOOK_CLASS_END_VCALL(thisClass) を利用してください。
			SEXYHOOK_BREAKPOINT;
		}

		/*
			//こういう演算をしたい  inVCallThisPointer = &this;
		_asm
		{
			mov ecx,inVCallThisPointer;
			mov ecx,[ecx];
			mov ecx,[ecx];
			mov overraideFunctionAddr,ecx;
		}

					or 
		_asm
		{
			mov ecx,inVCallThisPointer;
			mov ecx,[ecx];
			mov ecx,[ecx+4];			//+? は定義された関数分 virtualの数だけ増えるよ
			mov overraideFunctionAddr,ecx;
		}
		*/
		//多分こんな感じ,,,泣けてくるキャストだ.
		overraideFunctionAddr = (uintptr_t) *((void**)*((void***)inVCallThisPointer) + plusAddress);
		//そこにあるのは  関数の本体 jmp への命令のはず.
		if (*((unsigned char*)overraideFunctionAddr+0) == 0xe9)
		{
			//ついでなので関数の中を書き換えるため、関数の実体へのアドレスを求める.
			uintptr_t jmpaddress = *((unsigned long*)((unsigned char*)overraideFunctionAddr+1));
			overraideFunctionAddr = (((uintptr_t)overraideFunctionAddr) + jmpaddress) + 5;	//+5は e9 00 00 00 00 (ILTのサイズ)
		}

		return overraideFunctionAddr;
	}


	//フックされる関数(受け)が開始するアドレスを求める
	uintptr_t CalcUkeFunctionAddress(void * inUkeFunctionProc , void * inVCallThisPointer)
	{
		#ifdef __GNUC__
			//gccでは仮想関数のポインタを取得しようとすると、 vtable からの index を返してしまう。
			if ( (uintptr_t)inUkeFunctionProc < 100 )
			{
				//クラスのインスタンス(thisポインタ)が渡されていれば、indexから実体の場所を計算可能。
				if (inVCallThisPointer == NULL)
				{
					//thisがないなら計算不可能なので、とりあえずとめる.
					SEXYHOOK_BREAKPOINT;
				}
				//thisがあれば計算してアドレスを求める.
				//参考: http://d.hatena.ne.jp/Seasons/20090208/1234115944
				uintptr_t* vtable = (uintptr_t*) (*((uintptr_t*)inVCallThisPointer));
				//とりあえず、 (index - 1) / sizeof(void*) でアドレスが求まるみたい.
				//コレであっているのか不安だけど、とりあえず動くよ.
				uintptr_t index = ((uintptr_t)inUkeFunctionProc - 1) / sizeof(void*);

				//vtable から index を計算する.
				inUkeFunctionProc = (void*) (vtable[index] );
			}
		#endif
		
		//フックされる関数の実領域を求める.
		uintptr_t overraideFunctionAddr = 0;
		if (*((unsigned char*)inUkeFunctionProc+0) == 0xe9)
		{
			//フック関数も ILT経由で飛んでくる場合
			//0xe9 call [4バイト相対アドレス]
			uintptr_t jmpaddress = *((unsigned long*)((unsigned char*)inUkeFunctionProc+1));
			overraideFunctionAddr = (((uintptr_t)inUkeFunctionProc) + jmpaddress) + 5;	//+5は e9 00 00 00 00 (ILTのサイズ)
		}
		else
		{
			//即、プログラム領域に飛んでくる場合
			overraideFunctionAddr = (uintptr_t)inUkeFunctionProc;
		}

		//仮想関数の vcallだった場合...
		uintptr_t vcallFunctionAddr = this->CalcVCall(overraideFunctionAddr,inVCallThisPointer);
		if (vcallFunctionAddr != 0)
		{
			return vcallFunctionAddr;
		}
		return overraideFunctionAddr;
	}


	//トランポリンフックの作成
	int MakeTrampolineHookAsm(FUNCTIONHOOK_ASM* outBuffer , uintptr_t inUkeFunctionAddr, uintptr_t inSemeFunctionAddr) const
	{
#if (_WIN64 || __x86_64__)
		//参考 http://www.artonx.org/diary/200809.html
		//     http://hrb.osask.jp/wiki/?faq/asm
		if (inSemeFunctionAddr - inUkeFunctionAddr < 0x80000000)
		{//2G以下のjmp
			// 0xe9 相対アドレス   = 5バイト の命令
			*((unsigned char*)outBuffer+0) = 0xe9;	//近隣ジャンプ JMP
			*((unsigned long*)((unsigned char*)outBuffer+1)) = (unsigned long) (inSemeFunctionAddr - inUkeFunctionAddr - 5);	//-5はjmp命令自信のサイズ

			return 5;
		}
		else
		{//2G以上のjmp
			//Nikolay Igottiさんの方式を利用する. 14バイトの命令
			//push 64bit ; ret; で抜ける。 ret なので 相対ではなく絶対アドレス(inSemeFunctionAddr)を見る。
			//push 64bit がそのままだとできないので、分割して push する。
			//http://blogs.sun.com/nike/entry/long_absolute_jumps_on_amd64
			//http://www.ragestorm.net/blogs/?p=107
			*((unsigned char*)outBuffer+0) = 0x68;	//push imm32, subs 8 from rsp
			*((unsigned long*)((unsigned char*)outBuffer+1)) = 0x00000000ffffffff & inSemeFunctionAddr;

			*((unsigned char*)outBuffer+5) = 0xc7;	//mov imm32, 4(%rsp)
			*((unsigned char*)outBuffer+6) = 0x44;
			*((unsigned char*)outBuffer+7) = 0x24;
			*((unsigned char*)outBuffer+8) = 0x04;
			*((unsigned long*)((unsigned char*)outBuffer+9)) = inSemeFunctionAddr >> 32;

			*((unsigned char*)outBuffer+13) = 0xc3;	//ret

			return 14;
		}
#else
//#elif (_WIN32 || __i386__)
		//i386

		// 0xe9 相対アドレス   = 5バイト の命令
		*((unsigned char*)outBuffer+0) = 0xe9;	//近隣ジャンプ JMP
		*((unsigned long*)((unsigned char*)outBuffer+1)) = (unsigned long) (inSemeFunctionAddr - inUkeFunctionAddr - 5);	//-5はjmp命令自信のサイズ

		return 5;
#endif
	}

	//安全に上書きできるサイズを計算する.
	int CalcSeparateOpecodeLength(uintptr_t inAddr , int orderriadeSize) const
	{
		int len = 0;
		while(len < orderriadeSize)
		{
#if (_WIN64 || __x86_64__)
			const SEXYHOOK_CPU_ARCHITECTURE type = SEXYHOOK_CPU_ARCHITECTURE_X64;
#else
			const SEXYHOOK_CPU_ARCHITECTURE type = SEXYHOOK_CPU_ARCHITECTURE_X86;
#endif
			len += OperandLength(((const unsigned char*)inAddr) + len ,type);
		}
		SEXYHOOK_ASSERT(sizeof(FUNCTIONHOOK_ASM) > len );
		SEXYHOOK_ASSERT(len >= orderriadeSize);
		return len;
	}

	void BackupFunction(void* inAddr , void* outOldSrc , int size)
	{
		//割り込むコードを書き込みます。
		SEXYHOOK_ASSERT(outOldSrc != NULL);

		//バックアップ
		memcpy(outOldSrc , inAddr , size );
	}

	void OverraideFunction(void* inAddr , void* inNewSrc , int size)
	{
		#ifdef _WINDOWS_
			//書き換え許可
			unsigned long oldProtect = 0;
			VirtualProtect( inAddr , size , PAGE_EXECUTE_READWRITE , &oldProtect);
			//書き換え
			memcpy(inAddr , inNewSrc , size );
			//属性を元に戻す
			VirtualProtect( inAddr , size , oldProtect , &oldProtect);
		#else
			//mprotect に渡す値は、ページ境界(4K)に合わせる
			//http://d.hatena.ne.jp/kanbayashi/20081005/p2
			void *pageAddr = (void*)(((uintptr_t)inAddr) & 0xFFFFF000);
			//書き換え許可
			mprotect( pageAddr, 0x1000 , PROT_READ | PROT_WRITE | PROT_EXEC);
			//書き換え
			memcpy(inAddr , inNewSrc , size );
			//もとに戻したいんだけどどうすればいいの？
		#endif
	}

	//マシン語の長さを求めます。
	//http://www2.odn.ne.jp/~hab81760/modrm_sib.htm
	//http://dl.dropbox.com/u/2476414/TechResources/x86_opcodemap_1_b4.pdf
	//http://download.intel.com/jp/developer/jpdoc/IA32_Arh_Dev_Man_Vol2B_i.pdf
	//http://www.logix.cz/michal/doc/i386/app-a.htm
	//http://www.wdic.org/w/SCI/REX%E3%83%97%E3%83%AA%E3%83%95%E3%82%A3%E3%83%83%E3%82%AF%E3%82%B9
	//http://www.wdic.org/w/SCI/%E3%82%AA%E3%83%9A%E3%82%B3%E3%83%BC%E3%83%89%20(IA-32)
	//http://iwaohanma.web.fc2.com/
	static int OperandLength(const unsigned char* code,SEXYHOOK_CPU_ARCHITECTURE cputype)
	{
		//1バイト命令
		//0x00  仕様がよくわからない命令
		//0x01  1バイトの命令
		//0x02  2バイトの命令
		//...
		//0xff	modrm
		//0xfd	modrm + 4
		//0xfc	modrm + 1
		//0xfb  modrm + 4 or modrm + 1 or 2
		//0xfa  modrm + 1 or 2
		//0xf5	66 の影響を受ける 5バイト長の命令
		//0xe5	66 の影響を受ける 5バイト長の命令  64bitで48をつけると 64bitになる命令
		static unsigned char codeTable[] = {
//		0		1		2		3		4		5		6		7		8		9		a		b		c		d		e		f
/*0*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x00,//0
/*1*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//1
/*2*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//2
/*3*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//3
/*4*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//4
/*5*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//5
/*6*/	0x01,	0x01,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x05,	0xfd,	0x02,	0xfc,	0x01,	0x01,	0x00,	0x00,//6
/*7*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,//7
/*8*/	0xfc,	0xfd,	0xfd,	0xfc,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//8
/*9*/	0x01,	0x01,	0x05,	0x05,	0x01,	0x05,	0x05,	0x05,	0x01,	0x01,	0x00,	0x01,	0x01,	0x01,	0x01,	0x01,//9
/*a*/	0x05,	0x05,	0x05,	0x05,	0x01,	0x01,	0x01,	0x01,	0x02,	0xf5,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//a
/*b*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,//b
/*c*/	0xfc,	0xfc,	0x03,	0x01,	0xff,	0xff,	0xfc,	0xfd,	0x01,	0x01,	0x01,	0x03,	0x01,	0x02,	0x01,	0x01,//c
/*d*/	0xff,	0xff,	0xff,	0xff,	0x02,	0x02,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//d
/*e*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x05,	0x05,	0xf5,	0x02,	0xff,	0xff,	0xff,	0xff,//e
/*f*/	0x01,	0x00,	0x01,	0x00,	0x01,	0x00,	0xfa,	0xfb,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0xff,	0xff //f
		};

		//f0から始まる 2バイト命令
		static unsigned char codeTableF0[] = {
//		0		1		2		3		4		5		6		7		8		9		a		b		c		d		e		f
/*0*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x01,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,//0
/*1*/	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0x00,	0xff,//1
/*2*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,	0xff,	0x00,	0xff,	0xff,	0xff,	0xff,//2
/*3*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,//3
/*4*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//4
/*5*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//5
/*6*/	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,//6
/*7*/	0xfc,	0xfc,	0xfc,	0xfc,	0xff,	0xff,	0xff,	0x01,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,//7
/*8*/	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,//8
/*9*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x02,	0x02,	0x02,	0x02,	0xff,	0xff,	0xff,	0xff,//9
/*a*/	0x00,	0x00,	0x01,	0xff,	0xfc,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xfc,	0xff,	0xff,	0xff,//a
/*b*/	0x02,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0xfc,	0xff,	0xff,	0xff,	0xff,	0xff,//b
/*c*/	0x00,	0xff,	0xfc,	0x00,	0x00,	0x00,	0x00,	0xff,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//c
/*d*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//d
/*e*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//e
/*f*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00 //f
		};

		unsigned char len = codeTable[*code];
		unsigned char ex = 0;
		unsigned char registersize = 0;
		unsigned char registersize64 = 0;

		if (cputype == SEXYHOOK_CPU_ARCHITECTURE_X64)
		{//64bit対応 その1
			if (*code == 0xf2 || *code == 0xf3)
			{
				code =	code + 1;
				ex += 1;

				len = codeTable[*code];
			}
		}

		//prefix ぷよぷよだとダイアキュートみたいな
		if (*code >= 0x64 && *code <= 0x67 )
		{
			registersize = *code;

			code =	code + 1;
			ex += 1;
			len = codeTable[*code];
		}

		if (cputype == SEXYHOOK_CPU_ARCHITECTURE_X64)
		{//64bit対応 その2
			if ( registersize == 0x66 && *code == 0x2e)
			{//66 2e 0f 1f 84 00 00 00 00 00  長いnot のように、 66 2e と入るときがあるらしい。
				code =	code + 1;
				ex += 1;
			}

			if (*code >= 0x40 && *code <= 0x4f )
			{
				registersize64 = *code;

				code =	code + 1;
				ex += 1;
				len = codeTable[*code];
			}
		}

		//lock prefix
		if (*code == 0xf0)
		{
			code = code + 1;
			ex += 1;
			len = codeTable[*code];
		}

		//9b は謎w 
		if (*code == 0x9b)
		{
			if (*(code+1) == 0xdd)
			{// 9b dd b4 24 50 02 00 00  fsave  0x250(%rsp)
				return ex + 2 + 2 + 4;
			}
			else if (*(code+1) == 0x97)
			{//9b 97 66 00 ???
				return ex + 1 + 3;
			}
			else if (*(code+1) == 0x3f)
			{//9b 3f 08 ???
				return ex + 1 + 2;
			}
			else if (*(code+1) == 0xc4)
			{//9b c4 20   ???
				return ex + 1 + 2;
			}
			else
			{
				return ex + 1;
			}
		}

		//2バイトの長いオペコードの処理
		if (*code == 0x0f)
		{
			code = code + 1;
			ex += 1;
			len = codeTableF0[*code];
		}

		//repne / reps は 次のコードを見ないとわからない
		if (*code == 0xf2 || *code == 0xf3)
		{
			if ( (*(code + 1) & 0xf0)  == 0xa0)
			{
				return ex + 2;
			}
			return ex + 1;
		}

		if (len == 0)
		{
		}
		else if ( len <= 0x10)
		{
			return len + ex;
		}
		else if ( len == 0xf5 || len == 0xe5)
		{
			if (registersize == 0x66)
			{//66 bf と来ると word 単位でアクセスするらしい
				return 3 + ex;
			}
			else
			{
				if (len == 0xe5 && (registersize64 >= 0x48 && registersize64 <= 0x4f) )
				{//64bit拡張で 8バイトでアクセスできる命令
					return 9 + ex;
				}
				return 5 + ex;
			}
		}
		else
		{
			unsigned char mod = 0;
			unsigned char reg = 0;
			unsigned char rw =  0;
			int modlen = modrmLen( *(code + 1) ,*(code + 2) ,&mod,&reg,&rw);

			if ( len == 0xff)
			{
				return 1 + 1 + modlen + ex;
			}
			if ( len == 0xfd)
			{//c7 85 00 fc ff ff 00 00 00 00
			 //mov         dword ptr [ebp-400h],0
				if (registersize == 0x66)
				{//66 c7 と来ると word 単位でアクセスするらしい
					return 1 + 1 + modlen + 2 + ex;
				}
				else
				{
					return 1 + 1 + modlen + 4 + ex;
				}
			}
			if ( len == 0xfc)
			{//c1 f8 0a
			 //sar	eax,0x0a
				return 1 + 1 + modlen + 1 + ex;
			}
			if ( len == 0xfb)
			{//modrm によって長さが変わる
				if (reg == 0)
				{
					if (registersize == 0x66)
					{
						return 1 + 1 + 2 + modlen + ex;
					}
					else
					{
						return 1 + 1 + 4 + modlen + ex;
					}
				}
				else if (reg >= 2) return 1 + 1 + modlen + ex;
				else return 1 + 1 + ex;
			}
			if ( len == 0xfa)
			{//modrm によって長さが変わる
				if (reg == 0) return 1 + 1 + 1 + modlen + ex;
				else if (reg >= 2) return 1 + 1 + modlen + ex;
				else return 1 + 1 + ex;
			}
		}

		SEXYHOOK_BREAKPOINT;
		return 0;
	}

	static int modrmLen(unsigned char modrm,unsigned char sib,unsigned char *mod,unsigned char *reg,unsigned char *rw) 
	{
		*mod = (modrm & 0xc0) >> 6;
		*reg = (modrm & 0x38) >> 3;
		*rw =  (modrm & 0x07);

		switch (*mod)
		{
		case 0:
			switch(*rw)
			{
			case 0:		return 0;//eax
			case 1:		return 0;//ecx
			case 2:		return 0;//edx
			case 3:		return 0;//ebx
			case 4:
				{//SIB    sibのbaseビットによって長さが違う
					unsigned char base = (sib & 0x07);
					if (base == 5)	return 1 + 4;
					else			return 1;
				}
			case 5:		return 4;//disp32
			case 6:		return 0;//esi
			case 7:		return 0;//edi
			}
			break;
		case 1:
			switch(*rw)
			{
			case 0:		return 1;//eax + disp8
			case 1:		return 1;//ecx + disp8
			case 2:		return 1;//edx + disp8
			case 3:		return 1;//ebx + disp8
			case 4:		return 1 + 1;//SIB
			case 5:		return 1;//ebp + disp8
			case 6:		return 1;//esi + disp8
			case 7:		return 1;//edi + disp8
			}
		case 2:
			switch(*rw)
			{
			case 0:		return 4;//eax + disp32
			case 1:		return 4;//ecx + disp32
			case 2:		return 4;//edx + disp32
			case 3:		return 4;//ebx + disp32
			case 4:		return 1 + 4;//SIB
			case 5:		return 4;//ebp + disp32
			case 6:		return 4;//esi + disp32
			case 7:		return 4;//edi + disp32
			}
		case 3:
			switch(*rw)
			{
			case 0:		return 0;//al/ax/eax/mm0/xmm0
			case 1:		return 0;//cl/cx/ecx/mm1/xmm1
			case 2:		return 0;//dl/dx/edx/mm2/xmm2
			case 3:		return 0;//bl/bx/ebx/mm3/xmm3
			case 4:		return 0;//ah/sp/esp/mm4/xmm4
			case 5:		return 0;//ch/bp/ebp/mm5/xmm5
			case 6:		return 0;//dh/si/esi/mm6/xmm6
			case 7:		return 0;//bh/di/edi/mm7/xmm7
			}
		}
		SEXYHOOK_BREAKPOINT;
		return 0;
	}

};

#endif //____SEXYHOOK____72
