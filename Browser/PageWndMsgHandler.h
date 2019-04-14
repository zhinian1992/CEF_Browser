#pragma once

class PageWndMsgHandler
{
public:
	PageWndMsgHandler(HWND parientHWnd, std::string sURL);
	~PageWndMsgHandler();

	LRESULT PageWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void SizeMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void PaintMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void DestroyMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateBrowserPage(HWND hWnd);

private:
	static PageWndMsgHandler *m_MsgHandler;

	HWND m_hWnd;

	HWND m_ParientHWnd;

	int m_Width;

	int m_Height;

	std::string m_URL;
};

