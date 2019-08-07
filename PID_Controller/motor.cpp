#include "motor.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

void Motor::P()
{
	double error = m_targetPosition - m_currentPosition;
	m_controlValue = m_kp * error;
	m_lastError = error;
}

void Motor::PI()
{
	double error = m_targetPosition - m_currentPosition;
	m_integral += error;
	m_controlValue = m_kp * error + m_ki * m_integral;
	m_lastError = error;
}

void Motor::PD()
{
	double error = m_targetPosition - m_currentPosition;
	double derivative = error - m_lastError;
	m_controlValue = m_kp * error + m_kd * derivative;
	m_lastError = error;
}

void Motor::PID()
{
	double error = m_targetPosition - m_currentPosition;
	m_integral += error;
	double derivative = error - m_lastError;
	m_controlValue = m_kp * error + m_ki * m_integral + m_kd * derivative;
	m_lastError = error;
}

void Motor::Control()
{
	switch (m_controller)
	{
	case ControllerType::CT_P:
		P();
		break;
	case ControllerType::CT_PI:
		PI();
		break;
	case ControllerType::CT_PD:
		PD();
		break;
	case ControllerType::CT_PID:
		PID();
		break;
	}
}

void Motor::Simulate(double timeStep)
{
	double force = -m_gravity * sin(m_currentPosition) + m_controlValue;
	if (abs(force) < abs(m_friction)) return;
	if (force < 0.0) m_friction *= -1.0;
	force -= m_friction;
	m_currentPosition += force * timeStep;
	//m_currentPosition += m_controlValue * timeStep;
}

Motor::Motor() :
	m_kp(1.0),
	m_ki(1.0),
	m_kd(1.0),
	m_lastError(0.0),
	m_integral(0.0),
	m_friction(0.1),
	m_gravity(0.1),
	m_currentPosition(0.0),
	m_controlValue(0.0),
	m_targetPosition(M_PI * 0.5),
	m_controller(ControllerType::CT_P) {}

void Motor::Restart()
{
	m_currentPosition = 0.0;
	m_integral = 0.0;
	m_lastError = 0.0;
}

void Motor::SetParams(const char* controller, double kp, double ki, double kd, double friction, double gravity)
{
	if (std::strcmp(controller, "P") == 0)
		m_controller = ControllerType::CT_P;
	else if (std::strcmp(controller, "PI") == 0)
		m_controller = ControllerType::CT_PI;
	else if (std::strcmp(controller, "PD") == 0)
		m_controller = ControllerType::CT_PD;
	else if (std::strcmp(controller, "PID") == 0)
		m_controller = ControllerType::CT_PID;

	m_kp = kp;
	m_ki = ki;
	m_kd = kd;
	m_friction = friction;
	m_gravity = gravity;
}

void Motor::Update(double timeStep)
{
	Simulate(timeStep);
	Control();
}