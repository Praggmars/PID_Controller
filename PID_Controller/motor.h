#pragma once

class Motor
{
	enum ControllerType { CT_P, CT_PI, CT_PD, CT_PID };

	double m_lastError;
	double m_integral;

	double m_kp;
	double m_ki;
	double m_kd;
	double m_friction;
	double m_gravity;
	double m_outputLimit;
	double m_integralLimit;

	double m_currentPosition;
	double m_controlValue;
	double m_targetPosition;
	ControllerType m_controller;

private:
	void LimitOutput();
	void LimitIntegral();
	void P();
	void PI();
	void PD();
	void PID();
	void Control();
	void Simulate(double timeStep);

public:
	Motor();
	void Restart();
	void SetParams(const char* controller, double kp, double ki, double kd, double friction, double gravity, double outputLimit, double integralLimit);
	void Update(double timeStep);
	inline void setTargetPosition(double position) { m_targetPosition = position; }
	inline double getError() { return m_lastError; }
	inline double getCurrentPosition() { return m_currentPosition; }
	inline double getTargetPosition() { return m_targetPosition; }
};