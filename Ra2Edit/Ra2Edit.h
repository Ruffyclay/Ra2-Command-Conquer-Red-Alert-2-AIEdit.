
// Ra2Edit.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRa2EditApp: 
// �йش����ʵ�֣������ Ra2Edit.cpp
//

class CRa2EditApp : public CWinApp
{
public:
	CRa2EditApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRa2EditApp theApp;