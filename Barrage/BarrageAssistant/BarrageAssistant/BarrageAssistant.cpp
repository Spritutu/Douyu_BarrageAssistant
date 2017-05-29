
// BarrageAssistant.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "BarrageAssistant.h"
#include "BarrageAssistantDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString g_BarrageCfgPath;
extern CString sCachePath;
extern CString g_PlayingPath;
// CBarrageAssistantApp

BEGIN_MESSAGE_MAP(CBarrageAssistantApp, CBCGPWinApp)
	ON_COMMAND(ID_HELP, &CBCGPWinApp::OnHelp)
END_MESSAGE_MAP()


// CBarrageAssistantApp ����

CBarrageAssistantApp::CBarrageAssistantApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	SetVisualTheme(BCGP_VISUAL_THEME_VS_2013_LIGHT);
}


// Ψһ��һ�� CBarrageAssistantApp ����

CBarrageAssistantApp theApp;


// CBarrageAssistantApp ��ʼ��

BOOL CBarrageAssistantApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	CString FilePath, tempStr;
	
	wchar_t file[255] = {0};
	GetModuleFileName(NULL, file, 255);
	tempStr.Format(L"%s", file);
	int nPos = tempStr.ReverseFind('\\');
	CString FileDir = tempStr.Left(nPos + 1);
	g_PlayingPath = FileDir;
	g_BarrageCfgPath = FileDir + L"BarrageAssistantFontCfg.ini";

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	CBarrageAssistantDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CBarrageAssistantApp::ExitInstance()
{
	GdiplusShutdown(m_gdiplusToken);

	return CWinApp::ExitInstance();
}
