#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"

class Carbon : public Atom
{
public:
	// Constructors
	// Most common isotope = Carbon-12
	// Most common charge  = 0
	Carbon(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 6, int charge = 0);

	// Update
	void Update(double timeDelta, const std::vector<std::shared_ptr<Atom>>& atoms, DirectX::XMFLOAT3 boxDimensions) override;
};