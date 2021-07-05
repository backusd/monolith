#pragma once
#include "pch.h"

#include <vector>


enum class ROW_COL_TYPE {
	ROW_COL_TYPE_STAR,
	ROW_COL_TYPE_FIXED
};

/*  Defines the actual row/column in physical pixels within the window  */
class RowCol
{
public:
	RowCol(float top, float left, float height, float width);

	float Top() { return m_top; }
	float Left() { return m_left; }
	float Height() { return m_height; }
	float Width() { return m_width; }

private:
	float m_top;
	float m_left;
	float m_height;
	float m_width;
};

/*  Provides the abstract defintion for how a row/column is to be created */
class RowColDefinition
{
public:
	RowColDefinition(ROW_COL_TYPE type, float value);

	ROW_COL_TYPE	Type() { return m_type; }
	float			Value() { return m_value; }

private:
	ROW_COL_TYPE m_type;
	float m_value;
};

/*  Provides a list of all row or column definitions for a Layout */
class RowColDefinitions
{
public:
	RowColDefinitions() {}

	void AddDefinition(ROW_COL_TYPE type, float value) { m_definitions.push_back(RowColDefinition(type, value)); }

	unsigned int Count() { return static_cast<unsigned int>(m_definitions.size()); }
	RowColDefinition GetDefinition(int index) { return m_definitions[index]; }

private:
	std::vector<RowColDefinition> m_definitions;

};