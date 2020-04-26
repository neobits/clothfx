
#include ".\simulator.h"
#include <math.h>
#include <stdio.h>
#include "Triangle.h"

//----------------------------------------------------------------//
CSimulator::CSimulator(void)
:m_nParticles(1),m_nSprings(0),m_Kg(-9.8f),m_Kdrag(0.0f),m_Time(0.f)
{

}
//----------------------------------------------------------------//
CSimulator::~CSimulator(void)
{
	int i;
	for(i=0;i<m_nParticles;i++)
	{
		delete[] m_Phase[i];
		delete[] m_dPhase[i];
	}

	delete[] m_Phase;
	delete[] m_dPhase;
	delete[] springArray;
	delete[] partArray;
	delete[] planeArray;
}
//----------------------------------------------------------------//
void CSimulator::InitializeSimulator(int n, int s, int p)
{
	m_nParticles = n;
	m_nSprings = s;
	m_nPlanes = p;

	partArray = new CParticle[m_nParticles];
	springArray = new CSpring[m_nSprings];
	planeArray = new CPlane[m_nPlanes];

	m_Phase = new float*[m_nParticles];
	m_dPhase = new float*[m_nParticles];
	for (int i = 0; i < m_nParticles; i++)
	{
		m_Phase[i] = new float[6];
		m_dPhase[i] = new float[6];
	}
}
//----------------------------------------------------------------//
void CSimulator::InitializeParticleSystem(float **x, float **v, 
	float *m, float *r)
{
	int i;
	if (m_nParticles > 0)
	{
		for (i = 0; i < m_nParticles; i++)
		{
			partArray[i].SetPosition(x[i]);
			partArray[i].SetVelovity(v[i]);
			partArray[i].SetMass(m[i]);
			partArray[i].SetRadius(r[i]);
		}
	}
}
//----------------------------------------------------------------//
void CSimulator::InitializePlaneSystem(float** Pt1, 
	float** Pt2, float** Pt3)
{
	int k;
	for (k = 0; k < m_nPlanes; k++)
	{
		planeArray[k].setPlane(Pt1[k], Pt2[k], Pt3[k]);
	}
}
//----------------------------------------------------------------//
void CSimulator::InitializeClothSystem(float **uv)
{
	int i;

	//Triángulos 1-8 (primera fila)
	cloth.getTri()[0].setA(&partArray[1]);	cloth.getTri()[0].setB(&partArray[5]);	cloth.getTri()[0].setC(&partArray[0]);
	cloth.getTri()[1].setA(&partArray[1]);	cloth.getTri()[1].setB(&partArray[5]);	cloth.getTri()[1].setC(&partArray[6]);
	cloth.getTri()[2].setA(&partArray[1]);	cloth.getTri()[2].setB(&partArray[7]);	cloth.getTri()[2].setC(&partArray[6]);
	cloth.getTri()[3].setA(&partArray[1]);	cloth.getTri()[3].setB(&partArray[7]);	cloth.getTri()[3].setC(&partArray[2]);
	cloth.getTri()[4].setA(&partArray[3]);	cloth.getTri()[4].setB(&partArray[7]);	cloth.getTri()[4].setC(&partArray[2]);
	cloth.getTri()[5].setA(&partArray[3]);	cloth.getTri()[5].setB(&partArray[7]);	cloth.getTri()[5].setC(&partArray[8]);
	cloth.getTri()[6].setA(&partArray[3]);	cloth.getTri()[6].setB(&partArray[9]);	cloth.getTri()[6].setC(&partArray[8]);
	cloth.getTri()[7].setA(&partArray[3]);	cloth.getTri()[7].setB(&partArray[9]);	cloth.getTri()[7].setC(&partArray[4]);
	//Triángulos 9-16 (segunda fila fila)
	cloth.getTri()[8].setA(&partArray[5]);	cloth.getTri()[8].setB(&partArray[11]);	cloth.getTri()[8].setC(&partArray[10]);
	cloth.getTri()[9].setA(&partArray[5]);	cloth.getTri()[9].setB(&partArray[11]);	cloth.getTri()[9].setC(&partArray[6]);
	cloth.getTri()[10].setA(&partArray[7]);	cloth.getTri()[10].setB(&partArray[11]);	cloth.getTri()[10].setC(&partArray[6]);
	cloth.getTri()[11].setA(&partArray[7]);	cloth.getTri()[11].setB(&partArray[11]);	cloth.getTri()[11].setC(&partArray[12]);
	cloth.getTri()[12].setA(&partArray[7]);	cloth.getTri()[12].setB(&partArray[13]);	cloth.getTri()[12].setC(&partArray[12]);
	cloth.getTri()[13].setA(&partArray[7]);	cloth.getTri()[13].setB(&partArray[13]);	cloth.getTri()[13].setC(&partArray[8]);
	cloth.getTri()[14].setA(&partArray[9]);	cloth.getTri()[14].setB(&partArray[13]);	cloth.getTri()[14].setC(&partArray[8]);
	cloth.getTri()[15].setA(&partArray[9]);	cloth.getTri()[15].setB(&partArray[13]);	cloth.getTri()[15].setC(&partArray[14]);
	//Triángulos 17-24 (tercera fila fila)
	cloth.getTri()[16].setA(&partArray[11]);	cloth.getTri()[16].setB(&partArray[15]);	cloth.getTri()[16].setC(&partArray[10]);
	cloth.getTri()[17].setA(&partArray[11]);	cloth.getTri()[17].setB(&partArray[15]);	cloth.getTri()[17].setC(&partArray[16]);
	cloth.getTri()[18].setA(&partArray[11]);	cloth.getTri()[18].setB(&partArray[17]);	cloth.getTri()[18].setC(&partArray[16]);
	cloth.getTri()[19].setA(&partArray[11]);	cloth.getTri()[19].setB(&partArray[17]);	cloth.getTri()[19].setC(&partArray[12]);
	cloth.getTri()[20].setA(&partArray[13]);	cloth.getTri()[20].setB(&partArray[17]);	cloth.getTri()[20].setC(&partArray[12]);
	cloth.getTri()[21].setA(&partArray[13]);	cloth.getTri()[21].setB(&partArray[17]);	cloth.getTri()[21].setC(&partArray[18]);
	cloth.getTri()[22].setA(&partArray[13]);	cloth.getTri()[22].setB(&partArray[19]);	cloth.getTri()[22].setC(&partArray[18]);
	cloth.getTri()[23].setA(&partArray[13]);	cloth.getTri()[23].setB(&partArray[19]);	cloth.getTri()[23].setC(&partArray[14]);
	//Triángulos 25-32 (cuarta fila fila)
	cloth.getTri()[24].setA(&partArray[15]);	cloth.getTri()[24].setB(&partArray[21]);	cloth.getTri()[24].setC(&partArray[20]);
	cloth.getTri()[25].setA(&partArray[15]);	cloth.getTri()[25].setB(&partArray[21]);	cloth.getTri()[25].setC(&partArray[16]);
	cloth.getTri()[26].setA(&partArray[17]);	cloth.getTri()[26].setB(&partArray[21]);	cloth.getTri()[26].setC(&partArray[16]);
	cloth.getTri()[27].setA(&partArray[17]);	cloth.getTri()[27].setB(&partArray[21]);	cloth.getTri()[27].setC(&partArray[22]);
	cloth.getTri()[28].setA(&partArray[17]);	cloth.getTri()[28].setB(&partArray[23]);	cloth.getTri()[28].setC(&partArray[22]);
	cloth.getTri()[29].setA(&partArray[17]);	cloth.getTri()[29].setB(&partArray[23]);	cloth.getTri()[29].setC(&partArray[18]);
	cloth.getTri()[30].setA(&partArray[19]);	cloth.getTri()[30].setB(&partArray[23]);	cloth.getTri()[30].setC(&partArray[18]);
	cloth.getTri()[31].setA(&partArray[19]);	cloth.getTri()[31].setB(&partArray[23]);	cloth.getTri()[31].setC(&partArray[24]);

	for (i = 0; i < m_nParticles; i++)
		partArray[i].SetUV(uv[i]);

	for (i = 0; i < cloth.getTriangleCount(); i++)
		cloth.getTri()[i].setSprings();
}
//----------------------------------------------------------------//
void CSimulator::ForceCalculus()
{
	//* Varaible setup *//
	int i, j, k;
	float kpE = 0.5f;	// Particle ellasticity coefficient
	float kpRs = 0.1f;	// Particle-sphere restitution coefficient [0 < Km < 1]
	float kpRp = 0.85f;	// Particle-plane restitution coefficient [0 < Km < 1]

	float	Fi[3],		// Force [i-th]
		Ji[3],		// Local impulse [i-th]
		dVel[3],	// Delta velocity to impulse [i-th]
		JiMag,		// Impulse magnitude
		kSe,		// Spring ellasticity coefficient
		kSd,		// Spring damping coefficient
		kESt,		// Stretch stiffness energy constant
		kESr,		// Shear stiffness energy constant
		kEBd,		// Bend stiffness energy constant
		kSf = 0.2f,	// Sphere surface friction
		dx1[3],		// Direction vector: B -> A
		dx2[3],		// Direction vector: C -> A
		du1 = 0.0f, // Delta UV.u1
		du2 = 0.0f, // Delta UV.u2
		dv1 = 0.0f, // Delta UV.v1
		dv2 = 0.0f, // Delta UV.v2
		detUV,		// Matrix determinant
		Wu[3],
		Wv[3],
		Su,
		Sv,
		uvArea,		// Triangle UV area
		shTheta = 0,	// Shear angle
		Ri;			// i-th particle radius

	float Re = 10.0f; // sphere radius
	float Xe[] = { 2.0f,30.0f,0.0f }; // sphere position
	float XiXe[3]; // Direction from the particle (Xi) to sphere (Xe)
	float nmXiXe[3]; // Normalized direction vector XiXe
	float magXiXe = 0.0f; // Magnitude of vector XiXe

	CParticle *pt_i, *pt_j, *pi_k;

	//* For all particles *//

	float *x_i;			// particle i-th position
	float *vel_i;		// particle i-th velocity
	float dS = 0.0f;	// delta S

	for (i = 0; i < m_nParticles; i++)
	{
		x_i = partArray[i].GetPosition();
		vel_i = partArray[i].GetVelocity();
		Ri = partArray[i].GetRadius();

		// Init data for particles
		Fi[0] = Fi[1] = Fi[2] = 0.0f;
		dVel[0] = dVel[1] = dVel[2] = 0.0f;
		Ji[0] = Ji[1] = Ji[2] = 0.0f;

		// >> Attraction
		Fi[1] += partArray[i].GetMass()*m_Kg;

		// Unitary forces
		// >> Drag
		Fi[0] += -partArray[i].GetVelocity()[0] * m_Kdrag;
		Fi[1] += -partArray[i].GetVelocity()[1] * m_Kdrag;
		Fi[2] += -partArray[i].GetVelocity()[2] * m_Kdrag;

		//* Collision detection *//

		// Against plane H[k]
		for (k = 0; k < m_nPlanes; k++)
		{
			dS = (Ri - (float)fabs(planeArray[k].evaluatePoint(x_i)));
			if (dS > 0)
			{
				Fi[0] = Fi[1] = Fi[2] = 0.0f;
				dVel[0] += (-(1 + kpRp)*vel_i[0] * planeArray[k].getNormal()[0])*planeArray[k].getNormal()[0];
				dVel[1] += (-(1 + kpRp)*vel_i[1] * planeArray[k].getNormal()[1])*planeArray[k].getNormal()[1];
				dVel[2] += (-(1 + kpRp)*vel_i[2] * planeArray[k].getNormal()[2])*planeArray[k].getNormal()[2];
			} //end-if
		} //end-for

		// Against the sphere

		XiXe[0] = x_i[0] - Xe[0];
		XiXe[1] = x_i[1] - Xe[1];
		XiXe[2] = x_i[2] - Xe[2];
		magXiXe = (float)sqrt(XiXe[0] * XiXe[0] + XiXe[1] * XiXe[1] + XiXe[2] * XiXe[2]);

		dS = (Ri + Re) - magXiXe;

		if (dS > 0.f)
		{
			// TODO: Null force on contact (?)
			//Fi[0] = Fi[1] = Fi[2] = 0.0f;

			nmXiXe[0] = XiXe[0] / magXiXe;
			nmXiXe[1] = XiXe[1] / magXiXe;
			nmXiXe[2] = XiXe[2] / magXiXe;

			float veln = nmXiXe[0] * vel_i[0] + nmXiXe[1] * vel_i[1] + nmXiXe[2] * vel_i[2];

			JiMag = -(1 + kpRs)*partArray[i].GetMass() * veln;

			float vt[3];
			vt[0] = vel_i[0] - veln * nmXiXe[0];
			vt[1] = vel_i[1] - veln * nmXiXe[1];
			vt[2] = vel_i[2] - veln * nmXiXe[2];
			float vtMag = (float)sqrt(vt[0] * vt[0] + vt[1] * vt[1] + vt[2] * vt[2]);

			if (vtMag < 0.0001f)
			{
				Ji[0] += JiMag * nmXiXe[0];
				Ji[1] += JiMag * nmXiXe[1];
				Ji[2] += JiMag * nmXiXe[2];
			}
			else
			{
				Ji[0] += JiMag * nmXiXe[0] - (JiMag*kSf*vt[0]) / vtMag;
				Ji[1] += JiMag * nmXiXe[1] - (JiMag*kSf*vt[1]) / vtMag;
				Ji[2] += JiMag * nmXiXe[2] - (JiMag*kSf*vt[2]) / vtMag;
			}
			dVel[0] += Ji[0] / partArray[i].GetMass();
			dVel[1] += Ji[1] / partArray[i].GetMass();
			dVel[2] += Ji[2] / partArray[i].GetMass();
		}//end-if

		// Update Newtonian forces
		partArray[i].SetForce(Fi);
		partArray[i].AddVelocity(dVel);
	}//end-for


	//* For all UV triangles *//

	for (i = 0; i < cloth.getTriangleCount(); i++)
	{
		cloth.getTri()[i].getSprings()[0].checked = false;
		cloth.getTri()[i].getSprings()[1].checked = false;
		cloth.getTri()[i].getSprings()[2].checked = false;
	}
	for (i = 0; i < cloth.getTriangleCount(); i++)
	{
		for (j = 0; j < 3; j++)
		{
			// Fuerzas = 0
			Fi[0] = Fi[1] = Fi[2] = 0.0f;

			if (j == 0)
			{
				pt_i = cloth.getTri()[i].getA();
				pt_j = cloth.getTri()[i].getB();
				pi_k = cloth.getTri()[i].getC();
			}
			else if (j == 1)
			{
				pt_i = cloth.getTri()[i].getB();
				pt_j = cloth.getTri()[i].getA();
				pi_k = cloth.getTri()[i].getC();
			}
			else if (j == 2)
			{
				pt_i = cloth.getTri()[i].getC();
				pt_j = cloth.getTri()[i].getB();
				pi_k = cloth.getTri()[i].getA();
			}

			dx1[0] = pt_j->GetPosition()[0] - pt_i->GetPosition()[0];
			dx1[1] = pt_j->GetPosition()[1] - pt_i->GetPosition()[1];
			dx1[2] = pt_j->GetPosition()[2] - pt_i->GetPosition()[2];

			dx2[0] = pi_k->GetPosition()[0] - pt_i->GetPosition()[0];
			dx2[1] = pi_k->GetPosition()[1] - pt_i->GetPosition()[1];
			dx2[2] = pi_k->GetPosition()[2] - pt_i->GetPosition()[2];

			du1 = pt_j->GetUV()[0] - pt_i->GetUV()[0];
			du2 = pi_k->GetUV()[0] - pt_i->GetUV()[0];

			dv1 = pt_j->GetUV()[1] - pt_i->GetUV()[1];
			dv2 = pi_k->GetUV()[1] - pt_i->GetUV()[1];

			kESt = cloth.getStretchEk();
			kESr = cloth.getShearEk();
			kEBd = cloth.getBendEk();

			//* Energy functions *//

			//detIMuv = fabs(du1 * dv2 - dv1 * du2);
			detUV = du1 * dv2 - dv1 * du2;

			if (fabs(detUV) < 0.0001f)
				detUV = 0.0001f;

			Wu[0] = (dx1[0] * dv2 - dx2[0] * dv1) / detUV;
			Wu[1] = (dx1[1] * dv2 - dx2[1] * dv1) / detUV;
			Wu[2] = (dx1[2] * dv2 - dx2[2] * dv1) / detUV;

			Wv[0] = (-dx1[0] * du2 + dx2[0] * du1) / detUV;
			Wv[1] = (-dx1[1] * du2 + dx2[1] * du1) / detUV;
			Wv[2] = (-dx1[2] * du2 + dx2[2] * du1) / detUV;

			//Normalizamos
			float normaWu = (float)sqrt(Wu[0] * Wu[0] + Wu[1] * Wu[1] + Wu[2] * Wu[2]);
			Wu[0] = Wu[0] / normaWu;
			Wu[1] = Wu[1] / normaWu;
			Wu[2] = Wu[2] / normaWu;
			float normaWv = (float)sqrt(Wv[0] * Wv[0] + Wv[1] * Wv[1] + Wv[2] * Wv[2]);
			Wv[0] = Wv[0] / normaWv;
			Wv[1] = Wv[1] / normaWv;
			Wv[2] = Wv[2] / normaWv;

			uvArea = cloth.getTri()[i].GetAreaUV();

			// STRETCH //
			Su = (float)sqrt(Wu[0] * Wu[0] + Wu[1] * Wu[1] + Wu[2] * Wu[2]) - 0.9f;//bu = 1
			Sv = (float)sqrt(Wv[0] * Wv[0] + Wv[1] * Wv[1] + Wv[2] * Wv[2]) - 1.0f;//bv = 1
			// Stretch energy
			float stE = (kESt * 0.5f) * (Su * Su + Sv * Sv) * uvArea;

			x_i = pt_i->GetPosition();

			Fi[0] += -stE * x_i[0];
			Fi[1] += -stE * x_i[1];
			Fi[2] += -stE * x_i[2];

			// SHEAR //
			shTheta = (float)acos(Wu[0] * Wv[0] + Wu[1] * Wv[1] + Wu[2] * Wv[2]);
			// Shear energy
			float shE = (kESr * 0.5f) * (shTheta * shTheta) * uvArea;

			Fi[0] += -shE * x_i[0];
			Fi[1] += -shE * x_i[1];
			Fi[2] += -shE * x_i[2];

			// BEND //
			// Bend energy
			float bdE = 0;
			//(?)

			if (j == 0) {
				cloth.getTri()[i].getA()->AddForce(Fi);
			}
			else if (j == 1) {
				cloth.getTri()[i].getB()->AddForce(Fi);
			}
			else if (j == 2) {
				cloth.getTri()[i].getC()->AddForce(Fi);
			}
		}//end-for

		// Link-spring S
		
		float *x_o;		// pos. of particle O from  S
		float *x_p;		// pos. of particle P from S
		float *v_o;		// velocity of partícula O from S
		float *v_p;		// velocity of partícula P from S
		float XoXp[3];	// pos. vector from O to P
		float XoXpLen;	// Length of the vector OP
		float VoVp[3];  // vel. vector from O to P
		float sLen;		// spring rest length 
		float deltaL;	// delta length (small length diff.)
		float absdL;	// abs value of delta length
		float dotProduct;

		// Force calculation for the 3 triangle links
		for (j = 0; j < 3; j++)
		{
			// Check the link to be include once
			if (!cloth.getTri()[i].getSprings()[j].checked)
			{
				// Forces = 0
				Fi[0] = Fi[1] = Fi[2] = 0;

				x_o = cloth.getTri()[i].getSprings()[j].getO()->GetPosition();
				x_p = cloth.getTri()[i].getSprings()[j].getP()->GetPosition();
				v_o = cloth.getTri()[i].getSprings()[j].getO()->GetVelocity();
				v_p = cloth.getTri()[i].getSprings()[j].getP()->GetVelocity();
				kSe = cloth.getTri()[i].getSprings()[j].getEllasticK();
				kSd = cloth.getTri()[i].getSprings()[j].getDampK();
				sLen = cloth.getTri()[i].getSprings()[j].getLength();

				XoXp[0] = x_o[0] - x_p[0];	XoXp[1] = x_o[1] - x_p[1];	XoXp[2] = x_o[2] - x_p[2];
				VoVp[0] = v_o[0] - v_p[0];	VoVp[1] = v_o[1] - v_p[1];	VoVp[2] = v_o[2] - v_p[2];

				XoXpLen = (float)sqrt(XoXp[0] * XoXp[0] + XoXp[1] * XoXp[1] + XoXp[2] * XoXp[2]);

				deltaL = XoXpLen - sLen;
				absdL = (float)fabs(deltaL);
				dotProduct = VoVp[0] * XoXp[0] + VoVp[1] * XoXp[1] + VoVp[2] * XoXp[2];

				// Link forces
				if (deltaL > 0)
				{
					// Ellastic force
					Fi[0] += -deltaL / absdL * kSe*absdL * (XoXp[0] / XoXpLen);
					Fi[1] += -deltaL / absdL * kSe*absdL * (XoXp[1] / XoXpLen);
					Fi[2] += -deltaL / absdL * kSe*absdL * (XoXp[2] / XoXpLen);
					// Damping force
					Fi[0] -= deltaL / absdL * ((dotProduct) / XoXpLen)*kSd * (XoXp[0] / XoXpLen);
					Fi[1] -= deltaL / absdL * ((dotProduct) / XoXpLen)*kSd * (XoXp[1] / XoXpLen);
					Fi[2] -= deltaL / absdL * ((dotProduct) / XoXpLen)*kSd * (XoXp[2] / XoXpLen);
				}

				cloth.getTri()[i].getSprings()[j].getO()->AddForce(Fi);
				Fi[0] = -Fi[0];	Fi[1] = -Fi[1];	Fi[2] = -Fi[2];
				cloth.getTri()[i].getSprings()[j].getP()->AddForce(Fi);

				cloth.getTri()[i].getSprings()[j].checked = true;
			}//end-if
		}//end-for
	}//end-for
}
//----------------------------------------------------------------//
void CSimulator::UpdatePhase()
{
	int i, j;

	if (m_nParticles > 0)
	{
		float *x, *v, *f;
		float  m;

		for (i = 0; i < m_nParticles; i++)
		{
			x = partArray[i].GetPosition();
			v = partArray[i].GetVelocity();
			f = partArray[i].GetForce();
			m = partArray[i].GetMass();

			for (j = 0; j < 3; j++)
			{
				m_Phase[i][j] = x[j];
				m_Phase[i][j + 3] = v[j];
				m_dPhase[i][j] = v[j];
				m_dPhase[i][j + 3] = f[j] / m;
			}
		}
	}
}
//----------------------------------------------------------------//
void CSimulator::SolveODE_Euler()
{
	int i, j;
	if (m_nParticles > 0)
	{
		float x_i[3], v_i[3];
		for (i = 0; i < m_nParticles; i++)
		{
			// Solving ODE
			for (j = 0; j < 6; j++)
				m_Phase[i][j] += m_dPhase[i][j] * m_h;

			for (j = 0; j < 3; j++)
			{
				x_i[j] = m_Phase[i][j];
				v_i[j] = m_Phase[i][j + 3];
			}

			// Update particle position and velocity
			partArray[i].SetPosition(x_i);
			partArray[i].SetVelovity(v_i);
		}
	}
}
//----------------------------------------------------------------//
void CSimulator::SolveODE_RK4(float *solution, int i_particle)
{
	int j;

	float dph_i[6]; // i-th phase derivative
	float K1[6], K2[6], K3[6], K4[6];
	float aux[6];

	//K1
	CalcPhaseDerivative(m_Time, m_Phase[i_particle], i_particle, dph_i);
	for (j = 0; j < 6; j++)
	{
		K1[j] = dph_i[j];
		aux[j] = m_Phase[i_particle][j] + (K1[j] * m_h / 2);
	}
	//K2
	CalcPhaseDerivative(m_Time + m_h / 2, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
	{
		K2[j] = dph_i[j];
		aux[j] = m_Phase[i_particle][j] + (K2[j] * m_h / 2);
	}
	//K3
	CalcPhaseDerivative(m_Time + m_h / 2, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
	{
		K3[j] = dph_i[j];
		aux[j] = m_Phase[i_particle][j] + K3[j] * m_h;
	}
	//K4
	CalcPhaseDerivative(m_Time + m_h, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
		K4[j] = dph_i[j];

	for (j = 0; j < 6; j++)
		solution[j] = m_Phase[i_particle][j] + (K1[j] + 2 * K2[j] + 2 * K3[j] + K4[j])*m_h / 6;
}
//----------------------------------------------------------------//
void CSimulator::CalcPhaseDerivative(float time, 
									 float pf[6],
									 int i_particle, 
									 float df[6])
{
	int i;
	float dphase[6];

	if (time == m_Time)
	{
		for (i = 0; i < 6; i++)
			dphase[i] = m_dPhase[i_particle][i];
	}
	else
	{
		float deltaT = time - m_Time;
		for (i = 0; i < 6; i++)
			dphase[i] = (pf[i] - m_Phase[i_particle][i]) / deltaT;
	}

	for (i = 0; i < 6; i++)
		df[i] = dphase[i];
}
//----------------------------------------------------------------//
void CSimulator::SimStep()
{
	int i, j;
	float x_i[3],	// i-th position
		v_i[3],		// i-th velocity
		solution[6];

	ForceCalculus();
	UpdatePhase();

	// SolveDOE_Euler();
	for (i = 0; i < m_nParticles; i++)
	{
		for (j = 0; j < 6; j++)
			solution[j] = 0.0f;

		SolveODE_RK4(solution, i);

		for (j = 0; j < 3; j++)
		{
			x_i[j] = solution[j];
			v_i[j] = solution[j + 3];
		}

		// Update particles position and velocity
		partArray[i].SetPosition(x_i);
		partArray[i].SetVelovity(v_i);
	}
	UpdateTime();
}
//----------------------------------------------------------------//