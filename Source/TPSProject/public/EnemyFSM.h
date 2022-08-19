// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle, Move, Attack, Damage, Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//상태변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState mState = EEnemyState::Idle;
	
	void IdleState();			//대기상태
	void MoveState();			//이동상태
	void AttackState();			//공격상태
	void DamageState();			//피격상태
	void DieState();			//죽음상태
	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 2;
	//경과시간
	float currentTime = 0;
	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class ATPSPlayer* target;
	//액터 컴포넌트 소유 액터(적 자신)
	UPROPERTY()
		class AEnemy* me;
	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;
	//공격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//피격 알림 이벤트 함수
	void OnDamageProcess();
	//적 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		int32 hp = 3;
	//피격 대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float damageDelayTime = 2.0f;
	//아래로 내려가는 속도
	float dieSpeed = 50.0f;

	//사용 중인 애니메이션 블루프린트
	UPROPERTY()
		class UEnemyAnim* anim;

	//Enemy를 소유한 AIController
	UPROPERTY()
		class AAIController* ai;

	FVector randomPos;			//랜덤위치
	//랜덤위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);
};
