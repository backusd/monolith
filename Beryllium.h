#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"

class Beryllium : public Atom
{
public:
	// Constructors
	// Most common isotope = Beryllium-9
	// Most common charge  = +2
	Beryllium(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 5, int charge = 2);

	// Update
	void Update(double timeDelta, const std::vector<Atom*>& atoms, DirectX::XMFLOAT3 boxDimensions);
};