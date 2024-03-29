#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define ID_COMBOBOX 1

using namespace std;

static LPCSTR szWindowClass = "DiceGame";

static LPCSTR szTitle = "Dice Game";

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBoxW(NULL,
			L"Call to RegisterClassEx Failed!",
			L"Bruh I guess this broke?",
			0LL);

		return 1;
	}

	HINSTANCE hIsnt = hInstance;

	HWND hwnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	if (!hwnd) {
		MessageBoxW(NULL,
			L"Call to RegisterClassEx Failed!",
			L"Bruh I guess this broke?",
			0LL);

		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	HWND hwndComboBox = CreateWindowW(
		L"COMBOBOX", L"",
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		100, 100, 100, 10,
		hwnd, (HMENU)ID_COMBOBOX, NULL, NULL
	);

	wchar_t options[9][10] = {
		L"2", L"3", L"4", L"5", L"6",
		L"7", L"8", L"9", L"10"
	};

	wchar_t A[16];
	int k = 0;

	memset(&A, 0, sizeof(A));
	for (k = 0; k <= 8; k++) {
		wcscpy(A, (wchar_t*)options[k]);
		SendMessageW(hwndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	SendMessage(hwndComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	LRESULT result = 0;

	bool wasHandled = false;

	switch (message) {
	case WM_CREATE:
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			MessageBox(hwnd, (LPCSTR)ListItem, TEXT("Item Selected"), MB_OK);
		}
		wasHandled = true;
		result = 0;
		break;
	case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
		wasHandled = true;
		result = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return result;
}