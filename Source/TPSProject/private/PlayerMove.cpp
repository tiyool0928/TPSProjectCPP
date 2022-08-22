// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove()
{
	//Tick함수
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	//초기속도 걷기속도
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();
}

void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);

	//좌우 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	//상하 입력 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);

	//점프 입력 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);
	//달리기 입력
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::InputHorizontal(float value)
{
	direction.Y = value;
}

void UPlayerMove::InputVertical(float value)
{
	direction.X = value;
}

void UPlayerMove::InputJump()
{
	me->Jump();
}

void UPlayerMove::Move()
{
	//플레이어 이동 (등속 운동) = P(결과 위치) = P0(현재 위치) + v(속도) * t(시간);
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;*/
	//SetActorLocation(P);
	me->AddMovementInput(direction);
	//direction을 0으로 초기화
	direction = FVector::ZeroVector;
}

void UPlayerMove::InputRun()
{
	auto movement = moveComp;
	//현재 상태가 달리기라면
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		//걷기로 전환
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}
