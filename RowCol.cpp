#include "RowCol.h"

RowCol::RowCol(float top, float left, float height, float width) :
	m_top(top),
	m_left(left),
	m_height(height),
	m_width(width)
{
}


RowColDefinition::RowColDefinition(ROW_COL_TYPE type, float value) :
	m_type(type),
	m_value(value)
{
}