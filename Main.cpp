#include <windows.h>

#define NEW_FILE_MENU_ID 1
#define OPEN_FILE_MENU_ID 2
#define EXIT_FILE_MENU_ID 3
#define CHANGE_TITLE_SEETINGS_MENU_ID 4

LRESULT CALLBACK myWindowProcedure(HWND, UINT, WPARAM, LPARAM);
void addMenus(HWND);
void addControls(HWND);

HMENU gMainWindowMenu;
HWND gEditText;

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
		addControls(windowHandle);
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
		case CHANGE_TITLE_SEETINGS_MENU_ID:
			wchar_t text[100]; // We're assumming user input won't exceed 100 characters
			GetWindowTextW(gEditText, text, 100);
			SetWindowTextW(windowHandle, text); // For the main window, setting text means setting the title
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

	HMENU settingsMenu = CreateMenu();
	AppendMenuW(settingsMenu, MF_STRING, CHANGE_TITLE_SEETINGS_MENU_ID, L"Change Window Title");
	AppendMenuW(fileMenu, MF_POPUP, (UINT_PTR) settingsMenu, L"Settings");
	
	AppendMenuW(fileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(fileMenu, MF_STRING, EXIT_FILE_MENU_ID, L"Exit");

	AppendMenuW(gMainWindowMenu, MF_POPUP, (UINT_PTR) fileMenu, L"File");
	AppendMenuW(gMainWindowMenu, MF_STRING, NULL, L"Help");

	SetMenu(windowHandle, gMainWindowMenu);
}

void addControls(HWND windowHandle)
{
	/*	In Win32 API, the controls are also considered as "Window" but these window are set up as the
		children of the main window (The actual window). These controls can be of 2 type.
		1. Static control: User can't change these. For example: Label
		2. Edit control: User can change these. For example: Text box
		The Classess for these controls are already defined in the API so we don't have to register them before
		their creation unlike main windows. They can be created just by calling the "CreateWinow()" function
	*/
	CreateWindowW(L"Static", L"Enter text here: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 100, 50, windowHandle, NULL, NULL, NULL);

	gEditText = CreateWindowW(L"Edit", L"Edit here", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 200, 152, 100, 50, windowHandle, NULL, NULL, NULL);
}
