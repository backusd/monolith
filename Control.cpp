#include "Control.h"

Control::Control(const std::shared_ptr<DeviceResources>& deviceResources, 
	             const std::shared_ptr<Layout>& parentLayout) :
	m_deviceResources(deviceResources),
	m_parentLayout(parentLayout),
	m_mouseCapturedControl(nullptr),
	m_row(0),
	m_column(0),
	m_rowSpan(1),
	m_columnSpan(1),
	m_marginLeft(0.0f),
	m_marginTop(0.0f),
	m_marginRight(0.0f),
	m_marginBottom(0.0f)
{
}

Control::Control(const std::shared_ptr<DeviceResources>& deviceResources,
				 const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	m_deviceResources(deviceResources),
	m_parentLayout(parentLayout),
	m_mouseCapturedControl(nullptr),
	m_row(row),
	m_column(column),
	m_rowSpan(rowSpan),
	m_columnSpan(columnSpan),
	m_marginLeft(0.0f),
	m_marginTop(0.0f),
	m_marginRight(0.0f),
	m_marginBottom(0.0f)
{
}

Control::~Control()
{
}


void Control::Margin(float all)
{
	m_marginLeft = all;
	m_marginTop = all;
	m_marginRight = all;
	m_marginBottom = all;

	OnMarginChanged();
}

void Control::Margin(float leftRight, float topBottom)
{
	m_marginLeft = leftRight;
	m_marginTop = topBottom;
	m_marginRight = leftRight;
	m_marginBottom = topBottom;

	OnMarginChanged();
}

void Control::Margin(float left, float top, float right, float bottom)
{
	m_marginLeft = left;
	m_marginTop = top;
	m_marginRight = right;
	m_marginBottom = bottom;

	OnMarginChanged();
}
