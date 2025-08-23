#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;
extern Model* modelLIF;
extern Model* modelPolyProbe;
extern Model* model_8LIF;
extern Model* modelCochlea;
extern Model* modelCV2ISI;
extern Model* modelISI2CV;
extern Model* modelReservoir;
extern Model* modelReservoirPlus;
