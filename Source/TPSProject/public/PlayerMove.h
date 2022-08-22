// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	//좌우 회전 입력 처리
	void Turn(float value);
	//상하 회전 입력 처리
	void LookUp(float value);

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	//이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 200;
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float runSpeed = 600;

	FVector direction;

	//좌우 이동 입력 이벤트 처리 함수
	void InputHorizontal(float value);
	//상하 이동 입력 이벤트 처리 함수
	void InputVertical(float value);
	//점프 입력
	void InputJump();
	//플레이어 이동 처리
	void Move();
	//달리기 함수
	void InputRun();
};
