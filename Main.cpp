#include <windows.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>

#define NEW_FILE_MENU_ID 1
#define OPEN_FILE_MENU_ID 2
#define EXIT_FILE_MENU_ID 3
#define GENERATE_BUTTON_CLICKED 4
#define DIALOG_CLOSE 5
#define SAVE_FILE_MENU_ID 6

LRESULT CALLBACK myWindowProcedure(HWND, UINT, WPARAM, LPARAM);
void addMenus(HWND);
void addControls(HWND);
void loadImages();
void registerDialogWindowClass(HINSTANCE);
void displayDialogWindow(HWND);
LRESULT CALLBACK dialogWindowProcedure(HWND, UINT, WPARAM, LPARAM);
void openFileDialog(HWND);
void displayFileContent(wchar_t*);
void saveFileDialog(HWND);
void writeToFile(wchar_t*);

HMENU gMainWindowMenu;
HWND gMainWindow, gNameEditText, gAgeEditText, gOutputEditText;
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

	registerDialogWindowClass(instance);

	gMainWindow = CreateWindowW(L"myWindowClass", L"My Window Title", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

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
			displayDialogWindow(windowHandle);
			MessageBeep(MB_OK);
			break;
		case OPEN_FILE_MENU_ID:
			openFileDialog(windowHandle);
			MessageBeep(MB_ICONINFORMATION);
			break;
		case SAVE_FILE_MENU_ID:
			saveFileDialog(windowHandle);
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
	AppendMenuW(fileMenu, MF_STRING, SAVE_FILE_MENU_ID, L"Save");

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

	gOutputEditText = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL| ES_MULTILINE, 100, 200, 300, 200, windowHandle, NULL, NULL, NULL);

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

void registerDialogWindowClass(HINSTANCE instance)
{
	WNDCLASSW dialogClass = { 0 };

	dialogClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialogClass.hCursor = LoadCursor(NULL, IDC_CROSS); // We'll use the cross cursor inside the dialog
	dialogClass.hInstance = instance;
	dialogClass.lpszClassName = L"MyDialogClass";
	dialogClass.lpfnWndProc = dialogWindowProcedure;

	RegisterClassW(&dialogClass);
}

void displayDialogWindow(HWND windowHandle)
{
	HWND dialogHandle = CreateWindowW(L"MyDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 200, 200, windowHandle, NULL, NULL, NULL);

	CreateWindowW(L"Button", L"Close", WS_VISIBLE | WS_CHILD | BS_TEXT, 20, 20, 100, 40, dialogHandle, (HMENU) DIALOG_CLOSE, NULL, NULL);

	// Disabling the main window will make our dialog a "Modal" dialog. Not doing so
	// will make our dialog a "Modeless" dialog
	EnableWindow(windowHandle, FALSE);
}

LRESULT CALLBACK dialogWindowProcedure(HWND windowHandle, UINT message, WPARAM wp, LPARAM lp)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case DIALOG_CLOSE:
			// Our dialog is modal so re-enable the main window after the dialog is closed
			EnableWindow(gMainWindow, TRUE);
			// Then destroy the dialog. Note: If we destroy the dialog before enabling the main window, windows
			// will enable the main window but will keep it in minimized mode for some reason
			DestroyWindow(windowHandle);
			break;
		}

		break;
	case WM_CLOSE:
		EnableWindow(gMainWindow, TRUE);
		DestroyWindow(windowHandle);
		break;
	default:
		return DefWindowProcW(windowHandle, message, wp, lp);
	}
}

void openFileDialog(HWND windowHandle)
{
	OPENFILENAME ofn; // This structure is needed to construct the open file dialog

	wchar_t fileName[100];
	// Initialize the first character of the string as null terminated character because we don't want
	// any files to be selected by default
	fileName[0] = '\0';

	ZeroMemory(&ofn, sizeof(OPENFILENAME)); // This Win32 macro will initialize the given memory addess of the given size with zeros

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = windowHandle;
	ofn.lpstrFile = fileName; // Store the fetched file's path in the "fileName" variable
	ofn.nMaxFile = 100; // We have choosen 100 as the length of "fileName"
	// Filter format: String+Null terminator(\0)+ Regex of file type + \0 and keep repeating this
	ofn.lpstrFilter = L"All files\0*.*\0Source files\0*.cpp\0Text files\0*.txt\0";
	ofn.nFilterIndex = 1; // Select the first element of filter list by default. Yes it starts with 1 instead of 0

	GetOpenFileNameW(&ofn);

	OutputDebugStringW(fileName); // Just for debug purpose

	displayFileContent(fileName);
}

void displayFileContent(wchar_t* fileName)
{
	FILE* file;
	// This is just the unicode compatible version of fopen_s. We'll read the file in binary mode to 
	// read it byte by byte
	_wfopen_s(&file, fileName, L"rb");

	if (file == NULL)
	{
		OutputDebugStringW(L"ERROR: Failed to load file.");
		return;
	}

	// Now we have to determine the size of the file
	// At first, we will seek the cursor to the end of the file
	fseek(file, 0, SEEK_END);
	// Then, we'll determine the current position of the cursor
	// This will give us the file size
	size_t fileSize = ftell(file);

	// Now lets get back to the beginning of the file in order to read the contents
	rewind(file);

	// C89, which is the current standard of MSVC doesn't support variables while declaring array
	// size. So, we have to take help from C++. We're adding an extra space for the NULL terminated character 
	// that we'll add at the end of the file ourselves to avoid any kind of unexpected behaviour
	wchar_t* fileData = new wchar_t[fileSize + 1];

	for(int c = 0; c < fileSize; c++)
	{
		// We're reading the file in ascii mode since wchar_t was acting wired while reading file. Maybe some encoding issue of UTF-8 and UTF-16
		// Anyways use of wchar_t is discouraged in mordern c/c++ versions
		char character = (char)fgetc(file);

		fileData[c] = character;
	}

	fclose(file);

	// Specify the last character of the file as the null terminator just to be sure and to avoid errors
	fileData[fileSize] = '\0';

	//OutputDebugStringW(fileData);
	SetWindowTextW(gOutputEditText, fileData);
}

void saveFileDialog(HWND windowHandle)
{
	OPENFILENAME ofn;

	wchar_t fileName[100];
	fileName[0] = '\0';

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = windowHandle;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = L"All files\0*.*\0Source files\0*.cpp\0Text files\0*.txt\0";
	ofn.nFilterIndex = 1;

	GetSaveFileNameW(&ofn);

	writeToFile(fileName);
}

void writeToFile(wchar_t* fileName)
{

	int dataSize = GetWindowTextLengthW(gOutputEditText);
	wchar_t* data = new wchar_t[dataSize + 1]; // Add 1 for the null terminator that the "GetWindowTextLengthW()" doesn't count
	GetWindowTextW(gOutputEditText, data, dataSize + 1);
	
	FILE* file;
	_wfopen_s(&file, fileName, L"w");

	if (file == NULL)
	{
		OutputDebugStringW(L"Failed to write fo file");
		return;
	}


	for (int c = 0; c < (dataSize + 1); c++)
	{
		char character = (char)data[c];

		// The app repeats newline characters.However the app ignores new line 
		// when it itself tries to display it. Again could but the encoding issue so better avoid wchar_t
		fputc(character, file);
		
	}

	fclose(file);
}

