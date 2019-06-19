#pragma once

#include <Windows.h>
#include <assert.h>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
using namespace std;

/*
C++实现的函数调用辅助类
1.简化从DLL里面调用函数的过程
2.隐藏函数的调用(不会出现在导入表里面,但是动态获取GetProcAddress和LoadLirary更好)
*/

namespace EasyCallSpace
{
	
	class EasyCall
	{
	private:
		//DLL的模块句柄，就像WinMain的第一个参数hInstance
		HMODULE m_nModule;

		//保存函数的容器，防止函数调用的时候二次查找
		//map因为红黑树实现插入时自动排序的，所以我们用无序map(hash实现?)
		unordered_map<string, FARPROC> m_cMap;

	public:
		/*
		@strDllDLL:的名字
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
		@strFunc:函数的名字
		@args:函数的参数
		*/
		template<typename T,typename... Args>
		typename result_of<function<T>(Args...)>::type Call(const string& strFunc, Args&&... args)
		{
			//确保函数名字不为空
			assert(strFunc.size() && "note:Please input function name");

			//查找函数容器里面是否保存有该函数
			auto Func = m_cMap.find(strFunc);
			
			//如果没有保存该函数的话就需要去获取
			if (Func == m_cMap.end())
			{
				//获取DLL里面的函数
				auto pAddr = GetProcAddress(m_nModule, strFunc.c_str());
				if (!pAddr)
				{
					throw exception("Can't Get Function");
				}

				//保存到容器里面
				m_cMap.insert(make_pair(strFunc, pAddr));	

				//再获取出来
				Func = m_cMap.find(strFunc);
			}

			//获取函数指针
			auto pFunction = function<T>((T*)(Func->second));

			//执行函数并返回函数的返回值
			return pFunction(forward<Args>(args)...);
		}

	};
}
