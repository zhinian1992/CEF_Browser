#pragma once

class SinglePageWnd
{
public:
	SinglePageWnd(HINSTANCE hInstance,HWND parientHWnd,int iWidth,int iHeight,std::string sURL);
	~SinglePageWnd();

	void CreateBrowserPage();

	void WndSizeChanged(RECT rect,WPARAM wParam);

private:
	void MyRegisterClass();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static SinglePageWnd *m_staticSPW;

	HINSTANCE m_hIns;

	HWND m_hWnd;

	HWND m_ParientHWnd;

	int m_Width;

	int m_Height;

	std::string m_URL;
};

