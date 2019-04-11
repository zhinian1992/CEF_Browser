#pragma once



class MainWndMsgHandler
{
public:
	MainWndMsgHandler();
	~MainWndMsgHandler();

	/* browser window message handler*/
	LRESULT CALLBACK MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	bool CommandMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool ActivateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool SizeMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool PaintMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool CreateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool DestroyMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateBrowser(HWND hWnd);
};

