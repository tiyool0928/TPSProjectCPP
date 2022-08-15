// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드내 ATPSPlayer 찾아오기
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	target = Cast<ATPSPlayer>(actor);
	me = Cast<AEnemy>(GetOwner());
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	if (currentTime >= idleDelayTime)
	{
		//이동상태로 전환
		mState = EEnemyState::Move;
		currentTime = 0;
	}
}
void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();		//목적지
	FVector dir = destination - me->GetActorLocation();		//방향
	me->AddMovementInput(dir.GetSafeNormal());				//방향으로 이동

	//타깃과 가까워지면 공격상태로 전환
	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attack;
	}

}
void UEnemyFSM::AttackState() 
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
		currentTime = 0;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}
}
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
	}
}
void UEnemyFSM::DieState()
{
	//아래로 내려가면서 사라지는 연출
	//등속운동 P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	//2미터 이상 내려왔다면
	if (P.Z < -200.0f)
	{
		me->Destroy();
	}
	
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

