// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 등록
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));		//충돌 프로파일
	collisionComp->SetSphereRadius(13);								//크기
	RootComponent = collisionComp;									//루트로 지정
	//외관 컴포넌트 등록
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);			//루트는 collisionComp
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//충돌체 해제
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));		//크기
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMeshComp->SetStaticMesh(TempMesh.Object);
		bodyMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -12.5f));
	}

	//발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);		//갱신시킬 컴포넌트

	movementComp->InitialSpeed = 5000;						//초기속도
	movementComp->MaxSpeed = 5000;							//최대속도
	movementComp->bShouldBounce = true;						//반동여부
	movementComp->Bounciness = 0.3f;						//반동 값

	//생명 주기
	InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

