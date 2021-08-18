#pragma once
#include "pch.h"

#include "Atom.h"
#include "Enums.h"



class Helium : public Atom
{
public:
	// Constructors
	Helium(const std::shared_ptr<DeviceResources>& deviceResources, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount = 2, int charge = 0);

	// Update
	void Update(double timeDelta, const std::vector<std::shared_ptr<Atom>>& atoms, DirectX::XMFLOAT3 boxDimensions) override;
};