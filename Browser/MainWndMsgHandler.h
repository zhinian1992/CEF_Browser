#pragma once



class MainWndMsgHandler
{
public:
	MainWndMsgHandler();
	~MainWndMsgHandler();

	/* browser window message handler*/
	LRESULT MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	int CommandMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int ActivateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int SizeMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int PaintMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int CreateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int NccalcsizeMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int NCHitTestMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int DestroyMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateBrowserPage(HINSTANCE hInst,HWND hWnd);

	void PaintCustomCaption(HWND hWnd, HDC hdc);

	LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);
};

