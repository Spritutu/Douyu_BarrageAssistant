// LyricsWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TextAeroWindow.h"


HWND g_OsdhWhd = NULL;
// CTextAeroWindow

IMPLEMENT_DYNAMIC(CTextAeroWindow, CWnd)

CTextAeroWindow::CTextAeroWindow()
{
	HDC hDC=::GetDC(NULL);
	m_hCacheDC=::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL,hDC);
	//---------------------------------
	m_lpszTextStr=NULL ; //Unicode��ʽ���ı�
	m_nHighlight=NULL ; //�����ı��İٷֱ� 0--100
	m_TextGradientMode=GradientMode_None ; //��ͨ�ı�����ģʽ
	m_pTextPen=NULL ; //��ͨ�ı��߿򻭱�
	m_HighlightGradientMode=GradientMode_None ; //�����ı�����ģʽ
	m_pHighlightPen=NULL ; //�����ı��߿򻭱�
	m_pShadowBrush=NULL ; //��Ӱ��ˢ,GDIPlus��ˢ 
	m_nShadowOffset=GradientMode_None ; //��Ӱƫ��
	m_pFont=NULL ; //GDIPlus����
	m_FontStyle=NULL ; 
	m_FontSize=NULL ; 
	m_pTextFormat=NULL;
	//---------------------------------
	m_pFontFamily=new Gdiplus::FontFamily();
	m_pTextFormat=new Gdiplus::StringFormat();
	m_pTextFormat->SetFormatFlags(StringFormatFlagsNoClip);
	m_pTextFormat->SetAlignment(StringAlignmentNear); //������뷽ʽ
	m_pTextFormat->SetLineAlignment(StringAlignmentFar); //�ô�ֱ���뷽ʽ

	//---------------------------------
	SetTextStrFont(L"����",15,FontStyleBold);
	SetTextStrColor(Gdiplus::Color::Red,Gdiplus::Color(255,172,0),GradientMode_Three);
	SetTextStrBorder(Gdiplus::Color::Black,1);
	SetTextStrShadow(Gdiplus::Color(150,0,0,0),1);
	SetHighlightColor(Gdiplus::Color(255,100,26),Gdiplus::Color(255,255,0),GradientMode_Three);
	SetHighlightBorder(Gdiplus::Color::Black,1);
	m_bLock = FALSE;
	//hAccel=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));


	//CWnd *TmpWnd;
	
	//TmpWnd = FromHandle(this->GetSafeHwnd());
	//TmpWnd->ShowWindow(SW_SHOW);
	//TmpWnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	
}

CTextAeroWindow::~CTextAeroWindow()
{
	if(m_lpszTextStr){
		delete  m_lpszTextStr;
		m_lpszTextStr=NULL;
	}
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(m_pFontFamily){
		delete m_pFontFamily;
		m_pFontFamily=NULL;
	}	
	if(m_pTextFormat){
		delete m_pTextFormat;
		m_pTextFormat=NULL;
	}	
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
}


BEGIN_MESSAGE_MAP(CTextAeroWindow, CWnd)
	ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	ON_COMMAND(ID_LOCK_OSD, &CTextAeroWindow::OnLockOsd)
	ON_COMMAND(ID_UNLOCK_OSD, &CTextAeroWindow::OnUnlockOsd)
	//ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_LOCK_OSD, &CTextAeroWindow::OnUpdateLockOsd)
	ON_UPDATE_COMMAND_UI(ID_UNLOCK_OSD, &CTextAeroWindow::OnUpdateUnlockOsd)
	ON_WM_INITMENUPOPUP()
	ON_WM_MOVING()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



BOOL CTextAeroWindow::Create()
{
	return CTextAeroWindow::Create(_T("CometTextAeroWindow"),-1,-1);
}
BOOL CTextAeroWindow::Create(LPCTSTR lpszClassName)
{
	return CTextAeroWindow::Create(lpszClassName,-1,-1);
}
BOOL CTextAeroWindow::Create(LPCTSTR lpszClassName,int nWidth,int nHeight)
{
	if(!RegisterWndClass(lpszClassName))
	{
		TRACE("Class��Registration��Failedn");
	}
	//--------------------------------------------
	//ȡ�����湤������
	RECT rcWork;
	SystemParametersInfo (SPI_GETWORKAREA,NULL,&rcWork,NULL);
	int nWorkWidth=rcWork.right-rcWork.left;
	int nWorkHeight=rcWork.bottom-rcWork.top;
	//δ���ݿ�ȡ��߶Ȳ���ʱ���ø�Ĭ��ֵ
	if(nWidth<0)nWidth=nWorkWidth;
	if(nHeight<0)nHeight=150;
	//������ߡ�����λ��,�ô�������Ļ�·�
	int x=rcWork.left+( (nWorkWidth-nWidth)/2 );
	int y=rcWork.bottom-nHeight;
	//--------------------------------------------
	DWORD dwStyle=WS_POPUP|WS_VISIBLE;
	DWORD dwExStyle=WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED;
	if(!CWnd::CreateEx(dwExStyle,lpszClassName,NULL,dwStyle,x,y,nWidth,nHeight,NULL,NULL))
		return FALSE;
}
BOOL CTextAeroWindow::RegisterWndClass(LPCTSTR lpszClassName)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	WNDCLASSEX wndcls;
	memset(&wndcls,0,sizeof(WNDCLASSEX));
	wndcls.cbSize=sizeof(WNDCLASSEX);
	if(GetClassInfoEx(hInstance,lpszClassName,&wndcls))
	{
		return TRUE;
	}
	if(GetClassInfoEx(NULL,lpszClassName,&wndcls))
	{
		return TRUE;
	}

	wndcls.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc=::DefWindowProc;
	wndcls.hInstance=hInstance;
	wndcls.hIcon=NULL;
	wndcls.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wndcls.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndcls.lpszMenuName=NULL;
	wndcls.lpszClassName=lpszClassName;
	if(!RegisterClassEx(&wndcls))
	{
		return FALSE;
	}
	return TRUE;
}


//�����ı�(�ı��ı�,�������Ȱٷֱ�)
void CTextAeroWindow::UpdateTextStr(LPCSTR lpszText,float nHighlight)
{
	if(m_lpszTextStr){
		delete  m_lpszTextStr;
		m_lpszTextStr=NULL;
	}
	if(lpszText){
		//���ݽ�����ANSI�汾���ַ���,��Ҫת����Unicode
		int nLen=MultiByteToWideChar(CP_ACP,0,lpszText,-1, NULL,NULL);
		if(nLen){
			m_lpszTextStr = new WCHAR[nLen+1];
			ZeroMemory(m_lpszTextStr,sizeof(WCHAR)*(nLen+1));
			MultiByteToWideChar(CP_ACP,0,lpszText,-1,m_lpszTextStr,nLen); 
		}		
	}
	UpdateTextStr(nHighlight);
}
void CTextAeroWindow::UpdateTextStr(LPCWSTR lpszText,float nHighlight)
{
	if(m_lpszTextStr){
		delete  m_lpszTextStr;
		 m_lpszTextStr=NULL;
	}
	int nLen=0;
	if(lpszText){
		nLen=lstrlenW(lpszText);
	}
	if(nLen>0){
		m_lpszTextStr=new WCHAR[nLen+1];
		ZeroMemory(m_lpszTextStr,sizeof(WCHAR)*(nLen+1));
		CopyMemory(m_lpszTextStr,lpszText,sizeof(WCHAR)*(nLen));
	}
	UpdateTextStr(nHighlight);
}
//���¸�������(�������Ȱٷֱ�)
void CTextAeroWindow::UpdateTextStr(float nHighlight)
{
	m_nHighlight=nHighlight;
	if(m_nHighlight<0)
		m_nHighlight=0;
	if(m_nHighlight>100)
		m_nHighlight=100;
	Draw();
}
//�ػ��ı�����
void CTextAeroWindow::Draw()
{
	CRect rcClient;
	GetClientRect(rcClient);
	m_nWidth=rcClient.Width();
	m_nHeight=rcClient.Height();

	//----------------------------------
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = m_nHeight;
	bitmapinfo.bmiHeader.biWidth = m_nWidth;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
	//----------------------------------
	Gdiplus::Graphics* pGraphics=new Gdiplus::Graphics(m_hCacheDC);
	pGraphics->SetSmoothingMode (SmoothingModeHighQuality);
	pGraphics->SetTextRenderingHint (TextRenderingHintClearTypeGridFit);
	DrawTextStr(pGraphics);
	delete pGraphics;
	//----------------------------------
	//����͸������
	CPoint DestPt(0,0);
	CSize psize(m_nWidth,m_nHeight);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = 255;
	HDC hDC=::GetDC(m_hWnd);
	
	
	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);//ULW_ALPHA   ULW_OPAQUE

	//BYTE byteAlpha=50;
	//::SetLayeredWindowAttributes(m_hWnd, 0, byteAlpha, ULW_ALPHA);

	//----------------------------------
	//�ͷ���Դ
	::SelectObject (m_hCacheDC,hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hDC);
}
//�����ı�
void CTextAeroWindow::DrawTextStr(Gdiplus::Graphics* pGraphics)
{
	//��ȡ�����ֿ�Ⱥ͸߶�
	RectF layoutRect(0,0,500,0);
	RectF boundingBox;
	pGraphics->MeasureString (m_lpszTextStr, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
	//SizeF tmpSize = GetTextBounds(m_pFont, m_pTextFormat, m_lpszTextStr);
	//boundingBox.Width = tmpSize.Width;
	//boundingBox.Height = tmpSize.Height;
	//�����ı�������λ��
	RectF dstRect((/*m_nWidth - boundingBox.Width*/0), (0/*m_nHeight - boundingBox.Height*/) ,boundingBox.Width,boundingBox.Height);

//   	dstRect.X = 0;
//   	dstRect.Y = 0;
//   	dstRect.Width = 400;
//   	dstRect.Height = 500;
	if(dstRect.X<0)dstRect.X=0;
	if(dstRect.Width>m_nWidth)dstRect.Width=m_nWidth;

	//-----------------------------------------------------------
	//������Ӱ
 	if(m_pShadowBrush){
 		layoutRect=dstRect;
 		layoutRect.X=layoutRect.X + m_nShadowOffset;
 		layoutRect.Y=layoutRect.Y + m_nShadowOffset;
 		Gdiplus::GraphicsPath* pShadowPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
 		pShadowPath->AddString (m_lpszTextStr, -1, m_pFontFamily, m_FontStyle, m_FontSize, layoutRect, m_pTextFormat); //�����ּ���·��
 		pGraphics->FillPath (m_pShadowBrush,pShadowPath);//���·��
 		delete pShadowPath; //����·��
 	}

	//-----------------------------------------------------------
	//�����ı�
	Gdiplus::GraphicsPath* pStringPath=new Gdiplus::GraphicsPath(FillModeAlternate);//����·��
	pStringPath->AddString (m_lpszTextStr, -1, m_pFontFamily, m_FontStyle, m_FontSize,dstRect, m_pTextFormat); //�����ּ���·��
	if(m_pTextPen){
		pGraphics->DrawPath (m_pTextPen,pStringPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_TextGradientMode, m_TextColor1,m_TextColor2,dstRect);
	pGraphics->FillPath (pBrush,pStringPath);//���·��
	delete pBrush;//���ٻ�ˢ
	DrawHighlightTextStr(pGraphics,pStringPath,dstRect);
	delete pStringPath; //����·��

}
//���Ƹ����ı�
void CTextAeroWindow::DrawHighlightTextStr(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath,RectF& dstRect)
{
	if(m_nHighlight<=0)return;
	Gdiplus::Region* pRegion=NULL;
	if(m_nHighlight<100)
	{
		RectF CliptRect(dstRect);
		CliptRect.Width = /*CliptRect.Width * */m_nHighlight/* / 100.0f*/;
		pRegion=new Gdiplus::Region(CliptRect);
		pGraphics->SetClip(CliptRect,CombineModeReplace);
	}
 	//--------------------------------------------
 	if(m_pHighlightPen)
	{
 		pGraphics->DrawPath (m_pHighlightPen,pPath);//��·��,���ֱ߿�
 	}
  	Gdiplus::Brush* pBrush = CreateGradientBrush(m_HighlightGradientMode, m_HighlightColor1,m_HighlightColor2,dstRect);
  	pGraphics->FillPath (pBrush,pPath);//���·��
  	delete pBrush;//���ٻ�ˢ
 	//--------------------------------------------
	if(pRegion)
	{
		pGraphics->ResetClip();
		delete pRegion;
	}
}
//�������仭ˢ
Gdiplus::Brush* CTextAeroWindow::CreateGradientBrush(GradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2,RectF& dstRect)
{
	PointF pt1;
	PointF pt2;
	Gdiplus::Brush* pBrush=NULL;
	switch (TextGradientMode)
	{
	case GradientMode_Two://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	case GradientMode_Three://��ɫ����
		{
			PointF point1(dstRect.X,dstRect.Y);
			PointF point2(dstRect.X,dstRect.Y+dstRect.Height/2);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((LinearGradientBrush*)pBrush)->SetWrapMode(WrapModeTileFlipXY);
			break;
		}

	default://�޽���
		{
			pBrush=new Gdiplus::SolidBrush(Color1);
			break;
		}
	}
	return pBrush;
}

//�����ı���ɫ
void CTextAeroWindow::SetTextStrColor(Gdiplus::Color TextColor1)
{
	CTextAeroWindow::SetTextStrColor(TextColor1,Gdiplus::Color::Black,GradientMode_None);
}
void CTextAeroWindow::SetTextStrColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,GradientMode TextGradientMode)
{
	m_TextColor1=TextColor1;
	m_TextColor2=TextColor2;
	m_TextGradientMode=TextGradientMode;

}
//�����ı��߿�
void CTextAeroWindow::SetTextStrBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pTextPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø����ı���ɫ
void CTextAeroWindow::SetHighlightColor(Gdiplus::Color TextColor1)
{
	CTextAeroWindow::SetHighlightColor(TextColor1,Gdiplus::Color::Black,GradientMode_None);
}
void CTextAeroWindow::SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,GradientMode TextGradientMode)
{
	m_HighlightColor1=TextColor1;
	m_HighlightColor2=TextColor2;
	m_HighlightGradientMode=TextGradientMode;

}
//���ø����ı��߿�
void CTextAeroWindow::SetHighlightBorder(Gdiplus::Color BorderColor,REAL BorderWidth)
{
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pHighlightPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//�����ı���Ӱ
void CTextAeroWindow::SetTextStrShadow(Gdiplus::Color ShadowColor,int nShadowOffset)
{
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(ShadowColor.GetA()>0 && nShadowOffset>0){
		m_nShadowOffset=nShadowOffset;
		m_pShadowBrush=new Gdiplus::SolidBrush(ShadowColor);
	}else{
		m_nShadowOffset=0;
	}
}
//�����ı�����
void CTextAeroWindow::SetTextStrFont(const WCHAR * familyName,REAL emSize,INT style,Unit unit)
{
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
	FontFamily family(familyName,NULL);
	Status lastResult = family.GetLastStatus();
	if (lastResult != Ok)
	{
		HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTW lf;
		ZeroMemory(&lf,sizeof(LOGFONTW));
		GetObjectW(hFont,sizeof(LOGFONTW),&lf);
		FontFamily family2(lf.lfFaceName,NULL);
		m_pFont=new Gdiplus::Font(&family2,emSize,style,unit);
	}else{
		m_pFont=new Gdiplus::Font(&family,emSize,style,unit);
	}
	 //----------------
	//����һЩ��������,����·��ʱҪ�õ�
	m_pFont->GetFamily (m_pFontFamily);
	m_FontSize=m_pFont->GetSize ();
	m_FontStyle=m_pFont->GetStyle ();

	
	
}

//���ö��뷽ʽ
void CTextAeroWindow::SetAlignmentMethod(IN StringAlignment align)
{
	m_pTextFormat->SetAlignment(align);
}
void CTextAeroWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bLock == TRUE)
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
		ReleaseCapture();
		SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,NULL);

	}

	//CWnd::OnLButtonDown(nFlags, point);
}

SizeF CTextAeroWindow::GetTextBounds( const Gdiplus::Font* font,const StringFormat& strFormat,const CString& szText )
{
	GraphicsPath path;
	FontFamily fontfamily;
	font->GetFamily(&fontfamily);
	path.AddString(szText,-1,&fontfamily,font->GetStyle(),font->GetSize(),PointF(0,0),&strFormat);
	RectF rcBound;
	path.GetBounds(&rcBound);
	return SizeF(rcBound.Width,rcBound.Height);
}

// void CTextAeroWindow::OnLButtonUp(UINT nFlags, CPoint point)
// {
// 
// 	CWnd::OnLButtonUp(nFlags, point);
// }

// void CTextAeroWindow::OnRButtonDown(UINT nFlags, CPoint point)
// {
// //  	DWORD dwPos = GetMessagePos();
// //  	CPoint _point(LOWORD(dwPos), HIWORD(dwPos));
// //  	CMenu menu;
// //  	VERIFY(menu.LoadMenu(IDR_MENU_LOCK));
// //  	CMenu* popup = menu.GetSubMenu(0);
// //  	ASSERT(popup != NULL);
// //  	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, _point.x, _point.y, this);
// 
// 	CWnd::OnRButtonDown(nFlags, point);
// }

void CTextAeroWindow::OnUpdateLockOsd(CCmdUI *pCmdUI)
{
 	if (m_bLock == TRUE)
 	{
		pCmdUI->SetCheck(1);

 	}
 	else
 	{
 		pCmdUI->SetCheck(0);
 	}
}

void CTextAeroWindow::OnUpdateUnlockOsd(CCmdUI *pCmdUI)
{
	if (m_bLock == TRUE)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CTextAeroWindow::OnLockOsd()
{
	m_bLock = TRUE;
}

void CTextAeroWindow::OnUnlockOsd()
{
	m_bLock = FALSE;
}

void CTextAeroWindow::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu; // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue; // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE); // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}



BOOL CTextAeroWindow::PreTranslateMessage(MSG* pMsg)
{
// 	if(::TranslateAccelerator(this->m_hWnd, hAccel, pMsg))
// 	{
// 		return TRUE;
// 	}
// 	if (WM_KEYFIRST<=pMsg->message&&pMsg->message<= WM_KEYLAST) 
// 	{ 
// 		HACCEL Accel = hAccel; 
// 		if (Accel && ::TranslateAccelerator(m_hWnd, Accel, pMsg)) 
// 		{
// 			return TRUE;
// 		}
// 	} 

 	CWnd *TmpWnd;
 	TmpWnd = FromHandle(this->m_hWnd);
 	TmpWnd->ShowWindow(SW_SHOW);
 
 	TmpWnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return CWnd::PreTranslateMessage(pMsg);

}

void CTextAeroWindow::OnMoving(UINT fwSide, LPRECT pRect)
{
	CWnd::OnMoving(fwSide, pRect);
	
	::SendMessage(g_OsdhWhd, MSG_MOVEPANEL, MSG_AUTOMOVE, 0);
}


void CTextAeroWindow::OnMouseMove(UINT nFlags, CPoint point)
{
//  	CRect rcClient;
//  	GetClientRect(rcClient);
//  	m_nWidth=rcClient.Width();
//  	m_nHeight=rcClient.Height();
//  
//  	//----------------------------------
//  	BITMAPINFO bitmapinfo;
//  	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//  	bitmapinfo.bmiHeader.biBitCount = 32;
//  	bitmapinfo.bmiHeader.biHeight = m_nHeight;
//  	bitmapinfo.bmiHeader.biWidth = m_nWidth;
//  	bitmapinfo.bmiHeader.biPlanes = 1;
//  	bitmapinfo.bmiHeader.biCompression=BI_RGB;
//  	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
//  	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
//  	bitmapinfo.bmiHeader.biClrUsed=0;
//  	bitmapinfo.bmiHeader.biClrImportant=0;
//  	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
//  	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
//  	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
//  	//----------------------------------
//  	Gdiplus::Graphics* pGraphics=new Gdiplus::Graphics(m_hCacheDC);
//  	pGraphics->SetSmoothingMode (SmoothingModeHighQuality);
//  	pGraphics->SetTextRenderingHint (TextRenderingHintClearTypeGridFit);
//  	DrawTextStr(pGraphics);
//  	delete pGraphics;
//  	//----------------------------------
//  	//����͸������
//  	CPoint DestPt(0,0);
//  	CSize psize(m_nWidth,m_nHeight);
//  	BLENDFUNCTION blendFunc32bpp;
//  	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
//  	blendFunc32bpp.BlendFlags = 0;
//  	blendFunc32bpp.BlendOp = AC_SRC_OVER;
//  	blendFunc32bpp.SourceConstantAlpha = 254;
//  	HDC hDC=::GetDC(m_hWnd);
//  
//  
//  	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_OPAQUE);//ULW_ALPHA   ULW_OPAQUE
//  
//  	BYTE byteAlpha=50;
//  	::SetLayeredWindowAttributes(m_hWnd, 0,byteAlpha,ULW_ALPHA);
//  
//  	//----------------------------------
//  	//�ͷ���Դ
//  	::SelectObject (m_hCacheDC,hOldBitmap);
//  	::DeleteObject(hBitmap);
//  	::ReleaseDC(m_hWnd,hDC);
	::SendMessage(g_OsdhWhd, MSG_MOVEPANEL, MSG_LOCKINFO, 0);
	CWnd::OnMouseMove(nFlags, point);
}