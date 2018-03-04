#include "Win32App.h"
#include <tchar.h>
#include "MemDC.h"
#include "WtlString.h"
#include <GdiPlus.h>
using namespace Gdiplus;
using namespace WTL;


#define WINDOW_CLASS    _T("Win32")
#define WINDOW_NAME     _T("GdiPlusString")
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480


CWin32App	*g_pApp=NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->WinProc(hWnd,msg,wParam,lParam);
}

CWin32App::CWin32App(void)
{
	m_hWnd = NULL;
	g_pApp= this;
}

CWin32App::~CWin32App(void)
{
}

BOOL CWin32App::Run()
{
	// Show the window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);

	// Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(5);
			RenderScene();
		}
	}
	Shutdown();
	return 0;
}

void CWin32App::Shutdown()
{

}

void CWin32App::RenderScene()
{
	
}

BOOL CWin32App::Create(HINSTANCE hinst)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	m_hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	BOOL bRet = IsWindow(m_hWnd);
	return bRet;
}

// ��ȡ�ļ�����
CString g_FileStrBuf;

HRESULT CWin32App::OnPaint(HWND hWnd,WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd,&ps);
	RECT rtClient={0};
	GetClientRect(hWnd,&rtClient);
	{
		CMemDC  memDc(hdc,rtClient);

		Graphics graphics(memDc);
		graphics.Clear(Color::Gray);

		if (g_FileStrBuf.IsEmpty())
		{
			DrawFontInfo(graphics);

			// �����ı�
			graphics.TranslateTransform(100,150);
			DrawMeasureString(graphics);
			graphics.ResetTransform();
		}
		else
		{
			DrawFenLan(hWnd, graphics);
		}
	}
	EndPaint(hWnd,&ps);
	return 0;
}


void CWin32App::DrawFenLan(HWND hWnd, Graphics &graphics)
{
	// ѡ��Ҫ������ʾ���ļ�
	// ������ʾ
	// ��������������֮��ļ��
	int  COLUMWIDTH = 100;
	int  SPACE = 10;	

	SolidBrush brush(Color(255,0,0,0));
	Font myFont(L"���� ",10);

	// �õ���ǰ���ڵ�����
	RECT rect;
	GetClientRect(hWnd,&rect);
	RectF boundRect;
	INT cocepointsFitted = 0;
	INT linesFilled = 0;

	//���������ʽ 
	StringFormat format;

	// �����Զ��ü�
	format.SetFormatFlags(StringFormatFlagsNoClip);

	// ����ı��Ե��ʽ���
	format.SetTrimming(StringTrimmingWord);

	// �������Ұ��У���������ı�
	CString outStr = g_FileStrBuf;
	for (int x=0; outStr.GetLength()>0 && x<rect.right;x+=(COLUMWIDTH+SPACE))
	{
		// �涨�ı��������һ����
		RectF rectf(x,0,COLUMWIDTH,rect.bottom-myFont.GetHeight(0.0f));

		// ����һ�����ܹ���ʾ�����ַ�
		graphics.MeasureString(outStr,-1,&myFont,rectf,&format,&boundRect,&cocepointsFitted,&linesFilled);

		// ��ʾһ���ı�
		graphics.DrawString(outStr,-1,&myFont,rectf,&format,&brush);
		// ȥ���Ѿ�������ַ����Ա���һ���ܹ�������ʾ
		outStr = outStr.Mid(cocepointsFitted);
	}
}


void CWin32App::DrawMeasureString(Graphics &graphics)
{
	wchar_t string[256]=L"123456789ABCDEFGHIJKLM";
	FontFamily fontFamily(L"Arial");
	// ����������С��ͬ��Arial����
	Font font(&fontFamily,30,FontStyleRegular,UnitPixel);
	Font font2(&fontFamily,18,FontStyleRegular,UnitPixel);
	// �����ı�������Σ���������ã�
	// �߶�Ϊ0��ʱ�򣬲Ż᷵������Ӧ�ľ��θ߶�
	RectF layoutRect(10,10,130,90);
	SolidBrush brush(Color(255,0,0,0));
	RectF boundRect;
	INT codepointsFitted=0;
	INT linesFilled=0;

	int strlen = wcslen(string);

	//  �����ı� 
	graphics.MeasureString(string,strlen,&font,layoutRect,NULL,
		&boundRect,&codepointsFitted,&linesFilled);

	CString tmp;
	CString s(string);
	tmp.Format(L"Ԥ������ַ���Ϊ\n\"%s\"\n��%d���ַ���\n���У��ڹ涨����������У�\nֻ�����%d�У���%d���ַ�",
				s,s.GetLength(),linesFilled,codepointsFitted);

	CString msg(tmp);
	// �����ı�������κ����ƾ���
	graphics.DrawRectangle(&Pen(Color(255,0,255,255),2),layoutRect);
	graphics.DrawRectangle(&Pen(Color(255,0,0,0)),boundRect);

	// ����ַ����Ĳ�����Ϣ
	graphics.DrawString(string,-1,&font,boundRect,NULL,&brush);
	graphics.DrawString(msg,-1,&font2,PointF(10,180),NULL,&brush);
}

void CWin32App::DrawFontInfo(Graphics &graphics)
{
	PointF pointF(0.0f,0.0f);
	SolidBrush solidBrush(Color(255,0,0,0));
	wchar_t infoSting[100]=L"";
	REAL FontHeightPixel;
	UINT ascent;
	REAL ascentPixel;
	UINT descent;
	REAL descentPixel;
	UINT lineSpacing;
	REAL lineSpacingPixel;

	FontFamily fontFamily(L"Arial");
	// ����һ����СΪ16���ص�Arial����
	Font font(&fontFamily,16,FontStyleRegular,UnitPixel);

	// ��ʾ�����С
	swprintf(infoSting,L"ʹ��font.GetSize()�����ķ���ֵΪ�� %.2f.",font.GetSize());
	graphics.DrawString(infoSting,wcslen(infoSting),&font,pointF,&solidBrush);
	// ����һ��
	pointF.Y += font.GetHeight(0.0f);
	// ��ʾ����߶ȣ�����
	swprintf(infoSting,L"ʹ��font.GetHeight()�����ķ���ֵΪ�� %.2f����",font.GetHeight(0.0));
	graphics.DrawString(infoSting,wcslen(infoSting),&font,pointF,&solidBrush);
	// ����һ��
	pointF.Y += font.GetHeight(0.0f);

	// ��ʾ����ϵ�еĸ߶ȣ���Ƶ�λ��
	swprintf(infoSting,L"ʹ��fontFamily.GetEmHeight()�����ķ���ֵΪ�� %d����Ƶ�λ��",fontFamily.GetEmHeight(FontStyleRegular));
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);


	// �������У�GetHeight���ص�������ĸ߶ȣ���λΪ���أ�
	pointF.Y +=2.0f*font.GetHeight(0.0f);

	// ��ȡ������ϲ�����
	ascent = fontFamily.GetCellAscent(FontStyleRegular);

	//���ϲ��������Ƶ�λת��Ϊ���ص�λ
	ascentPixel =font.GetSize()*ascent/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"�ϲ�����Ϊ%d����Ƶ�λ�� %.2f����",ascent,ascentPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);

	// ��ֱ��������һ��
	pointF.Y += font.GetHeight(0.0f);

	// ��ȡ������²����룬��Ƶ�λ
	descent = fontFamily.GetCellDescent(FontStyleRegular);

	// ���²��������Ƶ�λת��Ϊ���ص�λ
	descentPixel  = font.GetSize()*descent/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"�²�����Ϊ %d����Ƶ�λ��%.2f ���ء�",descent,descentPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);

	// ��ֱ��������һ��
	pointF.Y += font.GetHeight(0.0f);
	// ��ȡ�оࣨ��Ƶ�λ��
	lineSpacing = fontFamily.GetLineSpacing(FontStyleRegular);

	// ������Ƶ�λת�������ص�λ
	lineSpacingPixel = font.GetSize() * lineSpacing/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"�о�Ϊ %d����Ƶ�λ��%.2f ���ء�",lineSpacing,lineSpacingPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);
}

LRESULT WINAPI CWin32App::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_RBUTTONUP:
		{
			OPENFILENAME ofn;       // common dialog box structure
			TCHAR szFile[260];       // buffer for file name
			HANDLE hf;              // file handle

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = _T('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 
			if (GetOpenFileName(&ofn)==TRUE)
			{
				CString strOpenFileName;
				strOpenFileName = ofn.lpstrFile;
				// ���ļ������Ƿ���ȷ

				hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
					0, (LPSECURITY_ATTRIBUTES) NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
					(HANDLE) NULL);
				if (hf!=INVALID_HANDLE_VALUE)
				{
					DWORD dwSize =0;
					dwSize = GetFileSize(hf,NULL);

					DWORD dwBytesRead = 0;
					char *pReadBuffer = new char[dwSize];
					ReadFile(hf,pReadBuffer,dwSize,&dwBytesRead,NULL);
					g_FileStrBuf =pReadBuffer;
					CloseHandle(hf);
				}
			}
			::InvalidateRect(hWnd,NULL,false);
		}
		break;
	case WM_PAINT:
		OnPaint(hWnd,wParam,lParam);
		break;
	case WM_KEYUP:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}