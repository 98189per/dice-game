#define NOMINMAX

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#include <algorithm>
#include <sstream>

#define ID_PLAYER1SCORE 1
#define ID_PLAYER2SCORE 2
#define ID_PLAYER1ROLL 3
#define ID_PLAYER1END 4
#define ID_PLAYER2ROLL 5
#define ID_PLAYER2END 6
#define ID_TEXTBOX 7
#define IDM_GAME_RESTART 8
#define IDM_CONFIGURE_MENU 9
#define IDM_HELP_MENU 10
#define IDM_GAME_EXIT 11


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

	srand(time(NULL));

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void AddMenus(HWND hwnd) {

	HMENU hmenubar;
	HMENU hgame;
	HMENU hconfigure;
	HMENU hhelp;

	hmenubar = CreateMenu();
	hgame = CreateMenu();
	hconfigure = CreateMenu();
	hhelp = CreateMenu();

	AppendMenuW(hgame, MF_STRING, IDM_GAME_RESTART, L"Restart");
	AppendMenuW(hgame, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hgame, MF_STRING, IDM_GAME_EXIT, L"Exit");
	AppendMenuW(hconfigure, MF_STRING, IDM_CONFIGURE_MENU, L"Configure");
	AppendMenuW(hhelp, MF_STRING, IDM_HELP_MENU, L"Help");

	AppendMenuW(hmenubar, MF_POPUP, (UINT_PTR)hgame, L"Game");
	AppendMenuW(hmenubar, MF_POPUP, (UINT_PTR)hconfigure, L"Configure");
	AppendMenuW(hmenubar, MF_POPUP, (UINT_PTR)hhelp, L"Help");
	SetMenu(hwnd, hmenubar);
}

HWND hwnd1score;
HWND hwnd2score;
HWND hwnd1roll;
HWND hwnd1end;
HWND hwnd2roll;
HWND hwnd2end;
HWND hwndText;
RECT rc;
char text[100];
int playerTurn = 1;
static int player1score = 0;
static int player2score = 0;

class Data {
public:
	//int player1score = 0;
	//int player2score = 0;
	int diceRoll1, diceRoll2, coinFlip;
	void reset() {
		player1score = 0;
		player2score = 0;
	}
	int Roll(int playerTag) {
		diceRoll1 = rand() % 6 + 1;
		diceRoll2 = rand() % 6 + 1;
		coinFlip = rand() % 2 + 1;

		if (diceRoll1 % 2 == 0 && diceRoll2 % 2 == 0) {
			if (coinFlip == 1) {
				if (playerTag == 1)
					player1score += diceRoll1 + diceRoll2;
				else
					player2score += diceRoll1 + diceRoll2;
				return diceRoll1 + diceRoll2;
			}
			else {
				if (playerTag == 1)
					player1score += diceRoll1 * diceRoll2;
				else
					player2score += diceRoll1 * diceRoll2;
				return diceRoll1 * diceRoll2;
			}
		}
		else if (diceRoll1 % 2 == 1 && diceRoll2 % 2 == 1) {
			if (coinFlip == 1) {
				if (playerTag == 1)
					player1score += 2 * std::max(diceRoll1, diceRoll2);
				else
					player2score += 2 * std::max(diceRoll1, diceRoll2);
				return 2 * std::max(diceRoll1, diceRoll2);
			}
			else {
				if (playerTag == 1)
					player1score += 2 * std::min(diceRoll1, diceRoll2);
				else
					player2score += 2 * std::min(diceRoll1, diceRoll2);
				return 2 * std::min(diceRoll1, diceRoll2);
			}
		}
		else {
			if (coinFlip == 1) {
				if (playerTag == 1)
					player1score += 2 * (diceRoll1 + diceRoll2);
				else
					player2score += 2 * (diceRoll1 + diceRoll2);
				return 2 * (diceRoll1 + diceRoll2);
			}
			else {
				if (playerTag == 1)
					player1score += 2 * (diceRoll1 * diceRoll2);
				else
					player2score += 2 * (diceRoll1 * diceRoll2);
				return 2 * (diceRoll1 * diceRoll2);
			}
		}
	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	Data data;

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
		AddMenus(hwnd);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_PLAYER1END:
			if (playerTurn == 1) {
				playerTurn++;
				MessageBoxW(hwnd, L"It's now Player 2's turn", L"Change turn", 0LL);
			}
			break;
		case ID_PLAYER2END:
			if (playerTurn == 2) {
				playerTurn--;
				MessageBoxW(hwnd, L"It's now Player 1's turn", L"Change turn", 0LL);
			}
			break;
		case ID_PLAYER1ROLL:
			if (playerTurn == 1) {
				char roll[10], die1[10], die2[10], coin[10], score[10];
				wchar_t temp[100];
				sprintf(roll, "%d", data.Roll(playerTurn));
				sprintf(die1, "%d", data.diceRoll1);
				sprintf(die2, "%d", data.diceRoll2);
				sprintf(score, "%d", player1score);
				if (data.coinFlip == 1)
					strcpy(coin, "Heads");
				else
					strcpy(coin, "Tails");
				strcpy(text, "Player 1 got ");
				strcat(text, roll);
				strcat(text, " because they rolled a ");
				strcat(text, die1);
				strcat(text, " and ");
				strcat(text, die2);
				strcat(text, " and flipped ");
				strcat(text, coin);
				std::mbstowcs(temp, text, strlen(text) + 1);
				SetWindowTextW(hwndText, temp);
				strcpy(text, "Player 1 - Score: ");
				strcat(text, score);
				std::mbstowcs(temp, text, strlen(text) + 1);
				SetWindowTextW(hwnd1score, temp);
				if (player1score > 50) {
					MessageBoxW(hwnd, L"Uh oh! Player 1 has bust, Player 2 wins this round", L"Next round", 0LL);
					SetWindowTextW(hwnd1score, L"Player 1");
					SetWindowTextW(hwnd2score, L"Player 2");
					SetWindowTextW(hwndText, L"Game text");
					playerTurn = 1;
					data.reset();
				}
			}
			break;
		case ID_PLAYER2ROLL:
			if (playerTurn == 2) {
				char roll[10], die1[10], die2[10], coin[10], score[10];
				wchar_t temp[100];
				sprintf(roll, "%d", data.Roll(playerTurn));
				sprintf(die1, "%d", data.diceRoll1);
				sprintf(die2, "%d", data.diceRoll2);
				sprintf(score, "%d", player2score);
				if (data.coinFlip == 1)
					strcpy(coin, "Heads");
				else
					strcpy(coin, "Tails");
				strcpy(text, "Player 2 got ");
				strcat(text, roll);
				strcat(text, " because they rolled a ");
				strcat(text, die1);
				strcat(text, " and ");
				strcat(text, die2);
				strcat(text, " and flipped ");
				strcat(text, coin);
				std::mbstowcs(temp, text, strlen(text) + 1);
				SetWindowTextW(hwndText, temp);
				strcpy(text, "Player 2 - Score: ");
				strcat(text, score);
				std::mbstowcs(temp, text, strlen(text) + 1);
				SetWindowTextW(hwnd2score, temp);
				if (player2score > 50) {
					MessageBoxW(hwnd, L"Uh oh! Player 2 has bust, Player 1 wins this round", L"Next round", 0LL);
					SetWindowTextW(hwnd1score, L"Player 1");
					SetWindowTextW(hwnd2score, L"Player 2");
					SetWindowTextW(hwndText, L"Game text");
					playerTurn = 1;
					data.reset();
				}
			}
			break;
		case IDM_GAME_RESTART:
			SetWindowTextW(hwnd1score, L"Player 1");
			SetWindowTextW(hwnd2score, L"Player 2");
			SetWindowTextW(hwndText, L"Game text");
			playerTurn = 1;
			data.reset();
			break;
		case IDM_GAME_EXIT:
			PostQuitMessage(0);
			break;
		case IDM_CONFIGURE_MENU:
			MessageBoxW(hwnd, L"Not yet coded...", 0LL, 0LL);
			break;
		case IDM_HELP_MENU:
			switch (rand() % 5 + 1) {
			case 1:
				MessageBoxW(hwnd, L"Zzz... Zzz...", L"Personal Assistant", 0LL);
				break;
			case 2:
				MessageBoxW(hwnd, L"Figure it out yourself!!!", L"Personal Assistant", 0LL);
				break;
			case 3:
				MessageBoxW(hwnd, L"I'm going to sleep...", L"Personal Assistant", 0LL);
				break;
			case 4:
				MessageBoxW(hwnd, L"It's really not that difficult...", L"Personal Assistant", 0LL);
				break;
			case 5:
				MessageBoxW(hwnd, L"Just Google it or something!!!", L"Personal Assistant", 0LL);
				break;
			}
		}
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