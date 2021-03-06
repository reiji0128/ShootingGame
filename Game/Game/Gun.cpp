#include "Gun.h"
#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "Mesh.h"
#include "AttachMeshComponent.h"

Gun::Gun(const Vector3& pos, const Vector3& rot, 
	     const char* gpmeshFileName, SkeletalMeshComponent* skelComp,
	     const char* boneName)
	:Actor(Tag::Weapon)
	,mBoneName(nullptr)
	,mSkelComp(nullptr)
	,mAttachComp(nullptr)
	,mShaderTag(ShaderTag::DepthmapAndShadowMap)
{
	mBoneName = boneName;
	mSkelComp = skelComp;

	//メッシュのセット
	Mesh* mesh = RENDERER->GetMesh(gpmeshFileName);
	mAttachComp = new AttachMeshComponent(this, mSkelComp, mBoneName,mShaderTag);
	mAttachComp->SetMesh(mesh);
	mAttachComp->SetOffsetPosition(pos);
	mAttachComp->SetOffsetRotation(rot);
}

Gun::~Gun()
{
}

void Gun::UpdateActor(float deltaTime)
{
	/*if (INPUT_INSTANCE.IsKeyPressed(KEY_SPACE))
	{
		mFireFlag = true;
	}*/
}
