
// BarrageAssistant.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBarrageAssistantApp:
// �йش����ʵ�֣������ BarrageAssistant.cpp
//

class CBarrageAssistantApp : public CBCGPWinApp
{
public:
	CBarrageAssistantApp();

// ��д
	public:
	virtual BOOL InitInstance();
public:
	ULONG_PTR m_gdiplusToken;  
public:
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBarrageAssistantApp theApp;