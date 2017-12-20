#include"DXWindow.hpp"
#include"MyDebug.hpp"
#include"Debug.h"

DXWindow	dxWind;
HDC         imageDC;        // the DC to hold our image
HBITMAP     imageBmp;       // the actual bitmap which contains the image (will be put in the DC)
HBITMAP     imageBmpOld;    // the DC's old bitmap (for cleanup)
int			texture_w;
int			texture_h;
void*		texture_ptr;

void loadImage(const char* pathname);
void cleanUpImage();
void drawImage(HDC screen);

LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//dxlib::Debug::Log("WndProc():进入消息处理函数 message = %d \r\n", message);
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		drawImage(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//glContext.shutdown();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


extern "C" _declspec(dllexport) bool OpenDXWind(char* fileName, void* pTexture, int width, int height)
{
	texture_w = width;
	texture_h = height;
	texture_ptr = pTexture;

	dxlib::Debug::GetInst()->isLog = true; //设置是否输出日志
	dxlib::Debug::GetInst()->CreatLogFile("another.txt");

	imageDC = CreateCompatibleDC(NULL);     // create an offscreen DC
	//loadImage("aa.bmp");
	loadImage(fileName);

	//FileDebug::Log("OpenDXWind was called");
	BOOL result = dxWind.Create(WndProc);
	if (result == FALSE)
		return false;
	return true;
}

extern "C" _declspec(dllexport) bool CloseDXWind()
{
	dxWind.Close();
	//InitD3DDevice();
	cleanUpImage();
	return true;
}

extern "C" _declspec(dllexport) bool SetTexture(void* pTexture)
{
	texture_ptr = pTexture;
	loadImage(NULL);
	return true;
}

// Function to load the image into our DC so we can draw it to the screen
void loadImage(const char* pathname)
{
	imageBmp = CreateBitmap(texture_w, texture_h, 1, 32, texture_ptr);

	//imageBmp = (HBITMAP)LoadImage(         // load the bitmap from a file
	//	NULL,                           // not loading from a module, so this is NULL
	//	(LPCWSTR)pathname,                       // the path we're loading from
	//	IMAGE_BITMAP,                   // we are loading a bitmap
	//	0, 0,                            // don't need to specify width/height
	//	LR_DEFAULTSIZE | LR_LOADFROMFILE// use the default bitmap size (whatever the file is), and load it from a file
	//);
	if (imageBmp)
		FileDebug::Log("File exist");
	else
		FileDebug::Log("File nonexist");
	imageBmpOld = (HBITMAP)SelectObject(imageDC, imageBmp);  // put the loaded image into our DC
}

// Function to clean up
void cleanUpImage()
{
	SelectObject(imageDC, imageBmpOld);      // put the old bmp back in our DC
	DeleteObject(imageBmp);                 // delete the bmp we loaded
	DeleteDC(imageDC);                      // delete the DC we created
}

// The function to draw our image to the display (the given DC is the screen DC)
void drawImage(HDC screen)
{
	BitBlt(
		screen,         // tell it we want to draw to the screen
		0, 0,            // as position 0,0 (upper-left corner)
		texture_w,   // width of the rect to draw
		texture_h,   // height of the rect
		imageDC,        // the DC to get the rect from (our image DC)
		0, 0,            // take it from position 0,0 in the image DC
		SRCCOPY         // tell it to do a pixel-by-pixel copy
	);
	DWORD key = GetLastError();
	dxlib::Debug::Log("BitBlt Error = %d \r\n", key);
}