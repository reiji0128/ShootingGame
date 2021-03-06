#pragma once
#include "EnemyStateBase.h"

class EnemyStateAttack : public EnemyStateBase
{
public:
	EnemyStateAttack();
	~EnemyStateAttack();

	EnemyState Update(class EnemyActor* owner, float deltaTime)override;
	void Enter(class EnemyActor* owner, float deltaTime)override;

private:
	void Attack(class EnemyActor* owner, float deltaTime);

	int mRandomNum;

	const float retrackingRange = 250.0f;      // 攻撃中から追跡に変わる距離
	const float attackOffsetTime = 0.5f;       // 攻撃アニメ開始してから攻撃判定が発生するまでの調整時間

	SkeletalMeshComponent* mSkelMeshComp;
};