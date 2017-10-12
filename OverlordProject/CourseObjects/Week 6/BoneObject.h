#pragma once
#include "Scenegraph/GameObject.h"

class BoneObject : public GameObject
{
public:
	BoneObject(int boneId, int materialId, float lenght = 5.0f);
	~BoneObject(void);

	void AddBone(BoneObject* pBone);
	XMFLOAT4X4 GetBindPose() { return m_BindPose; }
	void CalculateBindPose();

protected:

	virtual void Initialize(const GameContext& gameContext);

private:
	float m_Lenght;
	int m_BoneId;
	int m_MaterialId;
	XMFLOAT4X4 m_BindPose;

private:
	// -------------------------  
	// Disabling default copy constructor and default   
	// assignment operator.  
	// -------------------------  
	BoneObject(const BoneObject& yRef) = delete;  
	BoneObject& operator=(const BoneObject& yRef) = delete; 
};

