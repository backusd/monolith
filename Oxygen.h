#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"


class Oxygen : public Atom
{
public:
	// Constructors
	// Most common isotope = Oxygen-16
	// Most common charge  = 0
	Oxygen(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 8, int charge = 0);

	// Update
	void Update(double timeDelta, const std::vector<std::shared_ptr<Atom>>& atoms, DirectX::XMFLOAT3 boxDimensions) override;
};