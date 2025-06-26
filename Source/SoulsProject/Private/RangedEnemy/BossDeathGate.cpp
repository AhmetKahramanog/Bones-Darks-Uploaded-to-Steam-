#include "RangedEnemy/BossDeathGate.h"
#include "BaseEnemy.h"

ABossDeathGate::ABossDeathGate()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABossDeathGate::BeginPlay()
{
	Super::BeginPlay();

	InitilazeLocation = GetActorLocation();
	
}

void ABossDeathGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Boss)
	{
		if (Boss->IsBossDie)
		{
			FVector TargetLocation = InitilazeLocation + FVector(0.f, 0.f, 250.f);
			FVector MoveWall = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 0.5f);
			SetActorLocation(MoveWall);
		}
	}

}

