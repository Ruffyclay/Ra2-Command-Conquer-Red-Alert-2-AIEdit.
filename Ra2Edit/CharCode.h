#pragma once
// 编码转换工具箱

#include "stdafx.h"



// 将CString型字符转换成char*型字符，仅限于Unicode字符编码下使用
inline char* CStringToChar(CString Cstr)
{
	char* chstr;
	int lenth = WideCharToMultiByte(CP_ACP, 0, Cstr, -1, NULL, 0, NULL, NULL);
	chstr = new char[lenth];
	WideCharToMultiByte(CP_ACP, 0, Cstr, -1, chstr, lenth, NULL, NULL);
	return chstr;
}
