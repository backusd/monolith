#include "MoveLookController.h"


MoveLookController::MoveLookController(DirectX::XMFLOAT3 boxDimensions) :
    m_elapsedTime(0.0)
{
    ResetState();

	m_eyeVec = { 0.0f, 0.0f, 2 * boxDimensions.z, 0.0f };
	m_atVec = { 0.0f, 0.0f, 0.0f, 0.0f };								// look at the origin
	m_upVec = { 0.0f, 1.0f, 0.0f, 0.0f };								// Up in the positive y-direction
}

void MoveLookController::ResetState()
{
    m_mouseDown = false;
    m_left = false;
    m_right = false;
    m_up = false;
    m_down = false;
    m_shift = false;
}

DirectX::XMMATRIX MoveLookController::ViewMatrix()
{
	return DirectX::XMMatrixLookAtRH(m_eyeVec, m_atVec, m_upVec);
}

void MoveLookController::Update(StepTimer const& timer, D2D1_RECT_F renderPaneRect)
{
    if (m_mouseDown)
    {
        // Compute the eye distance to center
        float radius = 0.0f;
        DirectX::XMStoreFloat(&radius, DirectX::XMVector3Length(m_eyeVec));

        // If the pointer were to move from the middle of the screen to the far right,
        // that should produce one full rotation. Therefore, set a rotationFactor = 2
        float rotationFactor = 2.0f;
        float renderPaneWidth = renderPaneRect.right - renderPaneRect.left;
        float renderPaneHeight = renderPaneRect.bottom - renderPaneRect.top;

        float radiansPerPixelX = (DirectX::XM_2PI / renderPaneWidth) * rotationFactor;
        float radiansPerPixelY = (DirectX::XM_2PI / renderPaneHeight) * rotationFactor;

        float thetaX = radiansPerPixelX * (m_mousePositionX - m_mousePositionXNew);
        float thetaY = radiansPerPixelY * (m_mousePositionYNew - m_mousePositionY);

        // Rotate
        RotateLeftRight(thetaX);
        RotateUpDown(thetaY);

        // reset the mouse position variables
        m_mousePositionX = m_mousePositionXNew;
        m_mousePositionY = m_mousePositionYNew;
    }
    else if (m_up || m_down || m_left || m_right)
    {
        // When a button is pressed, we must begin tracking the time before we can make an update
        if (m_elapsedTime < 0.01f)
        {
            m_elapsedTime = timer.GetTotalSeconds();
            return;
        }

        // Compute the time delta
        double currentTime = timer.GetTotalSeconds();
        double timeDelta = currentTime - m_elapsedTime;
        m_elapsedTime = currentTime;

        // Compute the rotation
        float radiansPerSecond = 0.5;
        float theta = static_cast<float>(timeDelta * radiansPerSecond);

        // If rotating up or right, make the angle negative so the rest of the math is the same
        if (m_up || m_right)
            theta *= -1;

        if (m_up || m_down)
            RotateUpDown(theta);

        if (m_left || m_right)
            RotateLeftRight(theta);
    }
}

void MoveLookController::RotateLeftRight(float theta)
{
    // Use Rodrigue's Rotation Formula
        //     See here: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
        //     v_rot : vector after rotation
        //     v     : vector before rotation
        //     theta : angle of rotation
        //     k     : unit vector representing axis of rotation
        //     v_rot = v*cos(theta) + (k x v)*sin(theta) + k*(k dot v)*(1-cos(theta))

    XMVECTOR v = m_eyeVec;
    XMVECTOR k = m_upVec;
    m_eyeVec = v * cos(theta) + XMVector3Cross(k, v) * sin(theta) + k * XMVector3Dot(k, v) * (1 - cos(theta));

    // Do NOT change the up-vector
}

void MoveLookController::RotateUpDown(float theta)
{
    // Use Rodrigue's Rotation Formula
    //     See here: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    //     v_rot : vector after rotation
    //     v     : vector before rotation
    //     theta : angle of rotation
    //     k     : unit vector representing axis of rotation
    //     v_rot = v*cos(theta) + (k x v)*sin(theta) + k*(k dot v)*(1-cos(theta))

    // The axis of rotation vector for up/down rotation will be the cross product 
    // between the eye-vector and the up-vector (must make it a unit vector)
    XMVECTOR v = m_eyeVec;
    XMVECTOR k = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(m_eyeVec, m_upVec));
    m_eyeVec = v * cos(theta) + DirectX::XMVector3Cross(k, v) * sin(theta) + k * DirectX::XMVector3Dot(k, v) * (1 - cos(theta));

    // Now update the new up-vector should be the cross product between the k-vector and the new eye-vector
    m_upVec = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(k, m_eyeVec));
}

bool MoveLookController::IsMoving()
{
    return m_up || m_down || m_left || m_right || m_mouseDown;
}

void MoveLookController::OnLButtonDown(float mouseX, float mouseY)
{
    // When the pointer is pressed begin tracking the pointer movement.
    m_mouseDown = true;
    m_mousePositionX = m_mousePositionXNew = mouseX;
    m_mousePositionY = m_mousePositionYNew = mouseY;
}

void MoveLookController::OnLButtonUp(float mouseX, float mouseY)
{
    // Stop tracking pointer movement when the pointer is released.
    m_mouseDown = false;
    m_mousePositionX = m_mousePositionXNew = mouseX;
    m_mousePositionY = m_mousePositionYNew = mouseY;
}

void MoveLookController::OnMouseMove(float mouseX, float mouseY)
{
    m_mousePositionXNew = mouseX;
    m_mousePositionYNew = mouseY;
}

void MoveLookController::OnMouseLeave()
{

}

void MoveLookController::OnKeyDown(unsigned char keycode)
{
    switch (keycode)
    {
    case VK_UP:    m_up = true; break;
    case VK_DOWN:  m_down = true; break;
    case VK_LEFT:  m_left = true; break;
    case VK_RIGHT: m_right = true; break;
    case VK_SHIFT: m_shift = true; break;
    }
}

void MoveLookController::OnKeyUp(unsigned char keycode)
{
    switch (keycode)
    {
    case VK_UP:    m_up = false; break;
    case VK_DOWN:  m_down = false; break;
    case VK_LEFT:  m_left = false; break;
    case VK_RIGHT: m_right = false; break;
    case VK_SHIFT: m_shift = false; break;
    }

    // If no longer moving or rotating, reset the time to 0
    if (!(m_up || m_down || m_left || m_right))
        m_elapsedTime = 0.0f;
}