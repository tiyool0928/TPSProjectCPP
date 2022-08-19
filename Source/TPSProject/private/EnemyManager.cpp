// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	//랜덤 시간
	float createTime = FMath::RandRange(minTime, maxTime);
	//Time manager 알람 등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);

	FindSpawnPoints();
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreateEnemy()
{
	//랜덤 위치
	int index = FMath::RandRange(0, spawnPoints.Num() - 1);
	//적 생성 및 배치
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));

	//반복
	//랜덤 시간
	float createTime = FMath::RandRange(minTime, maxTime);
	//Time manager 알람 등록
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
}

void AEnemyManager::FindSpawnPoints()
{
	//결과 저장 배열
	TArray<AActor*> allActors;
	//액터 찾아오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	//actor 찾았을 경우 반복스폰
	for (auto spawn : allActors)
	{
		//찾은 액터의 이름이 해당 문자열을 포함하고 있는가?
		if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			//스폰목록에 추가
			spawnPoints.Add(spawn);
		}
	}



	//for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	//{
	//	AActor* spawn = *It;
	//	//찾은 액터의 이름이 해당 문자열을 포함하고 있는가?
	//	if (spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
	//	{
	//		//스폰목록에 추가
	//		spawnPoints.Add(spawn);
	//	}
	//}
}

