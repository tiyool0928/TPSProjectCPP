// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();

	//총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		class UCameraComponent* tpsCamComp;

	//총 스켈레탈메시
	UPROPERTY()
		class USkeletalMeshComponent* gunMeshComp;

	//스나이퍼 스태틱메시 추가
	UPROPERTY()
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
	//카메라 쉐이크
	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
		TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* bulletSound;
};
