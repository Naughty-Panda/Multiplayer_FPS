// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameModeBase.generated.h"

class APingPongPlayerController;
class APlayerStart;

/**
 * 
 */
UCLASS()
class MULTIPLAYER_FPS_API APingPongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	APingPongPlayerController* Player1 = nullptr;

	UPROPERTY()
	APingPongPlayerController* Player2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player1Start = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerStart* Player2Start = nullptr;

public:
	APingPongGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
