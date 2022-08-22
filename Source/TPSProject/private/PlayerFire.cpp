// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerAnim.h"
#include "Bullet.h"

UPlayerFire::UPlayerFire()
{
	//총알 사운드
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/SniperGun/Rifle'"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	//스나이퍼 UI
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	//일반 조준 UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();

	//기본 스나이퍼 건
	ChangeToGrenadeGun();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	//발사 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	//총 교체 바인딩
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeToSniperGun);
	//스나이퍼 조준
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperAim);
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);
}

void UPlayerFire::InputFire()
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
	//카메라 쉐이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);
	//공격애니메이션 재생
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else			//스나이퍼건 사용 시
	{
		//line trace 시작 위치
		FVector startPos = tpsCamComp->GetComponentLocation();
		//종료 위치 (시작 위치 + 바라보는 방향 * 5000)
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		FHitResult hitInfo;					//충돌 정보
		FCollisionQueryParams params;		//충돌 옵션 설정
		params.AddIgnoredActor(me);		//본인은 충돌 제외

		//channel 이용 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		if (bHit)
		{
			FTransform bulletTrans;
			//부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			//총알 파편 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			//충돌된 컴포넌트 저장
			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())	//물리 적용이 되어있는가?
			{
				//F = ma, 맞은 면의 벡터 반대방향으로 물리적용
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000;
				hitComp->AddForce(force);
			}
			//충돌 대상이 적이면
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

void UPlayerFire::ChangeToGrenadeGun()
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void UPlayerFire::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void UPlayerFire::SniperAim()
{
	if (bUsingGrenadeGun)
	{
		return;
	}

	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		//일반 조준 UI 제거
		_crosshairUI->RemoveFromParent();
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		//일반 조준 UI 등록
		_crosshairUI->AddToViewport();
	}
}
