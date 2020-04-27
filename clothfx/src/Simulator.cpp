//
//  Simulator.cpp
//  Cloth Simulation Engine ~ clothfx
//
//  Created by Angelo Moro on 26/04/2020
//

#include "Simulator.h"

using namespace Core;

float x[25][3]; // 25 particles for cloth

//----------------------------------------------------------------//
tkSimulator::tkSimulator(void)
	:m_nParticles(1), m_nPlanes(0)
{
	m_Time = 0.f;
	m_step = 0.015f;
	m_ft = (int)(m_step * 1000);
	m_dt = 1 / (float)m_ft;
	m_itr = 0;
}
//----------------------------------------------------------------//
tkSimulator::~tkSimulator(void)
{
	int i;
	for (i = 0; i < m_nParticles; i++)
	{
		delete[] m_Phase[i];
		delete[] m_dPhase[i];
	}

	delete[] m_Phase;
	delete[] m_dPhase;
}
//----------------------------------------------------------------//
void tkSimulator::Init()
{
	int i;
	int n = CLOTH_PARTICLE_COUNT;

	// Particle initial data
	float **X0;	// position matrix
	float **V0;	// velocity matrix
	float **UV;	// uv matrix
	float	*M; // mass array
	float	*R; // radius array

	M = new float[n];
	R = new float[n];
	X0 = new float *[n];
	V0 = new float *[n];
	UV = new float *[n];
	for (i = 0; i < n; i++)
	{
		X0[i] = new float[3];
		V0[i] = new float[3];
		UV[i] = new float[2];
	}

	float m = 1.0f;		// Particle mass
	float r = 1.0f;		// Particle radius;

	float posX = -10.0f, posY = 0.f, posZ = 10.0f;
	float uvU = 0, uvV = 0;

	// System init
	// Global coordinates

	int i_col = 0;
	for (int i_p = 0; i_p < n; i_p++)
	{
		if (i_col == 0 || i_col == 5)
		{
			posX = -10.0f;
			posY = 70.0f;
			posZ -= 5.0f;
			uvU = 0.0f;
			uvV += 0.25f;
			i_col = 1;
		}
		else {
			i_col++;
		}

		X0[i_p][0] = posX;
		X0[i_p][1] = posY;
		X0[i_p][2] = posZ;
		posX += 5.0f;

		UV[i_p][0] = uvU;
		UV[i_p][1] = uvV;
		uvU += 0.25f;
	}

	for (i = 0; i < n; i++)
	{
		V0[i][0] = V0[i][1] = V0[i][2] = 0.0f;
		M[i] = m;
		R[i] = r;
	}

	// Init cloth simulator
	InitializeSimulator(n, 0, 1);
	InitializeParticleSystem(X0, V0, M, R);
	InitializeClothSystem(UV);
	InitializePlaneSystem();

	// Cleaning memory
	for (i = 0; i < n; i++)
	{
		delete[] X0[i];	delete[] V0[i];	delete[] UV[i];
	}
	delete[] X0; delete[] V0; delete[] UV;
	delete[] M; delete[] R;
}
//----------------------------------------------------------------//
void tkSimulator::Update()
{
	int i, j;
	SimStep();

	// Spatial phase extraction
	for (i = 0; i < m_nParticles; i++)
	{
		for (j = 0; j < 3; j++)
			x[i][j] = m_Phase[i][j];
	}
}
//----------------------------------------------------------------//
void tkSimulator::KeyPressed(unsigned char key, int x, int y)
{
	// ASCII code Escape = 27
	if (key == 27)
		exit(0);

	if (key == 's' || key == 'S')
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == 'w' || key == 'W')
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == 'r' || key == 'R')
		Init();

	if (key == '1')
		glEnable(GL_CULL_FACE);

	if (key == '2')
		glDisable(GL_CULL_FACE);
}
//----------------------------------------------------------------//
void tkSimulator::InitializeSimulator(int np, int ns, int nH)
{
	m_nParticles = np;
	m_nPlanes = nH;

	m_Phase = new float*[m_nParticles];
	m_dPhase = new float*[m_nParticles];
	for (int i = 0; i < m_nParticles; i++)
	{
		m_Phase[i] = new float[6];
		m_dPhase[i] = new float[6];
	}
}
//----------------------------------------------------------------//
void tkSimulator::InitializeParticleSystem(float **x, float **v, 
	float *m, float *r)
{
	int i;
	if (m_nParticles > 0)
	{
		for (i = 0; i < m_nParticles; i++)
		{
			tkParticle p;
			p.SetPosition(x[i]);
			p.SetVelovity(v[i]);
			p.SetMass(m[i]);
			p.SetRadius(r[i]);
			m_Particles.push_back(p);
		}
	}
}
//----------------------------------------------------------------//
void tkSimulator::InitializePlaneSystem()
{
	// Init plane H0 ; ground
	float c_A[] = { 100.f,  0.f, 100.f },
		c_B[] = { 100.f,  0.f,-100.f },
		c_C[] = { -100.f,  0.f,-100.f };

	tkPlane ground(c_A, c_B, c_C);
	m_Planes.push_back(ground);
}
//----------------------------------------------------------------//
void tkSimulator::InitializeClothSystem(float **uv)
{
	int i;

	// Triangles 1-8 (1st row)
	cloth.getTris()[0].setA(&m_Particles[1]);	cloth.getTris()[0].setB(&m_Particles[5]);	cloth.getTris()[0].setC(&m_Particles[0]);
	cloth.getTris()[1].setA(&m_Particles[1]);	cloth.getTris()[1].setB(&m_Particles[5]);	cloth.getTris()[1].setC(&m_Particles[6]);
	cloth.getTris()[2].setA(&m_Particles[1]);	cloth.getTris()[2].setB(&m_Particles[7]);	cloth.getTris()[2].setC(&m_Particles[6]);
	cloth.getTris()[3].setA(&m_Particles[1]);	cloth.getTris()[3].setB(&m_Particles[7]);	cloth.getTris()[3].setC(&m_Particles[2]);
	cloth.getTris()[4].setA(&m_Particles[3]);	cloth.getTris()[4].setB(&m_Particles[7]);	cloth.getTris()[4].setC(&m_Particles[2]);
	cloth.getTris()[5].setA(&m_Particles[3]);	cloth.getTris()[5].setB(&m_Particles[7]);	cloth.getTris()[5].setC(&m_Particles[8]);
	cloth.getTris()[6].setA(&m_Particles[3]);	cloth.getTris()[6].setB(&m_Particles[9]);	cloth.getTris()[6].setC(&m_Particles[8]);
	cloth.getTris()[7].setA(&m_Particles[3]);	cloth.getTris()[7].setB(&m_Particles[9]);	cloth.getTris()[7].setC(&m_Particles[4]);
	
	// Triangles 9-16 (2nd row)
	cloth.getTris()[8].setA(&m_Particles[5]);	cloth.getTris()[8].setB(&m_Particles[11]);	cloth.getTris()[8].setC(&m_Particles[10]);
	cloth.getTris()[9].setA(&m_Particles[5]);	cloth.getTris()[9].setB(&m_Particles[11]);	cloth.getTris()[9].setC(&m_Particles[6]);
	cloth.getTris()[10].setA(&m_Particles[7]);	cloth.getTris()[10].setB(&m_Particles[11]);	cloth.getTris()[10].setC(&m_Particles[6]);
	cloth.getTris()[11].setA(&m_Particles[7]);	cloth.getTris()[11].setB(&m_Particles[11]);	cloth.getTris()[11].setC(&m_Particles[12]);
	cloth.getTris()[12].setA(&m_Particles[7]);	cloth.getTris()[12].setB(&m_Particles[13]);	cloth.getTris()[12].setC(&m_Particles[12]);
	cloth.getTris()[13].setA(&m_Particles[7]);	cloth.getTris()[13].setB(&m_Particles[13]);	cloth.getTris()[13].setC(&m_Particles[8]);
	cloth.getTris()[14].setA(&m_Particles[9]);	cloth.getTris()[14].setB(&m_Particles[13]);	cloth.getTris()[14].setC(&m_Particles[8]);
	cloth.getTris()[15].setA(&m_Particles[9]);	cloth.getTris()[15].setB(&m_Particles[13]);	cloth.getTris()[15].setC(&m_Particles[14]);
	
	// Triangles 17-24 (3rd row)
	cloth.getTris()[16].setA(&m_Particles[11]);	cloth.getTris()[16].setB(&m_Particles[15]);	cloth.getTris()[16].setC(&m_Particles[10]);
	cloth.getTris()[17].setA(&m_Particles[11]);	cloth.getTris()[17].setB(&m_Particles[15]);	cloth.getTris()[17].setC(&m_Particles[16]);
	cloth.getTris()[18].setA(&m_Particles[11]);	cloth.getTris()[18].setB(&m_Particles[17]);	cloth.getTris()[18].setC(&m_Particles[16]);
	cloth.getTris()[19].setA(&m_Particles[11]);	cloth.getTris()[19].setB(&m_Particles[17]);	cloth.getTris()[19].setC(&m_Particles[12]);
	cloth.getTris()[20].setA(&m_Particles[13]);	cloth.getTris()[20].setB(&m_Particles[17]);	cloth.getTris()[20].setC(&m_Particles[12]);
	cloth.getTris()[21].setA(&m_Particles[13]);	cloth.getTris()[21].setB(&m_Particles[17]);	cloth.getTris()[21].setC(&m_Particles[18]);
	cloth.getTris()[22].setA(&m_Particles[13]);	cloth.getTris()[22].setB(&m_Particles[19]);	cloth.getTris()[22].setC(&m_Particles[18]);
	cloth.getTris()[23].setA(&m_Particles[13]);	cloth.getTris()[23].setB(&m_Particles[19]);	cloth.getTris()[23].setC(&m_Particles[14]);
	
	// Triangles 25-32 (4th row)
	cloth.getTris()[24].setA(&m_Particles[15]);	cloth.getTris()[24].setB(&m_Particles[21]);	cloth.getTris()[24].setC(&m_Particles[20]);
	cloth.getTris()[25].setA(&m_Particles[15]);	cloth.getTris()[25].setB(&m_Particles[21]);	cloth.getTris()[25].setC(&m_Particles[16]);
	cloth.getTris()[26].setA(&m_Particles[17]);	cloth.getTris()[26].setB(&m_Particles[21]);	cloth.getTris()[26].setC(&m_Particles[16]);
	cloth.getTris()[27].setA(&m_Particles[17]);	cloth.getTris()[27].setB(&m_Particles[21]);	cloth.getTris()[27].setC(&m_Particles[22]);
	cloth.getTris()[28].setA(&m_Particles[17]);	cloth.getTris()[28].setB(&m_Particles[23]);	cloth.getTris()[28].setC(&m_Particles[22]);
	cloth.getTris()[29].setA(&m_Particles[17]);	cloth.getTris()[29].setB(&m_Particles[23]);	cloth.getTris()[29].setC(&m_Particles[18]);
	cloth.getTris()[30].setA(&m_Particles[19]);	cloth.getTris()[30].setB(&m_Particles[23]);	cloth.getTris()[30].setC(&m_Particles[18]);
	cloth.getTris()[31].setA(&m_Particles[19]);	cloth.getTris()[31].setB(&m_Particles[23]);	cloth.getTris()[31].setC(&m_Particles[24]);

	for (i = 0; i < m_nParticles; i++)
		m_Particles[i].SetUV(uv[i]);

	for (i = 0; i < cloth.TriangleCount(); i++)
		cloth.getTris()[i].setSprings();
}
//----------------------------------------------------------------//
void tkSimulator::Render()
{
	// Draw : Lights
	GLfloat lightPos[] = { 1.0, 1.0, 1.0, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	int i;
	// Draw : Planes
	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(-50, 0, 50);
	glVertex3i(50, 0, 50);
	glVertex3i(50, 0, -50);
	glVertex3i(-50, 0, -50);
	glEnd();

	// Draw : Particles
	for (i = 0; i < m_nParticles; i++)
	{
		if (i == 0 || i == 24)
		{
			glColor3f(0.f, 1.0f, 0.f);
		}
		else
		{
			glColor3f(1.0f, 0.f, 0.f);
		}
		glLoadIdentity();
		glTranslatef(x[i][0], x[i][1], x[i][2]);
		glutSolidSphere(m_Particles[i].GetRadius(), 10, 10);
	}

	// Draw : Cloth
	for (i = 0; i < getCloth().TriangleCount(); i++)
	{
		glLoadIdentity();
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(0.9f, 0.9f, 0.9f);
		
		glVertex3f(getCloth().getTris()[i].getA()->GetPosition()[0],
				   getCloth().getTris()[i].getA()->GetPosition()[1],
				   getCloth().getTris()[i].getA()->GetPosition()[2]);
		
		glVertex3f(getCloth().getTris()[i].getB()->GetPosition()[0],
				   getCloth().getTris()[i].getB()->GetPosition()[1],
				   getCloth().getTris()[i].getB()->GetPosition()[2]);
		
		glVertex3f(getCloth().getTris()[i].getC()->GetPosition()[0],
				   getCloth().getTris()[i].getC()->GetPosition()[1],
				   getCloth().getTris()[i].getC()->GetPosition()[2]);
		
		glEnd();
	}
	// Draw : Sphere
	glLoadIdentity();
	glColor3f(0.5f, 0.5f, 1.0f);
	glTranslatef(2.f, 30.f, 0.f);
	glutSolidSphere(10, 40, 40);
}
//----------------------------------------------------------------//
void tkSimulator::ForceCalculus()
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

	tkParticle *pt_i, *pt_j, *pi_k;

	//* For all particles *//

	float *x_i;			// particle i-th position
	float *vel_i;		// particle i-th velocity
	float dS = 0.0f;	// delta S

	for (i = 0; i < m_nParticles; i++)
	{
		x_i = m_Particles[i].GetPosition();
		vel_i = m_Particles[i].GetVelocity();
		Ri = m_Particles[i].GetRadius();

		// Init data for particles
		Fi[0] = Fi[1] = Fi[2] = 0.0f;
		dVel[0] = dVel[1] = dVel[2] = 0.0f;
		Ji[0] = Ji[1] = Ji[2] = 0.0f;

		// Gravity
		Fi[1] += m_Particles[i].GetMass() * GRAVITY.y;

		// Unitary forces
		// >> Drag
		Fi[0] += -m_Particles[i].GetVelocity()[0] * cloth.GetDrag();
		Fi[1] += -m_Particles[i].GetVelocity()[1] * cloth.GetDrag();
		Fi[2] += -m_Particles[i].GetVelocity()[2] * cloth.GetDrag();

		//* Collision detection *//

		// Against plane H[k]
		for (k = 0; k < m_nPlanes; k++)
		{
			dS = (Ri - (float)fabs(m_Planes[k].Evaluate(x_i)));
			if (dS > 0)
			{
				Fi[0] = Fi[1] = Fi[2] = 0.0f;
				dVel[0] += (-(1 + kpRp)*vel_i[0] * m_Planes[k].getNormal()[0])*m_Planes[k].getNormal()[0];
				dVel[1] += (-(1 + kpRp)*vel_i[1] * m_Planes[k].getNormal()[1])*m_Planes[k].getNormal()[1];
				dVel[2] += (-(1 + kpRp)*vel_i[2] * m_Planes[k].getNormal()[2])*m_Planes[k].getNormal()[2];
			}
		}

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

			JiMag = -(1 + kpRs)*m_Particles[i].GetMass() * veln;

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
			dVel[0] += Ji[0] / m_Particles[i].GetMass();
			dVel[1] += Ji[1] / m_Particles[i].GetMass();
			dVel[2] += Ji[2] / m_Particles[i].GetMass();
		}//end-if

		// Update Newtonian forces
		m_Particles[i].SetForce(Fi);
		m_Particles[i].AddVelocity(dVel);
	}//end-for


	//* For all UV triangles *//

	for (i = 0; i < cloth.TriangleCount(); i++)
	{
		cloth.getTris()[i].getSprings()[0].checked = false;
		cloth.getTris()[i].getSprings()[1].checked = false;
		cloth.getTris()[i].getSprings()[2].checked = false;
	}
	for (i = 0; i < cloth.TriangleCount(); i++)
	{
		for (j = 0; j < 3; j++)
		{
			// Fuerzas = 0
			Fi[0] = Fi[1] = Fi[2] = 0.0f;

			if (j == 0)
			{
				pt_i = cloth.getTris()[i].getA();
				pt_j = cloth.getTris()[i].getB();
				pi_k = cloth.getTris()[i].getC();
			}
			else if (j == 1)
			{
				pt_i = cloth.getTris()[i].getB();
				pt_j = cloth.getTris()[i].getA();
				pi_k = cloth.getTris()[i].getC();
			}
			else if (j == 2)
			{
				pt_i = cloth.getTris()[i].getC();
				pt_j = cloth.getTris()[i].getB();
				pi_k = cloth.getTris()[i].getA();
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

			kESt = cloth.GetStretch();
			kESr = cloth.GetShear();
			kEBd = cloth.GetBend();

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

			uvArea = cloth.getTris()[i].GetAreaUV();

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
				cloth.getTris()[i].getA()->AddForce(Fi);
			}
			else if (j == 1) {
				cloth.getTris()[i].getB()->AddForce(Fi);
			}
			else if (j == 2) {
				cloth.getTris()[i].getC()->AddForce(Fi);
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
			if (!cloth.getTris()[i].getSprings()[j].checked)
			{
				// Forces = 0
				Fi[0] = Fi[1] = Fi[2] = 0;

				x_o = cloth.getTris()[i].getSprings()[j].getO()->GetPosition();
				x_p = cloth.getTris()[i].getSprings()[j].getP()->GetPosition();
				v_o = cloth.getTris()[i].getSprings()[j].getO()->GetVelocity();
				v_p = cloth.getTris()[i].getSprings()[j].getP()->GetVelocity();
				kSe = cloth.getTris()[i].getSprings()[j].GetElastic();
				kSd = cloth.getTris()[i].getSprings()[j].GetDamp();
				sLen = cloth.getTris()[i].getSprings()[j].GetLength();

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

				cloth.getTris()[i].getSprings()[j].getO()->AddForce(Fi);
				Fi[0] = -Fi[0];	Fi[1] = -Fi[1];	Fi[2] = -Fi[2];
				cloth.getTris()[i].getSprings()[j].getP()->AddForce(Fi);

				cloth.getTris()[i].getSprings()[j].checked = true;
			}//end-if
		}//end-for
	}//end-for
}
//----------------------------------------------------------------//
void tkSimulator::UpdatePhase()
{
	int i, j;

	if (m_nParticles > 0)
	{
		float *x, *v, *f;
		float  m;

		for (i = 0; i < m_nParticles; i++)
		{
			x = m_Particles[i].GetPosition();
			v = m_Particles[i].GetVelocity();
			f = m_Particles[i].GetForce();
			m = m_Particles[i].GetMass();

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
void tkSimulator::SolveODE_Euler()
{
	int i, j;
	if (m_nParticles > 0)
	{
		float x_i[3], v_i[3];
		for (i = 0; i < m_nParticles; i++)
		{
			// Solving ODE
			for (j = 0; j < 6; j++)
				m_Phase[i][j] += m_dPhase[i][j] * m_step;

			for (j = 0; j < 3; j++)
			{
				x_i[j] = m_Phase[i][j];
				v_i[j] = m_Phase[i][j + 3];
			}

			// Update particle position and velocity
			m_Particles[i].SetPosition(x_i);
			m_Particles[i].SetVelovity(v_i);
		}
	}
}
//----------------------------------------------------------------//
void tkSimulator::SolveODE_RK4(float *solution, int i_particle)
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
		aux[j] = m_Phase[i_particle][j] + (K1[j] * m_step / 2);
	}
	//K2
	CalcPhaseDerivative(m_Time + m_step / 2, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
	{
		K2[j] = dph_i[j];
		aux[j] = m_Phase[i_particle][j] + (K2[j] * m_step / 2);
	}
	//K3
	CalcPhaseDerivative(m_Time + m_step / 2, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
	{
		K3[j] = dph_i[j];
		aux[j] = m_Phase[i_particle][j] + K3[j] * m_step;
	}
	//K4
	CalcPhaseDerivative(m_Time + m_step, aux, i_particle, dph_i);
	for (j = 0; j < 6; j++)
		K4[j] = dph_i[j];

	for (j = 0; j < 6; j++)
		solution[j] = m_Phase[i_particle][j] + (K1[j] + 2 * K2[j] + 2 * K3[j] + K4[j])*m_step / 6;
}
//----------------------------------------------------------------//
void tkSimulator::CalcPhaseDerivative(float time, 
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
void tkSimulator::SimStep()
{
	int i, j;
	float x_i[3],	// i-th position
		v_i[3],		// i-th velocity
		solution[6];

	ForceCalculus();
	UpdatePhase();

	// SolveODE_Euler();

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
		m_Particles[i].SetPosition(x_i);
		m_Particles[i].SetVelovity(v_i);
	}

	UpdateTime();
}
//----------------------------------------------------------------//