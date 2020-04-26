#pragma once

#include "Particle.h"
#include "Plane.h"
#include "Spring.h"
#include "Cloth.h"

class CSimulator
{
public:
	CSimulator(void);
	~CSimulator(void);

	// GET methods
	inline CParticle *getParticles() { return partArray; }
	inline CSpring *getSprings() { return springArray; }
	inline CPlane *getPlaneH() { return planeArray; }
	inline float getSimStep() { return m_h; }
	inline float getTime() { return m_Time; }
	inline float getDrag() { return m_Kdrag; }
	inline CCloth getCloth() { return cloth; }
	inline int getParticleCount() { return m_nParticles; }
	inline int getSpringCount() { return m_nSprings; }
	inline int getPlaneCount() { return m_nPlanes; }
	inline float **getPhase() { return m_Phase; }
	inline float **getdPhase() { return m_dPhase; }
	// SET methods
	inline void setSimStep(float h) { m_h = h; }
	inline void setDrag(float pKs) { m_Kdrag = pKs; }
	inline void setParticleCount(int n) { m_nParticles = n; }
	inline void setSpringCount(int s) { m_nSprings = s; }

	//SIMULATOR//
	// Init with {n} particles, {s} springs and {p} planes
	void InitializeSimulator(int n, int s, int p);
	void InitializeParticleSystem(float **x, float **v, float *m, float *r);
	void InitializeSpringSystem(int i_o, int i_p);
	void InitializePlaneSystem(float** Pt1, float** Pt2, float** Pt3);
	void InitializeClothSystem(float** uv);
	void ForceCalculus();
	bool CollisionDetection();
	void UpdatePhase();
	// ODE Euler solver
	void SolveODE_Euler();
	// Runge-Kutta order 4 solver
	void SolveODE_RK4(float *solution, int i_particle);
	void CalcPhaseDerivative(float time, float pf[6], int i_particle, float df[6]);
	
	inline void UpdateTime() { m_Time += m_h; }

	void SimStep();

private:
	CParticle *partArray;	// Particle array
	CSpring *springArray;	// Spring array
	CPlane *planeArray;		// Plane array
	CCloth cloth;		// Cloth info

	int m_nParticles;	// Number of particles
	int m_nSprings;		// Number of springs
	int m_nPlanes;		// Number of planes
	float m_h;			// Simulation step
	float m_Time;		// Current time	
	float **m_Phase;	//  phase[6]=<Xx, Xy, Xz, Vx,   Vy,   Vz> 
	float **m_dPhase;	// dphase[6]=<Vx, Vy, Vz, Fx/m, Fy/m, Fz/m>
	float m_Kg;		// Gravity constant
	float m_Kdrag;	// Drag constant
};