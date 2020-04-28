//
//  Simulator.h
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#pragma once

#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <vector>
#include <math.h>
#include <stdio.h>
#include "freeglut.h"
#include "core/IListener.h"
#include "core/Color.h"
#include "entities/Particle.h"
#include "entities/Plane.h"
#include "entities/Spring.h"
#include "entities/ClothTriangle.h"
#include "entities/ClothInfo.h"

#define CLOTH_PARTICLE_COUNT 25
#define GRAVITY tkVec3(0,-9.8f,0)

using namespace std;

namespace Core
{
	class tkSimulator : public IListener
	{
	public:
		tkSimulator(void);
		~tkSimulator(void);

		void Init();
		void Render();
		void Update();
		void KeyPressed(unsigned char key, int x, int y);

		inline float getTime() { return m_Time; }
		inline ClothInfo getCloth() { return cloth; }
		inline float **getPhase() { return m_Phase; }
		inline float **getdPhase() { return m_dPhase; }

		// Init with {n} particles, {s} springs and {p} planes
		void InitializeSimulator(int n, int s, int p);
		
		void InitializeParticleSystem(vector<tkVec3> x, 
									  vector<tkVec3> v, 
									  vector<float> m,
									  vector<float> r);

		void InitializeSpringSystem(int i_o, int i_p);
		void InitializePlaneSystem();
		void InitializeClothSystem(vector<tkVec2> uv);
		void ForceCalculus();
		bool CollisionDetection();
		void UpdatePhase();
		// ODE Euler solver
		void SolveODE_Euler();
		// Runge-Kutta order 4 solver
		void SolveODE_RK4(float *solution, int i_particle);
		void CalcPhaseDerivative(float time, float pf[6], int i_particle, float df[6]);

		inline void UpdateTime() { m_Time += m_step; m_itr++; }
		inline unsigned int GetFixedTime() { return m_ft; }

		void SimStep();

	private:
		std::vector<tkParticle> m_Particles;	// Particle array
		std::vector<tkPlane> m_Planes;			// Plane array

		ClothInfo cloth;		// Cloth info

		float m_Time;		/* Time since the simulation started */
		int m_itr;			/* Number of iterations */
		float m_step;		/* Simulation step */
		float m_dt;			/* Time since the last frame (delta time) */
		unsigned int m_ft;	/* Timer callback in milliseconds (ms) */
		
		int m_nParticles;	// Number of particles
		int m_nPlanes;		// Number of planes
		float **m_Phase;	//  phase[6]=<Xx, Xy, Xz, Vx,   Vy,   Vz> 
		float **m_dPhase;	// dphase[6]=<Vx, Vy, Vz, Fx/m, Fy/m, Fz/m>
	};
}

#endif /*!__SIMULATOR_H__*/