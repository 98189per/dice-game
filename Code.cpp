#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define ID_PLAYER1SCORE 1
#define ID_PLAYER2SCORE 2
#define ID_PLAYER1ROLL 3
#define ID_PLAYER1END 4
#define ID_PLAYER2ROLL 5
#define ID_PLAYER2END 6
#define ID_TEXTBOX 7


using namespace std;

static LPCSTR szWindowClass = "AppTitle";

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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szWindowClass;
	wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBoxW(NULL,
			L"Error message",
			L"MessageBox message",
			0LL);

		return 1;
	}

	HINSTANCE hIsnt = hInstance;

	HWND hwnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		920, 640,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd) {
		MessageBoxW(NULL,
			L"Error message",
			L"MessageBox message",
			0LL);

		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

HWND hwnd1score;
HWND hwnd2score;
HWND hwnd1roll;
HWND hwnd1end;
HWND hwnd2roll;
HWND hwnd2end;
HWND hwndText;
RECT rc;
int playerTurn = 1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_CREATE: 
		hwnd1score = CreateWindowW(
			L"STATIC", L"Player 1",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
			50, 50, 350, 100,
			hwnd,
			(HMENU)ID_PLAYER1SCORE,
			NULL,
			NULL);
		hwnd2score = CreateWindowW(
			L"STATIC", L"Player 2",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
			500, 50, 350, 100,
			hwnd,
			(HMENU)ID_PLAYER2SCORE,
			NULL,
			NULL);
		hwnd1roll = CreateWindowW(
			L"BUTTON", L"Roll die / Flip coin",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
			50, 200, 150, 100,
			hwnd,
			(HMENU)ID_PLAYER1ROLL,
			NULL,
			NULL);
		hwnd1end = CreateWindowW(
			L"BUTTON", L"End turn",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
			250, 200, 150, 100,
			hwnd,
			(HMENU)ID_PLAYER1END,
			NULL,
			NULL);
		hwnd2roll = CreateWindowW(
			L"BUTTON", L"Roll die / Flip coin",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
			500, 200, 150, 100,
			hwnd,
			(HMENU)ID_PLAYER2ROLL,
			NULL,
			NULL);
		hwnd2end = CreateWindowW(
			L"BUTTON", L"End turn",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
			700, 200, 150, 100,
			hwnd,
			(HMENU)ID_PLAYER2END,
			NULL,
			NULL);
		hwndText = CreateWindowW(
			L"STATIC", L"Game text",
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | SS_CENTERIMAGE,
			50, 350, 800, 200,
			hwnd,
			(HMENU)ID_TEXTBOX,
			NULL,
			NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		case ID_PLAYER1END:
			if (playerTurn == 1) {
				playerTurn++;
				MessageBoxW(hwnd, L"clicked 1", 0LL, 0LL);
			}
			break;
		case ID_PLAYER2END:
			if (playerTurn == 2) {
				playerTurn--;
				MessageBoxW(hwnd, L"clicked 2", 0LL, 0LL);
			}
			break;
		case ID_PLAYER1ROLL:
			break;
		case ID_PLAYER2ROLL:
			break;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
		break;
	/*case WM_ERASEBKGND:
		hdc = (HDC)wParam;
		SetRect(&rc, 50, 350, 800, 200);
		FillRect(hdc, &rc, CreateSolidBrush(RGB(0, 255, 0)));
		break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}

	return 0;
}