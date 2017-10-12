//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Physx/PhysxProxy.h"


ModelTestScene::ModelTestScene(void) :
	GameScene(L"ModelTestScene"),
	m_pChair(nullptr)
{
}


ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
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

	//1. Attach a modelcomponent (chair.ovm)
	auto modelComponent = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	//2. Create a ColorMaterial and add it to the material manager
	auto colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);
	//3. Assign the material to the previous created modelcomponent
	modelComponent->SetMaterial(0);
	m_pChair->AddComponent(modelComponent);
	// Build and Run
	//AddChild(m_pChair);

	//4. Create a DiffuseMaterial (using PosNormTex3D.fx)
	//		Make sure you are able to set a texture (DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile))
	//		Load the correct shadervariable and set it during the material variable update
	auto diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	//5. Assign the material to the modelcomponent
	// Build and Run
	m_pChair->GetComponent<ModelComponent>()->SetMaterial(1);

	//6. Attach a rigidbody component (pure-dynamic)
	m_pChair->AddComponent(new RigidBodyComponent());
	m_pChair->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	//7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	auto convexGeom = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	shared_ptr<PxGeometry> geometry(new PxConvexMeshGeometry(convexGeom));
	auto colliderComponent = new ColliderComponent(geometry, *bouncyMaterial);
	m_pChair->AddComponent(colliderComponent);
	// Build and Run
	AddChild(m_pChair);

	m_pChair->GetTransform()->Translate(XMFLOAT3(0.f, 10.f, 0.f));
	m_pChair->SetTag(L"Test");

	gameContext.pInput->AddInputAction(InputAction(11, InputTriggerState::Pressed, -1, VK_RBUTTON));
}

void ModelTestScene::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(11))
	{
		auto obj = gameContext.pCamera->Pick(gameContext);

		if (obj->GetTag() == L"Test")
		{
			m_pChair->GetTransform()->Scale(10.f, 10.f, 10.f);
		}
	}
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
