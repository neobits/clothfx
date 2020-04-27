//
//  main.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#include "Simulator.h"
#include "Core/GlutMachine.h"

using namespace std;
using namespace Core;

//---------------------- Application entry point ---------------------//
int main(int argc, char **argv)
{
	cout << "Cloth Simulation" << endl;

	tkGlutMachine *machine = new tkGlutMachine(argc, argv);
	tkSimulator *sim = new tkSimulator();
	sim->Init();
	IListener *pSim = dynamic_cast<IListener*>(sim);
	tkGlutMachine::SetListener(pSim);
	machine->InitGLUT(sim->GetFixedTime());

	tkGlutMachine::RunGLUT();

	delete pSim;
	delete sim;
	delete machine;

	return 0;
}
//----------------------------------------------------------------------