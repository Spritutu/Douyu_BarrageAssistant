// �ӿ�˵��
// Create				�����ı����� 
// UpdateLyrics			�����ı�(�ı�,�������Ȱٷֱ�)
// Draw					�ػ��ı�����
// SetLyricsColor		�����ı���ɫ
// SetLyricsBorder		�����ı��߿�
// SetHighlightColor	���ø����ı���ɫ
// SetHighlightBorder	���ø����ı��߿�
// SetLyricsShadow		�����ı���Ӱ
// SetLyricsFont		�����ı�����
// ========================================================

#pragma once

#include <gdiplus.h>
#include "resource.h"
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus; 


//�ı�����ģʽ
enum GradientMode
{
	GradientMode_None		=0, //�޽���
	GradientMode_Two		=1, //��ɫ����
	GradientMode_Three	=2 //��ɫ����
};

// CTextAeroWindow

class CTextAeroWindow : public CWnd
{
	DECLARE_DYNAMIC(CTextAeroWindow)

public:
	CTextAeroWindow();
	virtual ~CTextAeroWindow();
public:
	//�������� 
	BOOL Create();
	BOOL Create(LPCTSTR lpszClassName);
	BOOL Create(LPCTSTR lpszClassName,int nWidth,int nHeight);
public:
	//�����ı�(�������Ȱٷֱ�)
	void UpdateTextStr(LPCSTR lpszText,float nHighlight);
	void UpdateTextStr(LPCWSTR lpszText,float nHighlight);
	//���¸�������(�������Ȱٷֱ�)
	void UpdateTextStr(float nHighlight);
	//�ػ��ı�����
	void Draw();
	//�����ı���ɫ
	void SetTextStrColor(Gdiplus::Color TextColor1);
	void SetTextStrColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,GradientMode TextGradientMode);
	//�����ı��߿�
	void SetTextStrBorder(Gdiplus::Color BorderColor,REAL BorderWidth = 1.0f);
	//���ø����ı���ɫ
	void SetHighlightColor(Gdiplus::Color TextColor1);
	void SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,GradientMode TextGradientMode);
	//���ø����ı��߿�
	void SetHighlightBorder(Gdiplus::Color BorderColor,REAL BorderWidth = 1.0f);
	//�����ı���Ӱ
	void SetTextStrShadow(Gdiplus::Color ShadowColor,int nShadowOffset=2);
	//�����ı�����
	void SetTextStrFont(const WCHAR * familyName,REAL emSize,INT style=FontStyleRegular,Unit unit=UnitPixel);
	//���ö��뷽ʽ
	void SetAlignmentMethod(IN StringAlignment align);
	//����ı���ȣ��߶�
	SizeF GetTextBounds(const Gdiplus::Font* font,const StringFormat& strFormat,const CString& szText);
	BOOL m_bLock;
private:
	//�����ı�
	void DrawTextStr(Gdiplus::Graphics* pGraphics);
	//���Ƹ����ı�
	void DrawHighlightTextStr(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath,RectF& dstRect);
	//�������仭ˢ
	Gdiplus::Brush* CreateGradientBrush(GradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2,RectF& dstRect);
	//ע�ᴰ����
	BOOL RegisterWndClass(LPCTSTR lpszClassName);
	
	//HACCEL   hAccel;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	HDC m_hCacheDC;//����DC
	int m_nWidth;
	int m_nHeight;
	LPWSTR m_lpszTextStr;//Unicode��ʽ���ı�
	float m_nHighlight;//�����ı��İٷֱ� 0--100
	Gdiplus::Color m_TextColor1;//��ͨ�ı���ɫ,ARGB��ɫ
	Gdiplus::Color m_TextColor2;//��ͨ�ı���ɫ,ARGB��ɫ
	GradientMode m_TextGradientMode;//��ͨ�ı�����ģʽ
	Gdiplus::Pen* m_pTextPen;//��ͨ�ı��߿򻭱�
	Gdiplus::Color m_HighlightColor1;//�����ı���ɫ,ARGB��ɫ
	Gdiplus::Color m_HighlightColor2;//�����ı���ɫ,ARGB��ɫ
	GradientMode m_HighlightGradientMode;//�����ı�����ģʽ
	Gdiplus::Pen*m_pHighlightPen;//�����ı��߿򻭱�
	Gdiplus::Brush* m_pShadowBrush;//��Ӱ��ˢ,GDIPlus��ˢ 
	int m_nShadowOffset;//��Ӱƫ��
	Gdiplus::Font* m_pFont;//GDIPlus����
	int m_FontStyle;
	REAL m_FontSize;
	Gdiplus::FontFamily* m_pFontFamily;
	Gdiplus::StringFormat* m_pTextFormat;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLockOsd();
	afx_msg void OnUnlockOsd();
	//afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateLockOsd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUnlockOsd(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);  
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


