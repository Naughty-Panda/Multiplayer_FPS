// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongPlayerController.h"
#include "PingPongPlatform.h"

APingPongPlayerController::APingPongPlayerController()
{
	//SetReplicates(true);
	bReplicates = true;
}

void APingPongPlayerController::SetStartTransform(const FTransform& NewTransform)
{
	StartTransform = NewTransform;
}

void APingPongPlayerController::Initialize_Implementation()
{
	if (Platform)
	{
		Platform->Destroy();
	}

	SpawnPlatform(PlatformClass);
}

bool APingPongPlayerController::Initialize_Validate()
{
	return true;
}

void APingPongPlayerController::SpawnPlatform_Implementation(TSubclassOf<APingPongPlatform> NewPlatformClass)
{
	Platform = GetWorld()->SpawnActor<APingPongPlatform>(PlatformClass);

	if (Platform)
	{
		Platform->SetActorLocation(StartTransform.GetLocation());
		Platform->SetActorRotation(StartTransform.GetRotation());
	}
}

bool APingPongPlayerController::SpawnPlatform_Validate(TSubclassOf<APingPongPlatform> NewPlatformClass)
{
	return PlatformClass != nullptr;
}

void APingPongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &APingPongPlayerController::MoveRight);
}

void APingPongPlayerController::MoveRight(float AxisValue)
{
	if (AxisValue == 0.f)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("MoveRight()"));
	Server_MoveRight(AxisValue);
}

void APingPongPlayerController::Server_MoveRight_Implementation(float AxisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_MoveRight()"));
	Platform->Server_MoveRight(AxisValue);
}

bool APingPongPlayerController::Server_MoveRight_Validate(float AxisValue)
{
	return Platform != nullptr;
}
