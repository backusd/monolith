#pragma once
#include "pch.h"

#include "StepTimer.h"

#include <cmath>

// For some reason, the XMVECTOR operator* does not work unless I include the 
// entire namespace
using namespace DirectX;

class MoveLookController
{
public:
	MoveLookController(DirectX::XMFLOAT3 boxDimensions);

	DirectX::XMMATRIX ViewMatrix();

	void Update(StepTimer const& timer, D2D1_RECT_F renderPaneRect);
	bool IsMoving();

	void OnLButtonDown(float mouseX, float mouseY);
	void OnLButtonUp(float mouseX, float mouseY);
	void OnMouseMove(float mouseX, float mouseY);
	void OnMouseLeave();

	void OnKeyDown(unsigned char keycode);
	void OnKeyUp(unsigned char keycode);

	bool LButtonIsDown() { return m_mouseDown; }

private:
	void ResetState();

	void RotateLeftRight(float theta);
	void RotateUpDown(float theta);


	// Eye/at/up vectors
	DirectX::XMVECTOR m_eyeVec;
	DirectX::XMVECTOR m_atVec;
	DirectX::XMVECTOR m_upVec;

	// Pointer Variables
	bool  m_mouseDown;
	float m_mousePositionX;
	float m_mousePositionY;
	float m_mousePositionXNew;
	float m_mousePositionYNew;

	// Input states for keyboard
	bool  m_left;
	bool  m_right;
	bool  m_up;
	bool  m_down;
	bool  m_shift;

	// Keep track of total time to be able to compute the time delta
	double m_elapsedTime;
};