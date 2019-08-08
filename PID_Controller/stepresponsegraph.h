#pragma once

#include "motor.h"
#include <Windows.h>

class StepResponseGraph
{
	HPEN m_correctValuePen;
	HPEN m_graphPen;
	double* m_graph;
	size_t m_size;
	Motor m_motor;

private:
	void CalculateGraph();

public:
	StepResponseGraph(size_t size);
	~StepResponseGraph();
	void SetMotorParams(const char* controller, double kp, double ki, double kd, double friction, double gravity, double outputLimit, double integralLimit);
	void SetMotorTargetPosition(double target);
	void DrawGraph(HDC hdc, int x, int y, int w, int h);
};