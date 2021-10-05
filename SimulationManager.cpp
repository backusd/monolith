#include "SimulationManager.h"

std::unique_ptr<Simulation> SimulationManager::m_simulation = nullptr;

std::function<void(bool)> SimulationManager::PlayPauseChangedEvent = [](bool value) {};

