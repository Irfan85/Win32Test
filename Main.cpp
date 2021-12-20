#include <windows.h>
#include <string.h>
#include <wchar.h>

#define NEW_FILE_MENU_ID 1
#define OPEN_FILE_MENU_ID 2
#define EXIT_FILE_MENU_ID 3
#define GENERATE_BUTTON_CLICKED 4

LRESULT CALLBACK myWindowProcedure(HWND, UINT, WPARAM, LPARAM);
void addMenus(HWND);
void addControls(HWND);
void loadImages();

HMENU gMainWindowMenu;
HWND gNameEditText, gAgeEditText, gOutputEditText;
HBITMAP gIconImage, gGenerateImage;

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
		loadImages();
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
		case GENERATE_BUTTON_CLICKED:
			// Using ASCII functions provided in c "string.h" header file
			/*char name[30], age[30], output[50];
			GetWindowTextA(gNameEditText, name, 30);
			GetWindowTextA(gAgeEditText, age, 30);

			strcpy_s(output, name);
			strcat_s(output, " is ");
			strcat_s(output, age);
			strcat_s(output, " years old.");

			SetWindowTextA(gOutputEditText, output);*/

			// Using UNICODE functions provided in c "wchar.h" header file
			wchar_t name[30], age[30], output[50];
			GetWindowTextW(gNameEditText, name, 30);
			GetWindowTextW(gAgeEditText, age, 30);

			if (wcscmp(name, L"") == 0 || wcscmp(age, L"") == 0)
			{
				int userResposne = MessageBoxW(windowHandle, L"Input can't be empty", L"Input Error", MB_ABORTRETRYIGNORE | MB_ICONERROR);
				
				switch (userResposne)
				{
				case IDABORT:
					DestroyWindow(windowHandle);
					break;
				case IDRETRY:
					return 0; // Just get out of the procedure function and do nothing
				case IDIGNORE:
					break; // User wants to proceed with empty data so get out of the switch statement and continue operatoin
				}
			}

			wcscpy_s(output, name);
			wcscat_s(output, L" is ");
			wcscat_s(output, age);
			wcscat_s(output, L" years old.");

			SetWindowTextW(gOutputEditText, output);

			break;
		case EXIT_FILE_MENU_ID:
			int userResponse = MessageBoxW(windowHandle, L"Do you really want to close the application?", L"Are you sure?", MB_YESNO | MB_ICONEXCLAMATION);
			if (userResponse == IDYES)
			{
				DestroyWindow(windowHandle);
			}
			
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
	AppendMenuW(settingsMenu, MF_STRING, NULL, L"Change Window Title");
	AppendMenuW(fileMenu, MF_POPUP, (UINT_PTR) settingsMenu, L"Settings");
	
	AppendMenuW(fileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(fileMenu, MF_STRING, EXIT_FILE_MENU_ID, L"Exit");

	AppendMenuW(gMainWindowMenu, MF_POPUP, (UINT_PTR) fileMenu, L"File");
	AppendMenuW(gMainWindowMenu, MF_STRING, NULL, L"Help");

	SetMenu(windowHandle, gMainWindowMenu);
}

void addControls(HWND windowHandle)
{
	CreateWindowW(L"Static", L"Name :", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, windowHandle, NULL, NULL, NULL);
	gNameEditText = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, windowHandle, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Age :", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, windowHandle, NULL, NULL, NULL);
	gAgeEditText = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, windowHandle, NULL, NULL, NULL);

	HWND generateButton = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_BITMAP, 150, 140, 98, 38, windowHandle, (HMENU) GENERATE_BUTTON_CLICKED, NULL, NULL);
	SendMessageW(generateButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)gGenerateImage);

	gOutputEditText = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 200, 300, 200, windowHandle, NULL, NULL, NULL);

	HWND iconImageWindow = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD | SS_BITMAP, 350, 60, 100, 100, windowHandle,  NULL, NULL, NULL);
	// We need to send a message to the image window in order to set an image to it
	SendMessageW(iconImageWindow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)gIconImage);
}

void loadImages()
{
	// We will scale this image to 100X100 pixels. Use 0 in these places to keep the default image size
	gIconImage = (HBITMAP)LoadImageW(NULL, L"icon.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);

	gGenerateImage = (HBITMAP)LoadImage(NULL, L"button.bmp", IMAGE_BITMAP, 98, 38, LR_LOADFROMFILE);
}