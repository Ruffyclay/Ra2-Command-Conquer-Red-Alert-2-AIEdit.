#pragma once
// ����ת��������

#include "stdafx.h"



// ��CString���ַ�ת����char*���ַ���������Unicode�ַ�������ʹ��
inline char* CStringToChar(CString Cstr)
{
	char* chstr;
	int lenth = WideCharToMultiByte(CP_ACP, 0, Cstr, -1, NULL, 0, NULL, NULL);
	chstr = new char[lenth];
	WideCharToMultiByte(CP_ACP, 0, Cstr, -1, chstr, lenth, NULL, NULL);
	return chstr;
}
