// Fill out your copyright notice in the Description page of Project Settings.

#include "PingPongBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

DECLARE_LOG_CATEGORY_CLASS(LogBall, Display, Display);

// Sets default values
APingPongBall::APingPongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsMoving = false;

	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Ball Collision"));
	SetRootComponent(BallCollision);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	BallMesh->SetupAttachment(RootComponent);
	BallMesh->SetIsReplicated(true);
	SetReplicateMovement(true);
	
	//SetReplicates(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void APingPongBall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APingPongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() != ENetMode::NM_Client)
	{
		Server_Move(DeltaTime);
	}
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APingPongBall, bIsMoving);
}

void APingPongBall::StartMove()
{
	Server_StartMove();
}

void APingPongBall::StopMove()
{
	Server_StopMove();
}

void APingPongBall::Multicast_HitEffect_Implementation()
{
	const UWorld* CurrentWorld = GetWorld();

	if (CurrentWorld && HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(CurrentWorld, HitEffect, GetActorLocation());
	}
}

void APingPongBall::Server_StopMove_Implementation()
{
	bIsMoving = false;
}

bool APingPongBall::Server_StopMove_Validate()
{
	return true;
}

void APingPongBall::Server_StartMove_Implementation()
{
	bIsMoving = true;
}

bool APingPongBall::Server_StartMove_Validate()
{
	return true;
}

void APingPongBall::Server_Move_Implementation(float DeltaTime)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector CurrentLocation = GetActorLocation();
	const FVector NewLocation = CurrentLocation + ForwardVector * MoveSpeed * DeltaTime;

	FHitResult HitResult;
	if (!SetActorLocation(NewLocation, true, &HitResult))
	{
		UE_LOG(LogBall, Display, TEXT("Ball %s collided with %s"), *GetName(), *HitResult.GetActor()->GetName());

		FVector MoveVector = ForwardVector - CurrentLocation;
		MoveVector.Normalize();

		const FVector ResetPosition = CurrentLocation + MoveVector * MoveSpeed * DeltaTime * 5.f;
		DrawDebugDirectionalArrow(GetWorld(), NewLocation + MoveVector * 300.f, NewLocation, 30.f, FColor::Yellow, true, 3.f, 0, 3.f);

		const FVector ImpactCorrection = HitResult.ImpactPoint + HitResult.ImpactNormal * 300.f;
		DrawDebugDirectionalArrow(GetWorld(), HitResult.ImpactPoint, ImpactCorrection, 30.f, FColor::Orange, true, 3.f, 0, 3.f);

		const float AimAtAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(MoveVector, HitResult.ImpactNormal)));

		MoveVector = MoveVector.RotateAngleAxis(AimAtAngle * 2.f, FVector(0.,0.,1.));
		FVector NewTargetMove = NewLocation + MoveVector * 300.f;
		NewTargetMove.Z = CurrentLocation.Z;
		DrawDebugDirectionalArrow(GetWorld(), NewLocation, NewTargetMove, 30.f, FColor::Red, true, 3.f, 0, 3.f);

		SetActorLocation(ResetPosition);

		const FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, NewTargetMove);
		NewRotation.Pitch = 0.f;
		NewRotation.Roll = 0.f;

		SetActorRotation(NewRotation);

		Multicast_HitEffect();
	}
}

bool APingPongBall::Server_Move_Validate(float DeltaTime)
{
	return true;
}
