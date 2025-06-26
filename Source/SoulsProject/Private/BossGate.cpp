#include "BossGate.h"
#include "Components/BoxComponent.h"
#include <MyCharacter.h>
#include <InteractableKey.h>
#include "BaseEnemy.h"
#include <Components/SlateWrapperTypes.h>

ABossGate::ABossGate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossGate::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	OpenLocation = InitialLocation + FVector(0.f, 0.f, 600.f);

	if (AssignedKey)
	{
		AInteractableKey* KeyActor = Cast<AInteractableKey>(AssignedKey);
		if (KeyActor)
		{
			KeyActor->OnKeyPickedUp.AddDynamic(this, &ABossGate::HandleBossGateOpen);
		}
	}

}

void ABossGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOpen)
	{
		MoveGate(DeltaTime, OpenLocation);
	}
	else
	{
		MoveGate(DeltaTime, InitialLocation);
	}

}

void ABossGate::HandleBossGateOpen(AInteractableKey* PickedKey)
{
	if (PickedKey)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("KeyName : %s"), *PickedKey->GetName()));
		CanOpenBossGate = true;
	}
}

void ABossGate::MoveGate(float DeltaTime, FVector TargetLocation)
{
	FVector CurrentLocation = GetActorLocation();
	FVector SmoothUpMove = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 2.f);
	SetActorLocation(SmoothUpMove);
	
}

void ABossGate::Interact()
{
	if (!CanOpenBossGate || !BossEnemy) { return; }

	IsOpen = true;
	GetWorld()->GetTimerManager().SetTimer(DestroyGateHandle, FTimerDelegate::CreateLambda([this]
	{
		IsOpen = false;
		BossEnemy->BossHealthBarActive(ESlateVisibility::Visible);
	}), 5.5f, false);
}



