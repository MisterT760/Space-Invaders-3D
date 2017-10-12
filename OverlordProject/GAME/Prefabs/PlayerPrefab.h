#pragma once
#include "Scenegraph/GameObject.h"

class ControllerComponent;
class LaserPrefab;
class MeshMaterial;
class ParticleEmitterComponent;

class PlayerPrefab : public GameObject
{
public:
	enum PlayerMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		SHOOTPULSE
	};


	PlayerPrefab();
	~PlayerPrefab();

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

	LaserPrefab* ShootLeft(const GameContext& gameContext, const XMFLOAT3 &direction, const float &speed);
	LaserPrefab* ShootRight(const GameContext& gameContext, const XMFLOAT3 &direction, const float &speed);
	bool IsDead() const { return m_IsDead; }

	//Getters
	float GetHealth() const { return m_Health; }
	float GetShield() const { return m_Shield; }
	float GetInterval()  const { return m_Interval; }

	//Setters
	void SetHealth(float health) { m_Health = health; }
	void SetShield(float shield) { m_Shield = shield; }
	void SetInterval(float interval) { m_Interval = interval; }
	void SetTimer(float time) { m_Timer = time; }

	void Damage (float damage);
	template<class T> void RemoveFromVec(T* ptr, vector<T*>& vec);

protected:

	ControllerComponent* m_pController;

	//Movement
	float m_MaxVelocity,
		m_AccelerationTime,
		m_Acceleration,
		m_Velocity;

	float m_Interval = 0.5f, m_Timer = 0.0f, m_Health = 100.f, m_Shield = 100.f;
	bool m_IsDead = false;
	MeshMaterial* m_pMat = nullptr;

	ParticleEmitterComponent* m_pParticleLeft, *m_pParticleRight;

private:
	PlayerPrefab(const PlayerPrefab& t) = delete;
	PlayerPrefab& operator=(const PlayerPrefab& t) = delete;
};

template <class T>
void PlayerPrefab::RemoveFromVec(T* ptr, vector<T*>& vec)
{
	if (ptr)
	{
		for (typename vector<T*>::iterator it = vec.begin(); it != vec.end();)
		{
			if (ptr == *it)
			{
				RemoveChild(*it);
				it = vec.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}
