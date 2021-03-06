#include "Bullet.h"
#include "EffectComponent.h"
#include "BoxCollider.h"

BulletActor::BulletActor(const Vector3& pos, const Vector3& dir, Tag type, float speed, float range)
	:Actor(type)
	, mStartPos(pos)
	, mShootRange(range)
{
	mPosition = pos;
	mDirection = dir;
	mSpeed = speed;

	// 弾エフェクト生成
	EffectComponent* ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"Assets/Effect/bullet.efk");

	// 弾当たり判定エリア作成＆BoxCollider登録
	AABB box;
	box.mMin = Vector3(-30, -30, -30);
	box.mMax = Vector3(30, 30, 30);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);
}

BulletActor::~BulletActor()
{
}

void BulletActor::UpdateActor(float deltaTime)
{
	mPosition += mSpeed * deltaTime * mDirection;

	// 一定距離離れたら弾を消す
	Vector3 length = mPosition - mStartPos;
	if (length.LengthSq() > mShootRange * mShootRange)
	{
		mState = Actor::EDead;
	}

	mRecomputeWorldTransform = true;
}

void BulletActor::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	// 衝突情報
	CollisionInfo info;

	// 背景と衝突したか
	if (colliderTag == Tag::BackGround)
	{
		// 背景Boxに衝突したのもBox？
		if (ownCollider->GetColliderType() == ColliderTypeEnum::Box
			|| colliderTag == Tag::Enemy)
		{
			//ヒットボックス？
			if (ownCollider == mHitBox)
			{
				mState = State::EDead;
			}
		}
	}
}