
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// BarrageAssistant.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"



void DoEvent()
{
	static MSG msg;
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	::TranslateMessage(&msg);
	::DispatchMessage(&msg);
}
