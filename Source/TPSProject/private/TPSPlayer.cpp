// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerMove.h"
#include "PlayerFire.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈메시 데이터 불러오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//Mesh 컴포넌트의 위치 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, -90));
	}

	//카메라 설정
	//springarm 컴포넌트 지정
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	//camrera 컴포넌트 지정
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	//2단 점프
	JumpMaxCount = 2;
	//기본총 컴포넌트
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		//위치 조정
		gunMeshComp->SetRelativeLocation(FVector(-10, 3, -2));
		gunMeshComp->SetRelativeRotation(FRotator(0, 90, 0));
	}
	//스나이퍼 컴포넌트
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1'"));

	//if (TempSniperMesh.Succeeded())
	//{
		//sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
	//위치 조정
	sniperGunComp->SetRelativeLocation(FVector(-39, 2, 2));
	sniperGunComp->SetRelativeRotation(FRotator(0, 90, 0));
	//크기 조정
	sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	//}

	//이동 컴포넌트
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	//공격 컴포넌트
	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	onInputBindingDelegate.Broadcast(PlayerInputComponent);

	/*playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);*/
}

void ATPSPlayer::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaged!!!!"));
	hp--;
	if (hp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is Dead!!!"));
		OnGameOver();
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	//게임 오버 시 일시 정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}
