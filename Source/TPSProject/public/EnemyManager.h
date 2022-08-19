// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//스폰 랜덤 시간
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float minTime = 1;
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		float maxTime = 5;
	//스폰 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TArray<class AActor*> spawnPoints;
	//AEnemy 타입의 블루프린트 할당받는 변수
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
		TSubclassOf<class AEnemy> enemyFactory;
	//스폰 타이머
	FTimerHandle spawnTimerHandle;
	//적 생성 함수
	void CreateEnemy();
	//스폰 위치 동적 할당
	void FindSpawnPoints();
};
