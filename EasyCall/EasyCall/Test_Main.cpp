
#include <iostream>
#include "EasyCall.hpp"
using namespace EasyCallSpace;
using namespace std;

//简单的测试MessageBoxA函数，该函数的原型如下：
//int WINAPI MessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)

int main(int agrc, char* argv[], char* enp[])
{
	//指定DLL，这样子我们就可以调用该DLL里面的所有函数
	EasyCall Mytest("User32.dll");

	//这里可以不用定义这些大小，直接转换就OK 用static_cast<Type>(Context)
	HWND hTest = 0;
	LPCSTR lpTest = "Test666";
	UINT unTest = 6;
	//这里还有一点需要注意的,就是函数的调用约定一定要写上，就像下面的__stdcall
	//不写上的话，函数指向完就报错的，不信可以试试啊哈哈哈
	Mytest.Call<int __stdcall(HWND,LPCSTR,LPCSTR,UINT)>("MessageBoxA", hTest, lpTest, lpTest, unTest);

	//可以用PEID等查看软件，将会发现没有发现导入DLL和MessageBoxA函数
	system("pause");
	return 0;
}