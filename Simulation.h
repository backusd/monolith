#pragma once
#include "pch.h"

#include "Atom.h"
#include "AtomGenerator.h"
#include "StepTimer.h"

#include <cmath>
#include <vector>


class Simulation
{
public:
	Simulation(const std::shared_ptr<DeviceResources>& deviceResources);
	~Simulation();

	void AddNewAtom(Atom* atom);
	void RemoveAtom();

	void PlaySimulation() { m_paused = false; }
	void PauseSimulation() { m_paused = true; m_elapsedTime = -1.0f; }
	bool IsPaused() { return m_paused; }

	void StartRecording();
	void StopRecording();

	void LoadSimulationFromFile();
	void SaveSimulationToFile();

	void ClearSimulation();	// Completely delete the entire active simulation
	void ResetSimulation(); // Reset the simulation state to where it was before ever pressing Play

	void Update(StepTimer const& timer);
	void SwitchPlayPause() { m_paused = !m_paused; if (m_paused) m_elapsedTime = -1.0f; }

	// GET
	std::vector<Atom*> Atoms() { return m_atoms; }

	DirectX::XMFLOAT3	BoxDimensions() { return m_boxDimensions; }
	//LENGTH_UNIT BoxDimensionUnits() {	return m_boxDimensionUnits; }
	bool		BoxVisible() { return m_boxVisible; }

	float		ElapsedTime() { return m_elapsedTime; }
	//TIME_UNIT	ElapsedTimeUnit() {		return m_elapsedTimeUnit; }

	// SET
	void BoxDimensions(DirectX::XMFLOAT3 dimensions) { m_boxDimensions = dimensions; }
	//void BoxDimensionUnits(LENGTH_UNIT unit) {	m_boxDimensionUnits = unit; }
	void BoxVisible(bool visible) { m_boxVisible = visible; }

	void ElapsedTime(float time) { m_elapsedTime = time; }
	//void ElapsedTimeUnit(TIME_UNIT timeUnit) {	m_elapsedTimeUnit = timeUnit; }


private:
	// Atom Generator
	AtomGenerator m_atomGenerator;

	// Box
	DirectX::XMFLOAT3	m_boxDimensions;		// 3 floats to hold the MAX x,y,z dimensions for the simulation box (ex. if x = 10, then x-axis = [-10, 10])
	//LENGTH_UNIT m_boxDimensionUnits;	// The m_dimensions values will all be interpretted to be a specific unit
	bool		m_boxVisible;			// If true, the dimension box will be outlined

	// Time
	float		m_elapsedTime;
	//TIME_UNIT	m_elapsedTimeUnit;

	// Atoms
	std::vector<Atom*> m_atoms;			// List of Atoms active in the simulation

	// State
	bool m_paused;

};