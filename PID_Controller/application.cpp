#include "application.h"
#include <CommCtrl.h>
#include <string>

#define BUTTONID_SET 200
#define BUTTONID_RESTART 201

static const TCHAR g_ClassName[] = TEXT("PID_Controller");
static const TCHAR g_CanvasClass[] = TEXT("CanvasClass");
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ((Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->MessageHandler(hwnd, msg, wparam, lparam);
}
static LRESULT CALLBACK InitialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((CREATESTRUCT*)lparam)->lpCreateParams));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
		return WndProc(hwnd, msg, wparam, lparam);
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
static LRESULT CALLBACK CanvasWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ((Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->CanvasMessageHandler(hwnd, msg, wparam, lparam);
}
static LRESULT CALLBACK InitialCanvasWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((CREATESTRUCT*)lparam)->lpCreateParams));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)CanvasWndProc);
		return CanvasWndProc(hwnd, msg, wparam, lparam);
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Application::CreateUI()
{
	int padding = 4, x1 = 640 + padding, w = (200 - 3 * padding) / 2, x2 = x1 + w + padding, y = padding, h = 24;
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = InitialCanvasWndProc;
	wc.lpszClassName = g_CanvasClass;
	RegisterClassEx(&wc);
	m_canvas = CreateWindowEx(0, g_CanvasClass, TEXT("canvas"), WS_CHILD | WS_VISIBLE, 0, 0, 640, 480, m_hwnd, NULL, NULL, (LPVOID)this);

	lbl_controller = CreateWindowEx(0, WC_STATIC, TEXT("Controller"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	cb_controller = CreateWindowEx(0, WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE, x2, y, w, 200, m_hwnd, NULL, NULL, NULL);
	SendMessage(cb_controller, CB_ADDSTRING, 0, (LPARAM)TEXT("P"));
	SendMessage(cb_controller, CB_ADDSTRING, 0, (LPARAM)TEXT("PI"));
	SendMessage(cb_controller, CB_ADDSTRING, 0, (LPARAM)TEXT("PD"));
	SendMessage(cb_controller, CB_ADDSTRING, 0, (LPARAM)TEXT("PID"));
	SendMessage(cb_controller, CB_SETCURSEL, 0, 0);
	y += h + padding;

	lbl_kp = CreateWindowEx(0, WC_STATIC, TEXT("Kp"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_kp = CreateWindowEx(0, WC_EDIT, TEXT("1.0"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_ki = CreateWindowEx(0, WC_STATIC, TEXT("Ki"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_ki = CreateWindowEx(0, WC_EDIT, TEXT("1.0"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_kd = CreateWindowEx(0, WC_STATIC, TEXT("Kd"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_kd = CreateWindowEx(0, WC_EDIT, TEXT("1.0"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_friction = CreateWindowEx(0, WC_STATIC, TEXT("Friction"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_friction = CreateWindowEx(0, WC_EDIT, TEXT("0.1"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_gravity = CreateWindowEx(0, WC_STATIC, TEXT("Gravity"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_gravity = CreateWindowEx(0, WC_EDIT, TEXT("0.1"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_outputLimit = CreateWindowEx(0, WC_STATIC, TEXT("Output limit"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_outputLimit = CreateWindowEx(0, WC_EDIT, TEXT("10.0"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	lbl_integralLimit = CreateWindowEx(0, WC_STATIC, TEXT("Integral limit"), SS_LEFT | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, NULL, NULL, NULL);
	tb_integralLimit = CreateWindowEx(0, WC_EDIT, TEXT("20.0"), ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP, x2, y, w, h, m_hwnd, NULL, NULL, NULL);
	y += h + padding;

	btn_set = CreateWindowEx(0, WC_BUTTON, TEXT("Set"), BS_BOTTOM | WS_CHILD | WS_VISIBLE, x1, y, w, h, m_hwnd, (HMENU)BUTTONID_SET, NULL, NULL);
	btn_restart = CreateWindowEx(0, WC_BUTTON, TEXT("Restart"), BS_BOTTOM | WS_CHILD | WS_VISIBLE, x2, y, w, h, m_hwnd, (HMENU)BUTTONID_RESTART, NULL, NULL);
}

void Application::UIEvent(WPARAM wparam)
{
	switch (wparam)
	{
	case BUTTONID_SET:
		SetMotorParams();
		break;
	case BUTTONID_RESTART:
		RestartMotor();
		break;
	}
}

void Application::SetMotorParams()
{
	char controller[32]{};
	double kp;
	double ki;
	double kd;
	double friction;
	double gravity;
	double outputLimit;
	double integralLimit;
	TCHAR text[32];

	GetWindowText(cb_controller, text, 32);
	for (int i = 0; text[i]; i++)
		controller[i] = (char)text[i];

	GetWindowText(tb_kp, text, 32);
	kp = std::wcstod(text, NULL);

	GetWindowText(tb_ki, text, 32);
	ki = std::wcstod(text, NULL);

	GetWindowText(tb_kd, text, 32);
	kd = std::wcstod(text, NULL);

	GetWindowText(tb_friction, text, 32);
	friction = std::wcstod(text, NULL);

	GetWindowText(tb_gravity, text, 32);
	gravity = std::wcstod(text, NULL);

	GetWindowText(tb_outputLimit, text, 32);
	outputLimit = std::wcstod(text, NULL);

	GetWindowText(tb_integralLimit, text, 32);
	integralLimit = std::wcstod(text, NULL);

	m_motor.SetParams(controller, kp, ki, kd, friction, gravity, outputLimit, integralLimit);
	m_stepResponseGraph.SetMotorParams(controller, kp, ki, kd, friction, gravity, outputLimit, integralLimit);
}

void Application::RestartMotor()
{
	m_graph.Clear();
	m_motor.Restart();
}

void Application::LMouseButtonDown(int x, int y)
{
	double target = std::atan2(x - m_circleX, y - m_circleY);
	m_motor.setTargetPosition(target);
	m_stepResponseGraph.SetMotorTargetPosition(target);
}

void Application::PaintCanvas()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_canvas, &ps);
	HDC hmem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, 640, 480);
	SelectObject(hmem, hbmp);

	PatBlt(hmem, 0, 0, 640, 480, WHITENESS);
	PaintCanvas(hmem);

	BitBlt(hdc, 0, 0, 640, 480, hmem, 0, 0, SRCCOPY);
	DeleteObject(hbmp);
	DeleteDC(hmem);
	EndPaint(m_canvas, &ps);
}

void Application::PaintCanvas(HDC hdc)
{
	m_stepResponseGraph.DrawGraph(hdc, 200, 0, 440, 200);
	DrawMotor(hdc);
	DrawGraph(hdc);
}

void Application::DrawMotor(HDC hdc)
{
	SelectObject(hdc, m_blueBrush);
	SelectObject(hdc, m_bluePen);
	Ellipse(hdc, (int)(m_circleX - m_circleR), (int)(m_circleY - m_circleR), (int)(m_circleX + m_circleR), (int)(m_circleY + m_circleR));
	SelectObject(hdc, m_greenPen);
	MoveToEx(hdc, (int)(m_circleX), (int)(m_circleY), NULL);
	LineTo(hdc, (int)(sin(m_motor.getTargetPosition()) * m_leverL + m_circleX), (int)(cos(m_motor.getTargetPosition()) * m_leverL + m_circleY));
	SelectObject(hdc, m_redPen);
	MoveToEx(hdc, (int)(m_circleX), (int)(m_circleY), NULL);
	LineTo(hdc, (int)(sin(m_motor.getCurrentPosition()) * m_leverL + m_circleX), (int)(cos(m_motor.getCurrentPosition()) * m_leverL + m_circleY));
}

void Application::DrawGraph(HDC hdc)
{
	SelectObject(hdc, m_correctValuePen);
	MoveToEx(hdc, 0, 340 - (int)(m_motor.getTargetPosition() * 32.0), NULL);
	LineTo(hdc, 640, 340 - (int)(m_motor.getTargetPosition() * 32.0));

	if (m_graph.Size())
	{
		SelectObject(hdc, m_graphPen);
		MoveToEx(hdc, 0, 340 - (int)(m_graph[0] * 32.0), NULL);
		double x = 0.0;
		double step = (double)m_graph.Capacity() / 640.0;
		for (size_t i = 1; i < m_graph.Size(); i++)
		{
			x += step;
			LineTo(hdc, (int)x, 340 - (int)(m_graph[i] * 32.0));
		}
	}
	std::wstring errorStr = L"Error: " + std::to_wstring(m_motor.getError());
	TextOut(hdc, 10, 180, errorStr.c_str(), (int)errorStr.length());
}

void Application::StoreNewPosition(double newPosition)
{
	m_graph.PushBack(newPosition);
}

Application::Application() :
	m_hwnd(NULL), m_canvas(NULL),
	lbl_controller(NULL), cb_controller(NULL),
	lbl_kp(NULL), tb_kp(NULL),
	lbl_ki(NULL), tb_ki(NULL),
	lbl_kd(NULL), tb_kd(NULL),
	lbl_friction(NULL), tb_friction(NULL),
	lbl_gravity(NULL), tb_gravity(NULL),
	lbl_outputLimit(NULL), tb_outputLimit(NULL),
	lbl_integralLimit(NULL), tb_integralLimit(NULL),
	btn_set(NULL),
	btn_restart(NULL),
	m_backgroundBrush(CreateSolidBrush(RGB(50, 50, 50))),
	m_blueBrush(CreateSolidBrush(RGB(150, 150, 255))),
	m_redPen(CreatePen(PS_SOLID, 10, RGB(222, 50, 50))),
	m_bluePen(CreatePen(PS_SOLID, 8, RGB(50, 50, 222))),
	m_greenPen(CreatePen(PS_SOLID, 16, RGB(50, 222, 50))),
	m_correctValuePen(CreatePen(PS_SOLID, 4, RGB(100, 222, 150))),
	m_graphPen(CreatePen(PS_SOLID, 4, RGB(222, 150, 100))),
	m_circleX(100.0),
	m_circleY(100.0),
	m_circleR(50.0),
	m_leverL(75.0),
	m_motor(),
	m_graph(),
	m_stepResponseGraph(640) {}

Application::~Application()
{
	DeleteObject(m_backgroundBrush);
	DeleteObject(m_blueBrush);
	DeleteObject(m_redPen);
	DeleteObject(m_bluePen);
	DeleteObject(m_greenPen);
	DeleteObject(m_correctValuePen);
	DeleteObject(m_graphPen);

	DestroyWindow(m_canvas);
	DestroyWindow(lbl_controller);
	DestroyWindow(cb_controller);
	DestroyWindow(lbl_kp);
	DestroyWindow(tb_kp);
	DestroyWindow(lbl_ki);
	DestroyWindow(tb_ki);
	DestroyWindow(lbl_kd);
	DestroyWindow(tb_kd);
	DestroyWindow(lbl_friction);
	DestroyWindow(tb_friction);
	DestroyWindow(lbl_gravity);
	DestroyWindow(tb_gravity);
	DestroyWindow(lbl_outputLimit);
	DestroyWindow(tb_outputLimit);
	DestroyWindow(lbl_integralLimit);
	DestroyWindow(tb_integralLimit);
	DestroyWindow(btn_set);
	DestroyWindow(btn_restart);
}

void Application::Init()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = m_backgroundBrush;
	wc.lpfnWndProc = InitialWndProc;
	wc.lpszClassName = g_ClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);
	RECT rect{};
	rect.right = 840;
	rect.bottom = 480;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_ClassName, g_ClassName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		650, 250, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, wc.hInstance, (LPVOID)this);
	CreateUI();
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
}

void Application::Run()
{
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
LRESULT Application::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		m_hwnd = hwnd;
		SetTimer(m_hwnd, 0, 16, NULL);
		return 0;
	case WM_TIMER:
		m_motor.Update(0.016);
		StoreNewPosition(m_motor.getCurrentPosition());
		InvalidateRect(m_canvas, NULL, FALSE);
		return 0;
	case WM_COMMAND:
		UIEvent(wparam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT Application::CanvasMessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
		PaintCanvas();
		return 0;
	case WM_LBUTTONDOWN:
		LMouseButtonDown(LOWORD(lparam), HIWORD(lparam));
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
