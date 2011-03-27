//
//SEXYHOOK sample and TEST
//
//by rti
//
//new BSD ライセンス / NYSLライセンス 好きに使えばいいんぢゃなイカ ^p^
//
#ifdef WIN32
	#include <windows.h>
#endif //WIN32
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sexyhook.h"	//セクシーフック
//#include "sexyhook_addrutil.h"	//セクシーフック ユーティリティ

//関数のテスト
int add(int a,int b)
{
	return a + b;
}

//クラスのテスト
class testclass
{
public:
	int add(int a , int b)
	{
		return a + b;
	}
};


class testclass2
{
public:
	bool check()
	{
		if (this->checkCore(1))
		{
			return true;
		}
		else
		{
			//ここがテストできない。
			return false;
		}
	}
	bool checkCore(int a )
	{
		return a ? true : false;
	}

	//2000年以上か？
	bool isOver2000year()
	{
		return time(NULL) >= 946652400;	//2000-01-01 00:00:00
	}

	static void test()
	{
		{
			printf("\r\nクラスメソッドのフックその2\r\n");
			//これで書き換えてしまえばもう大丈夫w
			SEXYHOOK_BEGIN(bool,SEXYHOOK_CLASS,&testclass2::checkCore,(int a))
			{
				return false;
			}
			SEXYHOOK_END();

			testclass2 cls;
			SEXYHOOK_ASSERT( !cls.check() );
		}
		//time() を埋め込んでいるプログラムも黙らせられるよ
		{
			printf("\r\ntime関数のフックその2\r\n");
			//これで書き換えてしまえばもう大丈夫w
			SEXYHOOK_BEGIN(time_t,SEXYHOOK_CDECL,&time,(time_t * a))
			{
				return 915116400;//1999-01-01 00:00:00
			}
			SEXYHOOK_END();

			//テスト
			testclass2 cls;
			bool r = cls.isOver2000year();
			SEXYHOOK_ASSERT( r == false );	//1999年です.
		}
		{
			printf("\r\ntime関数のフックが終わっていること\r\n");
			//スコープを抜けたのでtime()関数はもう上書きされない。
			testclass2 cls;
			bool r = cls.isOver2000year();
			SEXYHOOK_ASSERT( r == true );	//2000年以上です.
		}

	}
};

//仮想/純粋仮想メソッドのテスト
class Parent 
{
	int Dummy;
public:
	virtual int f() 
	{
		return 1;
	}
	virtual int g() =0;
};

class Child : public Parent 
{
	int Dummy2;
public:
	int aaa()
	{
		return 0;
	}
	virtual int f() 
	{
		return 2;
	}
	virtual int g() 
	{
		return 3;
	}
};

//テンプレートクラスのテスト
template<typename _T> class TempTest
{
public:
	int add(_T a,_T b)
	{
		return a + b;
	}
};

//オペレータオーバーロードのテスト
class OpClass
{
	int A;
public:
	OpClass(int inA) : A(inA)
	{
	}
	int getA() const
	{
		return this->A;
	}

	OpClass operator+( const OpClass& x )
	{
		OpClass z( this->A + x.A );
		return z;
	}

};

//グローバルなオペレーターオーバーロードには、まだ未対応です。ごめんなさい。
bool operator==(const OpClass& x, const OpClass& y) 
{
	return x.getA() == y.getA();
}

//オーバーロードのテスト
int uadd(int a,int b)
{
	return a + b;
}
char uadd(char a,char b)
{
	return a + b;
}


//テンプレート関数のテスト
//掛け算
template<typename T> T mul( const T& a,const T& b ){
       return a * b;
}

//おまけ dumpする関数
static void hexDump(void* p,int size)
{
	printf("\r\nDUMP %p:",p);

	int i = 0;
	for( ; i < size ; i ++)
	{
		printf(" %02X",
			0x000000ff &  (unsigned int)*(((char*)p)+i));
		if (i % 16 == 15)
		{
			printf("\r\n");
		}
	}
	printf("\r\n");
}

#ifdef __GNUC__
	//__stdcallなクラスメソッド.
	class stdcallclass
	{
	public:
		int __attribute__ ((stdcall)) add(int a , int b)
		{
			return a + b;
		}
	};
	//fastcallのテスト
	int __attribute__ ((regparm(3))) fast_add(int a,int b)
	{
		return a + b;
	}

	//__fastcallなクラスメソッド.
	class fastcallclass
	{
	public:
		int __attribute__ ((regparm(3))) add(int a , int b)
		{
			return a + b;
		}
	};
#else
	//__stdcallなクラスメソッド.
	class stdcallclass
	{
	public:
		int __stdcall add(int a , int b)
		{
			return a + b;
		}
	};
	//fastcallのテスト
	int __fastcall fast_add(int a,int b)
	{
		return a + b;
	}

	//__fastcallなクラスメソッド.
	class fastcallclass
	{
	public:
		int __fastcall add(int a , int b)
		{
			return a + b;
		}
	};
#endif



class classMethodCallTest
{
private:
public:
	int Secret;
	classMethodCallTest()
	{
		this->Secret = 0;
	}
	classMethodCallTest(int a)
	{
		this->Secret = a;
	}
	int Add(int a) const
	{
		return a + this->Secret;
	}
	int Sub(int a) const
	{
		return a - this->Secret;
	}
	int setA(int a)
	{
		this->Secret = a;
	}
};

class constdoumei
{
public:
	int add(int a, int b) const
	{
		return a + b + 10;
	}
	int add(int a, int b) 
	{
		return a + b + 20;
	}
};


//private
class private_testclass
{
private:
	int private_add(int a , int b)
	{
		return a + b;
	}
public:
	int public_add(int a , int b)
	{
		return private_add(a,b);
	}
};



int main()
{
	//フック元呼び出しなどのテスト
	{
		classMethodCallTest c(2);
		{
			int r = c.Add(10);		//10 + 2 = 12
			SEXYHOOK_ASSERT(r == 12);
		}
		//ただのフック その2 引数のチェック
		{ 
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				SEXYHOOK_ASSERT(a == 10);
				return 20;
			}
			SEXYHOOK_END();

			int r = c.Add(10);
			SEXYHOOK_ASSERT(r == 20);
		}
		//ただのフック
		{
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				return 10;
			}
			SEXYHOOK_END();

			int r = c.Add(10);
			SEXYHOOK_ASSERT(r == 10);
		}
		//別のメソッドの呼び出し
		{
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				return a * SEXYHOOK_THIS(classMethodCallTest*)->Sub(5);
			}
			SEXYHOOK_END();

			int r = c.Add(10);		//10 * (5 - 2) = 30
			SEXYHOOK_ASSERT(r == 30);
		}
		//別のメンバ変数の呼び出し(ただし public だけ)
		{
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				return a * SEXYHOOK_THIS(classMethodCallTest*)->Secret;
			}
			SEXYHOOK_END();

			int r = c.Add(10);		//10 * 2 = 20
			SEXYHOOK_ASSERT(r == 20);
		}

		//自分自身の再呼び出し その1
		{
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				{
					//一時的にフックを解除
					SEXYHOOK_UNHOOK();
					//元の関数を呼び出す
					return SEXYHOOK_THIS(classMethodCallTest*)->Add(a);
				}
			}
			SEXYHOOK_END();

			int r = c.Add(10);		//10 + 2 = 12
			SEXYHOOK_ASSERT(r == 12);
		}
		//自分自身の再呼び出し その2
		{
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&classMethodCallTest::Add,(int a))
			{
				{
					//一時的にフックを解除
					SEXYHOOK_UNHOOK();
					//元の関数を呼び出す
					return CallOrignalFunction(a);
				}
			}
			SEXYHOOK_END();

			int r = c.Add(10);		//10 + 2 = 12
			SEXYHOOK_ASSERT(r == 12);
		}
	}

	//仮想メソッド
	{
		printf("\r\n仮想メソッドのテスト\r\n");
		int cf,cg,pf,pg;
		Child child;

		printf("まだフックしていない\r\n");
		cf = child.f();
		cg = child.g();
		pf = ((Parent*)&child)->f();
		pg = ((Parent*)&child)->g();
		printf("child.f : %d\r\n",cf);
		printf("child.g : %d\r\n",cg);
		printf("((Parent*)&child)->f : %d\r\n",pf);
		printf("((Parent*)&child)->g : %d\r\n",pg);

		{
			//ここからフック
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&Child::g,())
			{
				return 103;	//Child::g
			}
			SEXYHOOK_END(&child);

			printf("Child.gをフック\r\n");
			cf = child.f();
			cg = child.g();
			pf = ((Parent*)&child)->f();
			pg = ((Parent*)&child)->g();
			printf("child.f : %d\r\n",cf);
			printf("child.g : %d\r\n",cg);
			printf("((Parent*)&child)->f : %d\r\n",pf);
			printf("((Parent*)&child)->g : %d\r\n",pg);
			SEXYHOOK_ASSERT(cg == 103);
			SEXYHOOK_ASSERT(pg == 103);
		}
		{
			//ここからフック
			SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&Child::f,())
			{
				return 102;	//Child::f
			}
			SEXYHOOK_END(&child);

			printf("Child.fをフック\r\n");
			cf = child.f();
			cg = child.g();
			pf = ((Parent*)&child)->f();
			pg = ((Parent*)&child)->g();
			printf("child.f : %d\r\n",cf);
			printf("child.g : %d\r\n",cg);
			printf("((Parent*)&child)->f : %d\r\n",pf);
			printf("((Parent*)&child)->g : %d\r\n",pg);
			SEXYHOOK_ASSERT(cf == 102);
			SEXYHOOK_ASSERT(pf == 102);
		}
	}


	//テンプレート関数のテスト
	{

		//割り算に変更
		SEXYHOOK_BEGIN( int, SEXYHOOK_CDECL, &mul<int>, (const int& a , const int& b))
		{
			return a  / b;
		}
		SEXYHOOK_END();

		int r1 = mul(9,3);			//こっちのみ書き換える
		long r2 = mul<long>(9,3);

		printf("\r\nテンプレート関数のテスト\r\n");
		printf("r1:%d // こっちだけ割り算に\r\n", r1);
		printf("r2:%d\r\n",(int) r2);
		SEXYHOOK_ASSERT(r1 == 3);
		SEXYHOOK_ASSERT(r2 == 27);
	}


	//グローバルなオペレーターオーバーロード
	{
		OpClass a(10);
		OpClass b(10);

		bool br = (a == b);

		SEXYHOOK_BEGIN(bool,SEXYHOOK_CDECL,&operator==,(const OpClass& x, const OpClass& y))
		{
			return false;
		}
		SEXYHOOK_END();

		//書き換えて等しくなくする.
		bool br2 = (a == b);

		printf("\r\nグローバルなオペレーターオーバーロードのテスト\r\n");
		printf("フック前 %d //等しいのでtrue \r\n",(int)br);
		printf("フック後 %d //常にfalse \r\n",(int)br2);

		SEXYHOOK_ASSERT(br == true);
		SEXYHOOK_ASSERT(br2 != true);
	}

	//オーバーロード(別型定義)のテスト
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_CDECL,&uadd,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		int r = uadd(1,2);
		printf("\r\nオーバーロード(別型定義)のテスト\r\n");
		printf("r1:%d //足し算を引き算に \r\n",r);
		SEXYHOOK_ASSERT(r == -1);
	}

	//演算子オーバーロードのテスト
	{
		OpClass a(10);
		OpClass b(20);
		//フック関数を定義する前だからフックされない
		OpClass c = a + b;

		//ここからフック
		SEXYHOOK_BEGIN(OpClass,SEXYHOOK_CLASS,&OpClass::operator +,(const OpClass& x))
		{
//			OpClass z( this->A - x.A );	//thisは使えない...
//			OpClass z( 1000 - x.A );	//そうか、クラスの中ではないのか...いっそfiriendに(ry
			OpClass z( 1000 );
			return z;
		}
		SEXYHOOK_END();

		//これはフックされる.
		OpClass d = a + b;
		printf("\r\n演算子オーバーロードのテスト\r\n");
		printf("OpClass(C) %d //固定値 \r\n" , c.getA());
		printf("OpClass(D) %d\r\n" , d.getA());
		SEXYHOOK_ASSERT(c.getA() == 30);
		SEXYHOOK_ASSERT(d.getA() == 1000);	//固定値
	}
	//テンプレートクラスのテスト
	{
		int r;

		TempTest<int> t;
		//ここからフック
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&TempTest<int>::add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		r = t.add(1,2);
		printf("\r\nテンプレートクラスのテスト\r\n");
		printf("TempTest<int> : %d //足し算を引き算に\r\n",r);
		SEXYHOOK_ASSERT(r == -1);
	}
	//クラスメソッドのテスト
	{
		//クラスメソッドの場合は SEXYHOOK_CLASS_HOOK_2_BEGIN を使ってください。 
		//2は引数の数です。
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&testclass::add,(int a,int b))
		{
			return a - b;	//足し算を引き算に上書きする.
		}
		SEXYHOOK_END();

		testclass myclass;
		int r = myclass.add(10,20);	//足し算を引き算に上書きする.

		printf("\r\nクラスメソッドのテスト\r\n");
		printf("r1 : %d\r\n",r);
		SEXYHOOK_ASSERT(r == -10);
	}

#ifdef _WINDOWS_
	//APIフックのテスト
	{
		//HeapCreate API を失敗させてみる。
		//もしかしたら、win95系だとだめかも、、、そんなの誰も使っていないからいいか。
		SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,&HeapCreate,(HANDLE a1,DWORD a2,SIZE_T a3))
		{
			return NULL;
		}
		SEXYHOOK_END();

		//フックして 
		HANDLE h = HeapCreate(0,0,100);
		printf("\r\nAPIフックのテスト\r\n");
		printf("HeapCreate : %p\r\n" , h);
		SEXYHOOK_ASSERT(h == NULL);
	}
	//APIフックのテスト その2
	{
		
		//HeapCreate API を失敗させてみる。
		//もしかしたら、win95系だとだめかも、、、そんなの誰も使っていないからいいか。
		SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD
					("kernel32.dll","HeapCreate"),(HANDLE a1,DWORD a2,SIZE_T a3))
		{
			return NULL;
		}
		SEXYHOOK_END();

		//フックして 
		HANDLE h = HeapCreate(0,0,100);
		printf("\r\nAPIフックのテスト\r\n");
		printf("HeapCreate : %p\r\n" , h);
		SEXYHOOK_ASSERT(h == NULL);
	}

	//ダイナミックリンクのフック
	{
		printf("ダイナミックリンク のテスト\r\n");

		SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD("dlltest.dll","DllAdd"),(int a , int b))
		{
			return a - b;
		}
		SEXYHOOK_END();


		HMODULE mod = LoadLibraryA("dlltest.dll");
		SEXYHOOK_ASSERT(mod != NULL);
		//export
		PROC orignalProc = GetProcAddress(mod , "DllAdd" );
		SEXYHOOK_ASSERT(orignalProc != NULL);

		int r = ((int (__stdcall *) (int,int))orignalProc)(1,2);
		printf("dlltest.dll : DllAdd : %d\r\n" , r);
		SEXYHOOK_ASSERT(r == -1);

		FreeLibrary(mod);
	}

	//ダイナミックリンクの中の windows apiのフック
	{
		printf("ダイナミックリンクの中の windows apiのフックのテスト\r\n");

		//ふつーにそのままAPIを
		SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL, MessageBoxA ,
					(HWND hWnd, LPCSTR lpText , LPCSTR lpCaption ,  UINT uType))
		{
			printf("lpText:%s , lpCaption:%s\r\n" , lpText,lpCaption);

			return -99;
		}
		SEXYHOOK_END();

		HMODULE mod = LoadLibraryA("dlltest.dll");
		SEXYHOOK_ASSERT(mod != NULL);
		//export
		PROC orignalProc = GetProcAddress(mod , "DllMessageBoxAPI" );
		SEXYHOOK_ASSERT(orignalProc != NULL);

		//フックして 
		int r = ((int (__stdcall *) (int,int))orignalProc)(1,2);
		printf("dlltest.dll : DllMessageBoxAPI : %d\r\n" , r);
		SEXYHOOK_ASSERT(r == -99);

		FreeLibrary(mod);
	}
	//ダイナミックリンクの中でさらにダイナミックリンクのフック
	{
		printf("ダイナミックリンクの中でさらにダイナミックリンクのテスト\r\n");
		//dlltest.dll の DllMul は、 dlltest2.dll の Dll2Mul を呼び出して掛け算を行って結果を返す.

		//dlltest"2".dll の Dll2Mul をフックする.
		SEXYHOOK_BEGIN(int,SEXYHOOK_STDCALL,SEXYHOOK_DYNAMICLOAD("dlltest2.dll","Dll2Mul"),(int a , int b))
		{
			return a / b;
		}
		SEXYHOOK_END();

		HMODULE mod = LoadLibraryA("dlltest.dll");
		SEXYHOOK_ASSERT(mod != NULL);
		//export
		PROC orignalProc = GetProcAddress(mod , "DllMul" );	//dlltest.dll の DllMul は、 dlltest2.dll の Dll2Mul を呼び出す.
		SEXYHOOK_ASSERT(orignalProc != NULL);

		int r = ((int (__stdcall *) (int,int))orignalProc)(4,2);	//4 * 2 ではなく、 4 / 2を実行
		printf("dlltest.dll : DllMul : %d\r\n" , r);
		SEXYHOOK_ASSERT(r == 2);	// 4 / 2 = 2

		FreeLibrary(mod);
	}
#endif //_WINDOWS_

	//関数のフックのテスト
	{
		//add関数を書き換えて引き算にする。
		SEXYHOOK_BEGIN(int,SEXYHOOK_CDECL,&add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		int cc = add(10,20);
		printf("\r\n関数のフックのテスト\r\n");
		printf("cc: %d\r\n",cc);
		SEXYHOOK_ASSERT(cc == -10);
	}
	//C関数strstrのフック
	{
		//これで書き換えてしまえばもう大丈夫w
		SEXYHOOK_BEGIN(const char *,SEXYHOOK_CDECL,&strstr,(const char * p,const char *p2))
		{
			return NULL;
		}
		SEXYHOOK_END();

		const char * str1 = "hello";
		const char * str2 = "hel";
		const char * p2 = strstr(str1,str2);	//strstr("hello","hel") だと gcc が -O0なのに最適化するらしい.
		printf("\r\nC関数strstrのフック\r\n");
		printf("strstr: %p //ヒットしたけど NULL \r\n",p2);
		SEXYHOOK_ASSERT(p2 == NULL);
	}
	//クラスメソッドでのテスト例
	{
		testclass2::test();
	}

	//__stdcallなクラスメソッドのテスト
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS_STDCALL,&stdcallclass::add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		printf("\r\n__stdcallなクラスメソッドのフック\r\n");
		stdcallclass c;
		int r = c.add(10,20);
		SEXYHOOK_ASSERT(r == -10);
	}

	//__fastcallのテスト
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_FASTCALL,&fast_add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		printf("\r\n__fastcallのフック\r\n");
		int r = fast_add(10,20);
		SEXYHOOK_ASSERT(r == -10);
	}
	

	//__fastcall class method のテスト
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS_FASTCALL,&fastcallclass::add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		printf("\r\n__fastcall class methodのフック\r\n");
		fastcallclass c;
		int r = c.add(10,20);
		SEXYHOOK_ASSERT(r == -10);
	}
#ifndef __GNUC__
	//const同名
	//gcc で動かない、、、、
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,&constdoumei::add,(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		printf("\r\nconst同名のフック constがつかない方\r\n");
		constdoumei c;
		int r = c.add(10,20);		//フック
		SEXYHOOK_ASSERT(r == -10);

		const constdoumei c2;
		r = c2.add(10,20);
		SEXYHOOK_ASSERT(r == 30 + 10);
	}
	{
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,SEXYHOOK_AUTOCAST_CONST(&constdoumei::add),(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();

		printf("\r\nconst同名のフック constがつく方\r\n");

		constdoumei c;
		int r = c.add(10,20);
		SEXYHOOK_ASSERT(r == 30 + 20);

		const constdoumei c2;
		r = c2.add(10,20);	//フック
		SEXYHOOK_ASSERT(r == -10);
	}
#endif	//__GNUC__

/*
	//private メソッドのアドレスを算出してフック. 
	//今のところ VC6 + PlatformSDKでしか動作しない、、なんで？
	{
		printf("\r\nprivate メソッドのアドレスを算出してフック\r\n");


		private_testclass c;
		//ここではまだフックしていない
		int r = c.public_add(10,20);
		SEXYHOOK_ASSERT(r == 30);

		//private メソッドのアドレスを算出してフック.
		SEXYHOOK_BEGIN(int,SEXYHOOK_CLASS,SEXYHOOKAddrUtil::strstr_addr("private_testclass::private_add"),(int a,int b))
		{
			return a - b;
		}
		SEXYHOOK_END();	

		//フックできたことを確認!
		r = c.public_add(10,20);
		SEXYHOOK_ASSERT(r == -10);
	}
*/
	puts("ok");
	return 0;
}
