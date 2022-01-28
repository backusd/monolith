#pragma once

#include <string>

namespace Element
{
	enum VALUE
	{
		INVALID = 0,
		HYDROGEN = 1,
		HELIUM = 2,
		LITHIUM = 3,
		BERYLLIUM = 4,
		BORON = 5,
		CARBON = 6,
		NITROGEN = 7,
		OXYGEN = 8,
		FLOURINE = 9,
		NEON = 10
	};
}

typedef Element::VALUE ELEMENT;

static const std::wstring ElementStrings[] = {
	L"Invalid",
	L"Hydrogen",
	L"Helium",
	L"Lithium",
	L"Beryllium",
	L"Boron",
	L"Carbon",
	L"Nitrogen",
	L"Oxygen",
	L"Flourine",
	L"Neon"
};

static const std::wstring ElementAbbreviatedStrings[] = {
	L"Invalid",
	L"H",
	L"He",
	L"Li",
	L"Be",
	L"B",
	L"C",
	L"N",
	L"O",
	L"F",
	L"Ne"
};

namespace BondType
{
	enum VALUE
	{
		INVALID = 0,
		SINGLE = 1,
		DOUBLE = 2,
		TRIPLE = 3
	};
}

typedef BondType::VALUE BONDTYPE;

static const std::wstring BondTypeStrings[] = {
	L"Invalid",
	L"Single",
	L"Double",
	L"Triple"
};