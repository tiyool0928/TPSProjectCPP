// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;

	//좌우 회전 입력 처리
	void Turn(float value);
	//상하 회전 입력 처리
	void LookUp(float value);

	//이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 600;

	FVector direction;

	//좌우 이동 입력 이벤트 처리 함수
	void InputHorizontal(float value);
	//상하 이동 입력 이벤트 처리 함수
	void InputVertical(float value);

	//점프 입력
	void InputJump();

	//플레이어 이동 처리
	void Move();
	//총 스켈레탈메시
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class USkeletalMeshComponent* gunMeshComp;
	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	//스나이퍼 스태틱메시 추가
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class UStaticMeshComponent* sniperGunComp;
	//유탄총 사용중?
	bool bUsingGrenadeGun = true;
	void ChangeToGrenadeGun();
	void ChangeToSniperGun();
	//스나이퍼 조준
	void SniperAim();
	bool bSniperAim = false;

	//스나이퍼 UI 공장
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;
	//스나이퍼 UI 위젯 인스턴스
	class UUserWidget* _sniperUI;

	//총알 파편 공장
	UPROPERTY(EditAnywhere, Category = BulletEffect)
		class UParticleSystem* bulletEffectFactory;

	//일반 조준 UI
	UPROPERTY(EditDefaultsOnly, category = SniperUI)
		TSubclassOf<class UUserWidget> crosshairUIFactory;
	//크로스헤어 인스턴스
	class UUserWidget* _crosshairUI;
};
