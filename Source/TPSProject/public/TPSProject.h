// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TPS, Log, All);

#define CALLINF0 (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINF0() UE_LOG(TPS, Warning, TEXT("%s"), *CALLINF0)
#define PRINT_LOG(fmt, ...) UE_LOG(TPS, Warning, TEXT("%s %s"), *CALLINF0, *FString::Printf(fmt, ##__VA_ARGS__))

