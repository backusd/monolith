#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"



class Hydrogen : public Atom
{
public:
	// Constructors
	Hydrogen(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 0, int charge = 1);

	// Update
	void Update(double timeDelta, const std::vector<Atom*>& atoms, DirectX::XMFLOAT3 boxDimensions);
};