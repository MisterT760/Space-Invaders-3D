//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "UberTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Physx/PhysxProxy.h"
#include "../../Materials/UberMaterial.h"


UberTestScene::UberTestScene(void) :
	GameScene(L"UberTestScene"),
	m_pChair(nullptr)
{
}


UberTestScene::~UberTestScene(void)
{
}

void UberTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto bouncyMaterial = physX->createMaterial(0.f, 0.f, 0.1f);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	auto modelComponent = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	
	modelComponent->SetMaterial(0);
	m_pChair->AddComponent(modelComponent);

	auto uberMaterial = new UberMaterial();
	uberMaterial->EnableSpecularPhong(true);
	uberMaterial->EnableDiffuseTexture(true);
	uberMaterial->EnableEnvironmentMapping(false);
	uberMaterial->EnableFresnelFalloff(false);
	uberMaterial->EnableNormalMapping(false);
	uberMaterial->EnableOpacityMap(false);
	uberMaterial->EnableSpecularBlinn(false);
	uberMaterial->EnableSpecularLevelTexture(false);
	uberMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	uberMaterial->SetSpecularColor(XMFLOAT4(Colors::White));

	gameContext.pMaterialManager->AddMaterial(uberMaterial, 1);

	m_pChair->GetComponent<ModelComponent>()->SetMaterial(1);

	m_pChair->AddComponent(new RigidBodyComponent());
	auto convexGeom = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	shared_ptr<PxGeometry> geometry(new PxConvexMeshGeometry(convexGeom));
	auto colliderComponent = new ColliderComponent(geometry, *bouncyMaterial);
	m_pChair->AddComponent(colliderComponent);
	AddChild(m_pChair);

	m_pChair->GetTransform()->Translate(XMFLOAT3(0.f, 10.f, 0.f));
}

void UberTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void UberTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
