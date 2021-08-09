#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"


class Nitrogen : public Atom
{
public:
	// Constructors
	// Most common isotope = Nitrogen-14
	// Most common charge  = 0
	Nitrogen(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 7, int charge = 0);

	// Update
	void Update(double timeDelta, const std::vector<Atom*>& atoms, DirectX::XMFLOAT3 boxDimensions);
};