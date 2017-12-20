#pragma once

#include<Windows.h>
#include<iostream>
#include<vector>

using namespace std;


struct ALLMONITORINFO
{
	HMONITOR hMonitor;
	RECT     rect;
	bool     isPrimary;
};

class DXWindow
{
public:
	DXWindow()
	{
		_hWnd = NULL;
		szTitle = L"��ʾ����";
		szWindowClass = L"oglWindow";
	}

	~DXWindow()
	{
		//Close();
	}

	HINSTANCE _hInst;					// ��ǰʵ��
	TCHAR* szTitle;					// �������ı�
	TCHAR* szWindowClass;			// ����������
							//���ɵĴ��ھ��
	HWND _hWnd;

	///-------------------------------------------------------------------------------------------------
	/// <summary> ����һ������. </summary>
	///
	/// <remarks> Xian.dai, 2017/3/23. </remarks>
	///
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	BOOL Create(WNDPROC proc)
	{
		if (_hWnd != NULL)//����Ѿ�������һ��������ô�����ظ�����
		{
			return FALSE;
		}

		//����ģ��ľ����������.exeģ��ľ������.dllģ��ľ����
		//����������ΪNULL���򷵻ص��øô��ڵ�.exeģ������
		//Ҳ�������������û�ȡģ����GetModuleHandle("mydll.dll")
		_hInst = GetModuleHandle(NULL);

		WNDCLASSEX wcex;
		memset(&wcex, 0, sizeof(wcex));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;

		//������ص�����
		if (proc != NULL)
			wcex.lpfnWndProc = proc;
		else
			wcex.lpfnWndProc = this->MyWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInst;

		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(107));
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN01);//��Ҫ�˵���
		wcex.lpszClassName = szWindowClass;//����������
										   //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

										   //ע�ᴰ����,����Ǳ���ģ�Ҫ��Ȼ�����CreateWindow�����ܷ���NULL
		//ATOM atom = RegisterClassEx(&wcex);
		RegisterClassEx(&wcex);

		//���е���ʾ����Ϣ��
		vector<ALLMONITORINFO> mInfo;
		//��ȡ��ʾ����Ϣ��
		mInfo.clear();
		//get number of monitors
		mInfo.reserve(GetSystemMetrics(SM_CMONITORS));

		EnumDisplayMonitors(NULL, NULL, this->MonitorEnumProc, reinterpret_cast<LPARAM>(&mInfo));

		//cout << "the number of monitors:" << mInfo.size() << endl;

		LONG width = 0,height = 0,startx = 0,starty = 0;


		if (mInfo.size() == 1)
		{
			RECT rect = mInfo[0].rect;
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
			startx = rect.left;
			starty = rect.top;
		}
		else
		{
			for (int i = 0; i < mInfo.size(); i++)
			{
				//��ȡ�ض���ʾ������Ϣ������Ҫ���Ƿֱ��ʣ���ʼ����ȡ�
				if (!mInfo[i].isPrimary)
				{
					RECT rect = mInfo[0].rect;
					width = rect.right - rect.left;
					height = rect.bottom - rect.top;
					startx = rect.left;
					starty = rect.top;
					break;
				}
			}
		}


		//��ȡ������Ϊ��λ����Ļ�ߴ硣
		//��ӡ�ó�ȷʵ�õ��˳�����
		//cout << "width:" << width << "  height:" << height << endl;

		_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			startx, starty, width, height, NULL, NULL, _hInst, NULL);

		if (!_hWnd)
		{
			return FALSE;
		}
		//ToggleFullScreen();

		ShowWindow(_hWnd, SW_SHOW);
		//ShowWindow(_hWnd, SW_MAXIMIZE);
		UpdateWindow(_hWnd);

		return TRUE;
	}

	void Close()
	{
		if (_hWnd)
		{
			//CloseWindow(_hWnd);
			DestroyWindow(_hWnd);
		}
		_hWnd = NULL;

	}

	//void ToggleFullScreen()
	//{
	//	ToggleFullScreen(width, height, startx, starty);
	//}

	//void ToggleFullScreen(int width, int height, int startx, int starty)
	//{
	//	if (_hWnd == NULL)
	//	{
	//		cout << "TXWindow.ChangeToFullScreenInRun():����_hWndΪ�գ� \r\n";
	//		return;
	//	}

	//	//��ȫ���˵���ȫ�������⡣
	//	if (isFullScreen)
	//	{

	//		SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE) | WS_OVERLAPPEDWINDOW);

	//		//GWL_EXSTYLE  extended style��չ��ʽ��WS_EX_WINDOWEDGE  ʹ�ô�����ʾ�����������������������
	//		SetWindowLong(_hWnd, GWL_EXSTYLE, GetWindowLong(_hWnd, GWL_EXSTYLE) | WS_EX_WINDOWEDGE);

	//		//�ڶ���ʾ���ϲ�֪���᲻�������⣬�Ƿ������������ʾ
	//		//SetWindowPos(_hWnd, NULL, startx, starty, width, height, SWP_SHOWWINDOW);// SWP_FRAMECHANGED);
	//	}
	//	else
	//	{
	//		LONG curWinStyle = GetWindowLong(_hWnd, GWL_STYLE);
	//		curWinStyle = curWinStyle & ~WS_OVERLAPPEDWINDOW;
	//		SetWindowLong(_hWnd, GWL_STYLE, curWinStyle | WS_POPUP);
	//		MoveWindow(_hWnd, startx, starty, width, height, TRUE);
	//	}
	//	isFullScreen = !isFullScreen;
	//}

	//RECT GetMonitorRECT(HMONITOR hMonitor)
	//{
	//	for (int i = 0; i < mInfo.size(); i++)
	//	{
	//		if (mInfo[i].hMonitor == hMonitor)
	//		{
	//			return mInfo[i].rect;
	//		}
	//	}
	//	return RECT();
	//}
private:

	///-------------------------------------------------------------------------------------------------
	/// <summary> һ��WndProc��Ϣ��������ģ��. </summary>
	/// <returns> A CALLBACK. </returns>
	///-------------------------------------------------------------------------------------------------
	static	LRESULT CALLBACK  MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		//int wmId, wmEvent;
		//dxlib::Debug::Log("WndProc():������Ϣ�������� message = %d \r\n", message);
		switch (message)
		{
		case WM_COMMAND:
			break;
		case WM_PAINT:
			//hdc = BeginPaint(hWnd, &ps);
			//// TODO:  �ڴ����������ͼ����...
			//EndPaint(hWnd, &ps);
			break;

		case WM_KEYDOWN:
			//����������ĳһ�����µ���Ϣ

			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	///----------------------------------------------------------------------------------------
	///��ȡ������ʾ������Ϣ���������Ǵ洢��vector�����ڡ�
	///�÷�������ΪEnumDisplayMonitors�Ļص���������vector����Ҳ���Դ���ʽ����ȥ��
	///
	///----------------------------------------------------------------------------------------
	static BOOL CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData)
	{
		vector<ALLMONITORINFO>& infoArray = *reinterpret_cast<vector<ALLMONITORINFO>* >(dwData);

		ALLMONITORINFO monitorInfo;
		monitorInfo.hMonitor = hMonitor;
		monitorInfo.rect = *lprcMonitor;

		HMONITOR priMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
		if (priMonitor == hMonitor)
			monitorInfo.isPrimary = true;
		else
			monitorInfo.isPrimary = false;

		infoArray.push_back(monitorInfo);
		return TRUE;
	}

};