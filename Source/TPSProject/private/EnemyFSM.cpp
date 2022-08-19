// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIController.h>
#include <NavigationSystem.h>

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
	
	//UEnemyAnim* 할당
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	//AAIController 할당
	ai = Cast<AAIController>(me->GetController());
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

		//애니메이션 상태 동기화
		anim->animState = mState;
		//최초 랜덤 위치
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}
void UEnemyFSM::MoveState()
{
	FVector destination = target->GetActorLocation();		//목적지
	FVector dir = destination - me->GetActorLocation();		//방향
	//me->AddMovementInput(dir.GetSafeNormal());				//방향으로 이동(직선이동)
	

	//NavigationSystem 객체
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	//목적지 길 찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;
	//인지범위
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	//길 찾기를 위한 쿼리
	ai->BuildPathfindingQuery(req, query);
	//길 찾기 결과
	FPathFindingResult r = ns->FindPathSync(query);
	//길 찾기 성공했는가
	if (r.Result == ENavigationQueryResult::Success)
	{
		//타깃쪽으로 이동
		ai->MoveToLocation(destination);
	}
	else
	{
		//랜덤 이동
		auto result = ai->MoveToLocation(randomPos);
		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			//새로운 랜덤 위치
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);		//invoker tile radius = 500;
		}
	}

	//타깃과 가까워지면 공격상태로 전환
	if (dir.Size() < attackRange)
	{
		//길 찾기 기능 정지
		ai->StopMovement();
		mState = EEnemyState::Attack;
		//애니메이션 상태 동기화
		anim->animState = mState;
		anim->bAttackPlay = true;
		//공격 상태 전환 시 대기 시간이 바로 끝나도록 처리
		currentTime = attackDelayTime;
	}

}
void UEnemyFSM::AttackState() 
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= attackDelayTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	//공격범위를 벗어났는가?
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		//애니메이션 상태 동기화
		anim->animState = mState;
		//
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime >= damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}
void UEnemyFSM::DieState()
{
	//죽음 애니메이션이 아직 안끝났다면
	if (anim->bDieDone == false)
	{
		return;
	}
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
	anim->animState = mState;
}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		currentTime = 0;
		//피격 애니메이션 재생
		int32 index = FMath::RandRange(0, 1);		//2개 피격 애니메이션 중 랜덤 1
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	else
	{
		//죽음으로 전환
		mState = EEnemyState::Die;
		//충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽음 재생
		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;
	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

