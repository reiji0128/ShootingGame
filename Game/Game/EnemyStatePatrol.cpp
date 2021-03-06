#include "EnemyStatePatrol.h"
#include "Game.h"

EnemyStatePatrol::EnemyStatePatrol()
	:mAimVec(0, 0, 0)
	, mRotateNow(false)
{
}

EnemyStatePatrol::~EnemyStatePatrol()
{
}

EnemyState EnemyStatePatrol::Update(EnemyActor* owner, float deltaTime)
{
	Vector3 dir = owner->GetDirection();

	//回転中か?
	if (mRotateNow)
	{
		// 目標角度に近づいたら回移転終了・直進へ
		if (isNearAngle(dir, mAimVec))
		{
			dir = mAimVec;
			mRotateNow = false;
			owner->SetDirection(dir);
			owner->SetVelosity(dir * patrolSpeed * deltaTime);
			return EnemyState::STATE_PATROL;
		}
		// 回転
		dir = (zRotateToAimVec(dir, mAimVec, rotateSpeed * deltaTime));
		owner->SetDirection(dir);
		owner->RotateToNewForward();
	}

	// 敵キャラの視界&範囲に入ったら追尾モードへ
	Vector3 playerPos = GAMEINSTANCE.GetFirstActor(Tag::Altar)->GetPosition();
	enemyToPlayerVec = playerPos - owner->GetPosition();
	float dot = Vector3::Dot(enemyToPlayerVec, dir);

	if (dot > 0 && enemyToPlayerVec.Length() < trackingRange)
	{
		return EnemyState::STATE_RUN;
	}

	//プレイヤーの攻撃を受けたか
	if (owner->GetIsHitTrig())
	{
		return EnemyState::STATE_DEATH;
	}

	// BGTriggerが背景に触れたら回転モードに変更
	if (owner->GetIsBGTrig())
	{
		// 90度時計回りに方向転換&回転モードに移行
		mRotateNow = true;
		mAimVec = Vector3::Cross(Vector3::UnitZ, dir);
		owner->SetVelosity(Vector3::Zero);
	}

	owner->SetDirection(dir);

	// 前進
	owner->SetVelosity(dir * patrolSpeed * deltaTime);

	return EnemyState::STATE_PATROL;
}

void EnemyStatePatrol::Enter(EnemyActor* owner, float deltaTime)
{
	//パトロール状態のアニメーション再生
	mSkelMeshComp = owner->GetSkeltalMeshComp();
	mSkelMeshComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_PATROL));

	mRotateNow = false;
}