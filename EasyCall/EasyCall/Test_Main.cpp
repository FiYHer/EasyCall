
#include <iostream>
#include "EasyCall.hpp"
using namespace EasyCallSpace;
using namespace std;

//�򵥵Ĳ���MessageBoxA�������ú�����ԭ�����£�
//int WINAPI MessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)

int main(int agrc, char* argv[], char* enp[])
{
	//ָ��DLL�����������ǾͿ��Ե��ø�DLL��������к���
	EasyCall Mytest("User32.dll");

	//������Բ��ö�����Щ��С��ֱ��ת����OK ��static_cast<Type>(Context)
	HWND hTest = 0;
	LPCSTR lpTest = "Test666";
	UINT unTest = 6;
	//���ﻹ��һ����Ҫע���,���Ǻ����ĵ���Լ��һ��Ҫд�ϣ����������__stdcall
	//��д�ϵĻ�������ָ����ͱ���ģ����ſ������԰�������
	Mytest.Call<int __stdcall(HWND,LPCSTR,LPCSTR,UINT)>("MessageBoxA", hTest, lpTest, lpTest, unTest);

	//������PEID�Ȳ鿴��������ᷢ��û�з��ֵ���DLL��MessageBoxA����
	system("pause");
	return 0;
}