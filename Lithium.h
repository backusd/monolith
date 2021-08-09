#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"



class Lithium : public Atom
{
public:
	// Constructors
	// Most common isotope = Lithium-7
	// Most common charge  = +1
	Lithium(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 4, int charge = 1);

	// Update
	void Update(double timeDelta, const std::vector<Atom*>& atoms, DirectX::XMFLOAT3 boxDimensions);
};