#pragma once



class MainWndMsgHandler
{
public:
	MainWndMsgHandler();
	~MainWndMsgHandler();

	/* browser window message handler*/
	LRESULT CALLBACK MainWndProc(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void CommandMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void ActivateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SizeMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void PaintMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void DestroyMsgHandler(HINSTANCE hInst,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CreateNewPage(HINSTANCE hInst,HWND hWnd);
};

