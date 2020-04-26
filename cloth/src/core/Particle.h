#pragma once

class CParticle
{
public:
	CParticle(void)
		:Mass(1.0f)
	{
		for (int i = 0; i < 3; i++)
			Position[i] = Velocity[i] = Force[i] = 0.0f;

		UV[0] = UV[1] = 0.0f;
	}
	virtual ~CParticle(void) {

	}

	//Funciones SET
	inline void SetMass(float m) { Mass = m; }
	inline void SetRadius(float r) { Radius = r; }
	inline void SetPosition(float *p) {
		for (int i = 0; i < 3; i++) { Position[i] = p[i]; }
	}
	inline void SetUV(float *uv) {
		for (int i = 0; i < 2; i++) { UV[i] = uv[i]; }
	}
	inline void SetVelovity(float *v) {
		for (int i = 0; i < 3; i++) { Velocity[i] = v[i]; }
	}
	inline void SetForce(float *f) {
		for (int i = 0; i < 3; i++) { Force[i] = f[i]; }
	}
	inline void AddVelocity(float *dv) {
		for (int i = 0; i < 3; i++) { Velocity[i] += dv[i]; }
	}

	void AddForce(float *df) {
		for (int i = 0; i < 3; i++) { Force[i] += df[i]; }
	}

	inline float* GetPosition() { return Position; }
	inline float* GetUV() { return UV; }
	inline float* GetVelocity() { return Velocity; }
	inline float* GetForce() { return Force; }
	inline float GetMass() { return Mass; }
	inline float GetRadius() { return Radius; }

private:
	float Position[3];//posición coordenadas globales
	float UV[2];//coordenadas UV
	float Velocity[3];//velocidad
	float Force[3];//fuerzas
	float Mass;//masa
	float Radius;
};