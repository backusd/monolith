#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"

class Flourine : public Atom
{
public:
	// Constructors
	// Most common isotope = Flourine-19
	// Most common charge  = -1
	Flourine(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 10, int charge = -1);

	// Update
	void Update(double timeDelta, const std::vector<std::shared_ptr<Atom>>& atoms, DirectX::XMFLOAT3 boxDimensions) override;
};