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
	std::vector<tkVec3> x0;	// position
	std::vector<tkVec3> v0;	// velocity
	std::vector<tkVec2> uv;	// uv matrix
	std::vector<float>	mass; // mass array
	std::vector<float>	radius; // radius array

	//M = new float[n];
	//R = new float[n];

	float m = 1.0f;		// Particle mass
	float r = 0.15f;	// Particle radius;

	float posX = -10.0f, posY = 0.f, posZ = 10.0f;
	float uCoord = 0, vCoord = 0;

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
			uCoord = 0.0f;
			vCoord += 0.25f;
			i_col = 1;
		}
		else {
			i_col++;
		}
		x0.push_back(tkVec3(posX, posY, posZ));
		v0.push_back(tkVec3::ZeroVector());
		posX += 5.0f;
		uv.push_back(tkVec2(uCoord, vCoord));
		uCoord += 0.25f;
	}

	for (i = 0; i < n; i++)
	{
		//M[i] = m;
		//R[i] = r;
		mass.push_back(m);
		radius.push_back(r);
	}

	// Init cloth simulator
	InitializeSimulator(n, 0, 1);
	InitializeParticleSystem(x0, v0, mass, radius);
	InitializeClothSystem(uv);
	InitializePlaneSystem();

	// Cleaning memory
	//for (i = 0; i < n; i++)
	//{
		/*delete[] X0[i];*/	/*delete[] V0[i];*/	/*delete[] uv[i]*/;
	//}
	/*delete[] X0;*/ /*delete[] V0;*/ /*delete[] uv*/;
	/*delete[] M; delete[] R;*/
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
void tkSimulator::InitializeParticleSystem(vector<tkVec3> x,
										   vector<tkVec3> v,
										   vector<float> m,
										   vector<float> r)
{
	int i;
	if (m_nParticles > 0)
	{
		for (i = 0; i < m_nParticles; i++)
		{
			tkParticle p;
			p.SetPosition(x.at(i));
			p.SetVelovity(v.at(i));
			p.SetMass(m.at(i));
			p.SetRadius(r.at(i));
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
void tkSimulator::InitializeClothSystem(vector<tkVec2> uv)
{
	// TODO: solve triangle particle distribution algorithm

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

	int i;
	for (i = 0; i < m_nParticles; i++)
		m_Particles[i].SetUV(uv.at(i));

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
		
		glVertex3f(getCloth().getTris()[i].getA()->GetPosition().x,
				   getCloth().getTris()[i].getA()->GetPosition().y,
				   getCloth().getTris()[i].getA()->GetPosition().z);
		
		glVertex3f(getCloth().getTris()[i].getB()->GetPosition().x,
				   getCloth().getTris()[i].getB()->GetPosition().y,
				   getCloth().getTris()[i].getB()->GetPosition().z);
		
		glVertex3f(getCloth().getTris()[i].getC()->GetPosition().x,
				   getCloth().getTris()[i].getC()->GetPosition().y,
				   getCloth().getTris()[i].getC()->GetPosition().z);
		
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

	tkVec3	Fi;			// Force [i-th]
	tkVec3	Ji;			// Local impulse [i-th]
	tkVec3	dVel;		// Delta velocity to impulse [i-th]
	float	JiMag;		// Impulse magnitude
	float	kSe;		// Spring ellasticity coefficient
	float	kSd;		// Spring damping coefficient
	float	kESt;		// Stretch stiffness energy constant
	float	kESr;		// Shear stiffness energy constant
	float	kEBd;		// Bend stiffness energy constant
	float	kSf = 0.2f;	// Sphere surface friction
	tkVec3	dx1;		// Direction vector: B -> A
	tkVec3	dx2;		// Direction vector: C -> A
	float	du1 = 0.0f; // Delta UV.u1
	float	du2 = 0.0f; // Delta UV.u2
	float	dv1 = 0.0f; // Delta UV.v1
	float	dv2 = 0.0f; // Delta UV.v2
	float	detUV;		// Matrix determinant
	float	Wu[3];
	float	Wv[3];
	float	Su;
	float	Sv;
	float	uvArea;		// Triangle UV area
	float	shTheta = 0;	// Shear angle
	float	r_i;			// i-th particle radius

	tkVec3 Xs(2.0f, 30.0f, 0.0f); // sphere position
	float r_s = 10.0f;	// sphere radius
	tkVec3 XiXs;		// Direction from the particle (Xi) to sphere (Xe)
	tkVec3 nmXiXe;		// Normalized direction vector XiXe
	//float distXiXe = 0.0f; // Magnitude of vector XiXe

	tkParticle *pt_i, *pt_j, *pi_k;

	//* For all particles *//

	tkVec3 x_i;	// particle i-th position
	tkVec3 v_i;	// particle i-th velocity
	float deltaS = 0.0f;

	for (i = 0; i < m_nParticles; i++)
	{
		x_i = m_Particles[i].GetPosition();
		v_i = m_Particles[i].GetVelocity();
		r_i = m_Particles[i].GetRadius();

		// Init data for particles
		Fi.x = Fi.y = Fi.z = 0.0f;
		dVel.x = dVel.y = dVel.z = 0;
		Ji.x = Ji.y = Ji.z = 0.0f;

		// Gravity
		Fi += GRAVITY * m_Particles[i].GetMass();

		// Unitary forces
		// >> Drag
		Fi += -m_Particles[i].GetVelocity().x * cloth.GetDrag();

		//* Collision detection *//

		// Against plane H[k]
		for (k = 0; k < m_nPlanes; k++)
		{
			deltaS = (r_i - (float)fabs(m_Planes[k].Evaluate(x_i)));
			if (deltaS > 0)
			{
				Fi.x = Fi.y = Fi.z = 0.0f;
				dVel.x += (-(1 + kpRp)*v_i.x * m_Planes[k].getNormal()[0])*m_Planes[k].getNormal()[0];
				dVel.y += (-(1 + kpRp)*v_i.y * m_Planes[k].getNormal()[1])*m_Planes[k].getNormal()[1];
				dVel.z += (-(1 + kpRp)*v_i.z * m_Planes[k].getNormal()[2])*m_Planes[k].getNormal()[2];
			}
		}

		// Against the sphere
		XiXs = x_i - Xs;
		deltaS = (r_i + r_s) - XiXs.Magnitude();

		if (deltaS > 0.f)
		{
			// TODO: Null force on contact (?)
			Fi.x = Fi.y = Fi.z = 0.0f;
			nmXiXe = XiXs;
			nmXiXe.Normalize();

			float veln = tkVec3::Dot(v_i, nmXiXe);
			JiMag = -(1 + kpRs) * m_Particles[i].GetMass() * veln;

			tkVec3 vt = v_i - nmXiXe * veln;
			float vtMag = vt.Magnitude();
			if (vtMag < 0.0001f)
			{
				Ji += nmXiXe * JiMag;
			}
			else
			{
				Ji += nmXiXe * JiMag - (vt*JiMag*kSf) / vtMag;
			}
			dVel += Ji / m_Particles[i].GetMass();
		}

		// Update Newtonian forces
		m_Particles[i].SetForce(Fi);
		m_Particles[i].AddVelocity(dVel);
	}

	//* For all UV triangles *//

	for (i = 0; i < cloth.TriangleCount(); i++)
	{
		cloth.getTris()[i].GetSprings()[0].checked = false;
		cloth.getTris()[i].GetSprings()[1].checked = false;
		cloth.getTris()[i].GetSprings()[2].checked = false;
	}
	for (i = 0; i < cloth.TriangleCount(); i++)
	{
		for (j = 0; j < 3; j++)
		{
			// Forces = 0
			Fi.x = Fi.y = Fi.z = 0.0f;

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

			dx1 = pt_j->GetPosition() - pt_i->GetPosition();
			dx2 = pi_k->GetPosition() - pt_i->GetPosition();

			du1 = pt_j->GetUV().x - pt_i->GetUV().x;
			du2 = pi_k->GetUV().x - pt_i->GetUV().x;

			dv1 = pt_j->GetUV().y - pt_i->GetUV().y;
			dv2 = pi_k->GetUV().y - pt_i->GetUV().y;

			kESt = cloth.GetStretch();
			kESr = cloth.GetShear();
			kEBd = cloth.GetBend();

			//* Energy functions *//

			//detIMuv = fabs(du1 * dv2 - dv1 * du2);
			detUV = du1 * dv2 - dv1 * du2;

			if (fabs(detUV) < 0.0001f)
				detUV = 0.0001f;

			Wu[0] = (dx1.x * dv2 - dx2.x * dv1) / detUV;
			Wu[1] = (dx1.y * dv2 - dx2.y * dv1) / detUV;
			Wu[2] = (dx1.z * dv2 - dx2.z * dv1) / detUV;

			Wv[0] = (-dx1.x * du2 + dx2.x * du1) / detUV;
			Wv[1] = (-dx1.y * du2 + dx2.y * du1) / detUV;
			Wv[2] = (-dx1.z * du2 + dx2.z * du1) / detUV;

			// Normalizing
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

			Fi += x_i * (-stE);

			// SHEAR //
			shTheta = (float)acos(Wu[0] * Wv[0] + Wu[1] * Wv[1] + Wu[2] * Wv[2]);
			// Shear energy
			float shE = (kESr * 0.5f) * (shTheta * shTheta) * uvArea;

			Fi += x_i * (-shE);

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
		}

		// Link-spring S
		
		tkVec3 x_o;		// pos. of particle O from S
		tkVec3 x_p;		// pos. of particle P from S
		tkVec3 v_o;		// velocity of particle O from S
		tkVec3 v_p;		// velocity of particle P from S
		tkVec3 XoXp;	// pos. vector from O to P
		float XoXpLen;	// Length of the vector OP
		tkVec3 VoVp;	// vel. vector from O to P
		float sLen;		// spring rest length 
		float deltaL;	// delta length (small length diff.)
		float absdL;	// abs value of delta length
		float dotProduct;

		// Force calculation for the 3 triangle links
		for (j = 0; j < 3; j++)
		{
			// Check the link to be include once
			if (!cloth.getTris()[i].GetSprings()[j].checked)
			{
				// Forces = 0
				Fi.x = Fi.y = Fi.z = 0;

				x_o = cloth.getTris()[i].GetSprings()[j].getO()->GetPosition();
				x_p = cloth.getTris()[i].GetSprings()[j].getP()->GetPosition();
				v_o = cloth.getTris()[i].GetSprings()[j].getO()->GetVelocity();
				v_p = cloth.getTris()[i].GetSprings()[j].getP()->GetVelocity();
				kSe = cloth.getTris()[i].GetSprings()[j].GetElastic();
				kSd = cloth.getTris()[i].GetSprings()[j].GetDamp();
				sLen = cloth.getTris()[i].GetSprings()[j].GetLength();

				XoXp = x_o - x_p;
				VoVp = v_o - v_p;

				XoXpLen = XoXp.Magnitude();
				deltaL = XoXpLen - sLen;
				absdL = (float)fabs(deltaL);
				dotProduct = tkVec3::Dot(VoVp, XoXp);

				// Link forces
				if (deltaL > 0)
				{
					// Ellastic force
					Fi +=  (XoXp / XoXpLen) * (-deltaL / absdL * kSe * absdL);
					// Damping force
					Fi -= (XoXp / XoXpLen) * (deltaL / absdL * ((dotProduct) / XoXpLen)*kSd);
				}

				cloth.getTris()[i].GetSprings()[j].getO()->AddForce(Fi);
				Fi.Invert();
				cloth.getTris()[i].GetSprings()[j].getP()->AddForce(Fi);

				cloth.getTris()[i].GetSprings()[j].checked = true;
			}
		}
	}
}
//----------------------------------------------------------------//
void tkSimulator::UpdatePhase()
{
	int i;

	if (m_nParticles > 0)
	{
		tkVec3 x;
		tkVec3 v;
		tkVec3 f;
		float  m;

		for (i = 0; i < m_nParticles; i++)
		{
			x = m_Particles[i].GetPosition();
			v = m_Particles[i].GetVelocity();
			f = m_Particles[i].GetForce();
			m = m_Particles[i].GetMass();

			// Phase position
			m_Phase[i][0] = x.x; 
			m_Phase[i][1] = x.y; 
			m_Phase[i][2] = x.z;
			// Phase velocity
			m_Phase[i][3] = v.x; 
			m_Phase[i][4] = v.y; 
			m_Phase[i][5] = v.z;
			// Phase derivative velocity
			m_dPhase[i][0] = v.x; 
			m_dPhase[i][1] = v.y; 
			m_dPhase[i][2] = v.z;
			m_dPhase[i][3] = f.x / m;
			m_dPhase[i][4] = f.y / m;
			m_dPhase[i][5] = f.z / m;
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