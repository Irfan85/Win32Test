#include <windows.h>

#define NEW_FILE_MENU_ID 1
#define OPEN_FILE_MENU_ID 2
#define EXIT_FILE_MENU_ID 3

LRESULT CALLBACK myWindowProcedure(HWND, UINT, WPARAM, LPARAM);
void addMenus(HWND);
void addControls(HWND);

HMENU gMainWindowMenu;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	WNDCLASS wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hInstance = instance;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = myWindowProcedure;

	if (!RegisterClassW(&wc))
	{
		return -1;
	}

	CreateWindowW(L"myWindowClass", L"My Window Title", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessageW(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}

LRESULT CALLBACK myWindowProcedure(HWND windowHandle, UINT message, WPARAM wp, LPARAM lp)
{
	switch (message)
	{
	case WM_CREATE:
		addMenus(windowHandle);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case NEW_FILE_MENU_ID:
			MessageBeep(MB_OK);
			break;
		case OPEN_FILE_MENU_ID:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case EXIT_FILE_MENU_ID:
			DestroyWindow(windowHandle);
			break;
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(windowHandle, message, wp, lp);
	}
}

void addMenus(HWND windowHandle)
{
	gMainWindowMenu = CreateMenu();

	HMENU fileMenu = CreateMenu();
	AppendMenuW(fileMenu, MF_STRING, NEW_FILE_MENU_ID, L"New");
	AppendMenuW(fileMenu, MF_STRING, OPEN_FILE_MENU_ID, L"Open");
	AppendMenuW(fileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(fileMenu, MF_STRING, EXIT_FILE_MENU_ID, L"Exit");

	AppendMenuW(gMainWindowMenu, MF_POPUP, (UINT_PTR) fileMenu, L"File");
	AppendMenuW(gMainWindowMenu, MF_STRING, NULL, L"Help");

	SetMenu(windowHandle, gMainWindowMenu);
}

void addControls(HWND windowHandle)
{

}
