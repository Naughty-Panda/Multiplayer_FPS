// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

class APingPongPlatform;
/**
 * 
 */
UCLASS()
class MULTIPLAYER_FPS_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	FTransform StartTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<APingPongPlatform> PlatformClass;

	UPROPERTY()
	APingPongPlatform* Platform;

public:
	APingPongPlayerController();

	UFUNCTION()
	void SetStartTransform(const FTransform& NewTransform);

	UFUNCTION(Server, Reliable, WithValidation)
	void Initialize();

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnPlatform(TSubclassOf<APingPongPlatform> NewPlatformClass);

	virtual void SetupInputComponent() override;

protected:
	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);
};
