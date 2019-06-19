#pragma once

#include <Windows.h>
#include <assert.h>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
using namespace std;

/*
C++ʵ�ֵĺ������ø�����
1.�򻯴�DLL������ú����Ĺ���
2.���غ����ĵ���(��������ڵ��������,���Ƕ�̬��ȡGetProcAddress��LoadLirary����)
*/

namespace EasyCallSpace
{
	
	class EasyCall
	{
	private:
		//DLL��ģ����������WinMain�ĵ�һ������hInstance
		HMODULE m_nModule;

		//���溯������������ֹ�������õ�ʱ����β���
		//map��Ϊ�����ʵ�ֲ���ʱ�Զ�����ģ���������������map(hashʵ��?)
		unordered_map<string, FARPROC> m_cMap;

	public:
		/*
		@strDllDLL:������
		*/
		EasyCall(const string& strDll)
		{
			assert(strDll.size() && "note: Please input dll name");

			m_nModule = LoadLibraryA(strDll.c_str());

			assert(m_nModule && "note: Load dll fail");
		}

		~EasyCall()
		{
			if (m_nModule)
				FreeLibrary(m_nModule);
		}

	public:

		/*
		@strFunc:����������
		@args:�����Ĳ���
		*/
		template<typename T,typename... Args>
		typename result_of<function<T>(Args...)>::type Call(const string& strFunc, Args&&... args)
		{
			//ȷ���������ֲ�Ϊ��
			assert(strFunc.size() && "note:Please input function name");

			//���Һ������������Ƿ񱣴��иú���
			auto Func = m_cMap.find(strFunc);
			
			//���û�б���ú����Ļ�����Ҫȥ��ȡ
			if (Func == m_cMap.end())
			{
				//��ȡDLL����ĺ���
				auto pAddr = GetProcAddress(m_nModule, strFunc.c_str());
				if (!pAddr)
				{
					throw exception("Can't Get Function");
				}

				//���浽��������
				m_cMap.insert(make_pair(strFunc, pAddr));	

				//�ٻ�ȡ����
				Func = m_cMap.find(strFunc);
			}

			//��ȡ����ָ��
			auto pFunction = function<T>((T*)(Func->second));

			//ִ�к��������غ����ķ���ֵ
			return pFunction(forward<Args>(args)...);
		}

	};
}
