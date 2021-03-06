#pragma once
#include "Actor.h"
#include "SkeletalMeshComponent.h"
#include "ShaderTag.h"

class Gun : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">オフセット位置</param>
	/// <param name="rot">オフセット回転</param>
	/// <param name="gpmeshFileName">gpmeshのファイルパス</param>
	/// <param name="skelComp">SkeletalMeshComponentのポインタ</param>
	/// <param name="boneName">持たせたいボーン名</param>
	Gun(const Vector3& pos,const Vector3& rot,
	    const char* gpmeshFileName,SkeletalMeshComponent* skelComp,
		const char* boneName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Gun();

	void UpdateActor(float deltaTime)override;

// ゲッター //
	bool GetFireFlag() { return mFireFlag; }
private:
	// アタッチ先のボーン名
	const char* mBoneName;

	// 射撃フラグ
	bool mFireFlag;

	// SkeletalMeshComponentクラスのポインタ
	class SkeletalMeshComponent* mSkelComp;

	// AttachMeshComponentクラスのポインタ(武器のアタッチメント用)
	class AttachMeshComponent* mAttachComp;

	// 適用するシェーダーのタグ
	ShaderTag mShaderTag;
};