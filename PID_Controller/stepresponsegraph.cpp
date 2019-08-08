#include "stepresponsegraph.h"

void StepResponseGraph::CalculateGraph()
{
	m_motor.Restart();
	m_graph[0] = m_motor.getCurrentPosition();
	for (size_t i = 1; i < m_size; i++)
	{
		m_motor.Update(0.016);
		m_graph[i] = m_motor.getCurrentPosition();
	}
}

StepResponseGraph::StepResponseGraph(size_t size) :
	m_correctValuePen(CreatePen(PS_SOLID, 4, RGB(100, 222, 150))),
	m_graphPen(CreatePen(PS_SOLID, 4, RGB(222, 150, 100))),
	m_graph(new double[size]),
	m_size(size),
	m_motor() 
{
	CalculateGraph();
}

StepResponseGraph::~StepResponseGraph()
{
	if (m_graph) delete[] m_graph;
	DeleteObject(m_correctValuePen);
	DeleteObject(m_graphPen);
}

void StepResponseGraph::SetMotorParams(const char* controller, double kp, double ki, double kd, double friction, double gravity, double outputLimit, double integralLimit)
{
	m_motor.SetParams(controller, kp, ki, kd, friction, gravity, outputLimit, integralLimit);
	CalculateGraph();
}

void StepResponseGraph::SetMotorTargetPosition(double target)
{
	m_motor.setTargetPosition(target);
	CalculateGraph();
}

void StepResponseGraph::DrawGraph(HDC hdc, int x, int y, int w, int h)
{
	SelectObject(hdc, m_correctValuePen);
	double scale = (double)h / 7.0;
	int middle = y + h / 2;
	MoveToEx(hdc, x, middle - (int)(m_motor.getTargetPosition() * scale), NULL);
	LineTo(hdc, x + w, middle - (int)(m_motor.getTargetPosition() * scale));

	SelectObject(hdc, m_graphPen);
	MoveToEx(hdc, x, middle - (int)(m_graph[0] * scale), NULL);
	double xd = 0.0;
	double step = (double)w / 640.0;
	for (size_t i = 1; i < m_size; i++)
	{
		xd += step;
		LineTo(hdc, x + (int)xd, middle - (int)(m_graph[i] * scale));
	}
}
