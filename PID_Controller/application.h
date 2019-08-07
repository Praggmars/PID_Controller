#pragma once

#include "motor.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

class Application
{
	HWND m_hwnd;
	HWND m_canvas;
	HWND lbl_controller, cb_controller;
	HWND lbl_kp, tb_kp;
	HWND lbl_ki, tb_ki;
	HWND lbl_kd, tb_kd;
	HWND lbl_friction, tb_friction;
	HWND lbl_gravity, tb_gravity;
	HWND btn_set;
	HWND btn_restart;

	HBRUSH m_backgroundBrush;
	HBRUSH m_blueBrush;
	HPEN m_redPen;
	HPEN m_bluePen;
	HPEN m_greenPen;
	HPEN m_correctValuePen;
	HPEN m_graphPen;

	double m_circleX;
	double m_circleY;
	double m_circleR;
	double m_leverL;

	Motor m_motor;
	std::vector<double> m_graph;
	int m_maxGraphDataCount;

private:
	void CreateUI();
	void UIEvent(WPARAM wparam);
	void SetMotorParams();
	void RestartMotor();
	void LMouseButtonDown(int x, int y);
	void PaintCanvas();
	void PaintCanvas(HDC hdc);
	void DrawMotor(HDC hdc);
	void DrawGraph(HDC hdc);
	void StoreNewPosition(double newPosition);

public:
	Application();
	~Application();
	void Init();
	void Run();
	LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CanvasMessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};