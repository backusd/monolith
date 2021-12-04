#pragma once

#include <string>

enum Element
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