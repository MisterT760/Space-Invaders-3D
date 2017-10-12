//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;
	if (clipNumber < GetClipCount())
	{
		auto clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
	else
	{
		Reset(true);
		Logger::LogWarning(L"ModelAnimation -> Specified ClipNumber doesn't exist! Animation Reset");
		return;
	}

	//Set m_ClipSet to false
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
	//	Call Reset
	//	Log a warning with an appropriate message
	//	return
	//else
	//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
	//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	m_ClipSet = false;
	for (auto clip : m_pMeshFilter->m_AnimationClips)
	{
		if (clip.Name == clipName)
		{
			SetAnimation(clip);
			return;
		}
	}

	Reset();
	Logger::LogWarning(L"ModelAnimation -> Specified ClipName doesn't exist! Animation Reset");

	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;

	Reset(false);

	//Set m_ClipSet to true
	//Set m_CurrentClip
	
	//Call Reset(false)
}

void ModelAnimator::Reset(bool pause)
{
	if (pause)
		m_IsPlaying = false;
	
	m_TickCount = 0;
	m_AnimationSpeed = 1.0f;

	if (m_ClipSet)
	{
		auto boneTrans = m_CurrentClip.Keys[0].BoneTransforms;
		m_Transforms.assign(boneTrans.begin(), boneTrans.end());
	}
	else
	{
		auto identityMat = XMMatrixIdentity();
		XMFLOAT4X4 identity;
		XMStoreFloat4x4(&identity, identityMat);
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
	}

	//If pause is true, set m_IsPlaying to false
	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f
	
	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		if (m_Reversed == true)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount += m_CurrentClip.Duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.Duration)
			{
				m_TickCount -= m_CurrentClip.Duration;
				m_IsPlaying = false;
			}
		}	

		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		
		//3.
		//Find the enclosing keys
		AnimationKey keyA = m_CurrentClip.Keys[0], keyB = m_CurrentClip.Keys[m_CurrentClip.Keys.size() - 1];
		for (auto key : m_CurrentClip.Keys)
		{
			if (keyA.Tick < key.Tick && key.Tick < m_TickCount )
			{
				keyA = key;
			}
			else if (keyB.Tick > key.Tick && key.Tick > m_TickCount)
			{
				keyB = key;
			}
		}

		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		
		//4.
		auto length = keyB.Tick - keyA.Tick;
		auto blendFactorB = 1.0f - ((keyB.Tick - m_TickCount) / length);

		m_Transforms.clear();

		for (UINT i = 0; i < keyB.BoneTransforms.size(); ++i)
		{
			auto transformA = XMLoadFloat4x4(&keyA.BoneTransforms[i]);
			auto transformB = XMLoadFloat4x4(&keyB.BoneTransforms[i]);

			XMVECTOR decomPosA, decomPosB, decomRotA, decomRotB, decomScaleA, decomScaleB;
			XMMatrixDecompose(&decomScaleA, &decomRotA, &decomPosA, transformA);
			XMMatrixDecompose(&decomScaleB, &decomRotB, &decomPosB, transformB);

			auto pos = XMMatrixTranslationFromVector(XMVectorLerp(decomPosA, decomPosB, blendFactorB));
			auto scale = XMMatrixScalingFromVector(XMVectorLerp(decomScaleA, decomScaleB, blendFactorB));
			auto rot = XMMatrixRotationQuaternion(XMQuaternionSlerp(decomRotA, decomRotB, blendFactorB));

			XMMATRIX transformMatrix = scale * rot * pos;
			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, transformMatrix);

			m_Transforms.push_back(transform);
		}

		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
	}
}
