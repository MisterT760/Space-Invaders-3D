//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.3.133
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
#pragma once
#include "Scenegraph\GameObject.h"

// Forward declarations
class MeshIndexedDrawComponent;

class ClothPlane : public GameObject
{
public:
	ClothPlane(const UINT columns, const UINT rows, const float width, const float depth, const UINT materialID, const XMFLOAT3 initPos, const vector<UINT> &fixedVerts = {});
	~ClothPlane();

	PxCloth* GetCloth() { return m_pCloth; }

protected:
	virtual void Initialize(const GameContext & gameContext);
	virtual void Update(const GameContext & gameContext);

private:

	void CreateMeshDrawer(const GameContext& gameContext);
	void AddQuad(MeshIndexedDrawComponent* mesh, const UINT a, const UINT b, const UINT c, const UINT d);
	void AddAdjacencyQuad(MeshIndexedDrawComponent* mesh, const UINT a, const UINT b, const UINT c, const UINT d, const UINT ae, const UINT be, const UINT ce, const UINT de);

	void CreateCloth();

	void FillMeshDrawer(const GameContext & gameContext, MeshIndexedDrawComponent* meshDrawer, PxCloth *cloth);

	UINT m_Columns, m_Rows;
	float m_Width, m_Depth;
	XMFLOAT3 m_InitialPose;
	UINT m_NrOfVertices, m_NrOfIndices;
	UINT m_MaterialID;
	vector<UINT> m_FixedVerts;

	MeshIndexedDrawComponent* m_pMeshDrawer;
	PxCloth* m_pCloth;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ClothPlane(const ClothPlane& t);
	ClothPlane& operator=(const ClothPlane& t);
};

