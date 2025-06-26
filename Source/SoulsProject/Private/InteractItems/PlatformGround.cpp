#include "InteractItems/PlatformGround.h"
#include "InteractItems/InteractElevator.h"
#include "Components/BoxComponent.h"
#include <MyCharacter.h>

APlatformGround::APlatformGround()
{
	PrimaryActorTick.bCanEverTick = true;

	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformGround"));
	GroundMesh->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(GroundMesh);

}

void APlatformGround::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APlatformGround::OnBeginCollider);
	
}



void APlatformGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsMoving)
	{
		FVector TargetLocation = InitialLocation + FVector(0.f, 0.f, 1405.f);
		FVector TargetToMove = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 0.5f);
		SetActorLocation(TargetToMove);
		if (GetActorLocation().Z >= 1400.f)
		{
			IsMoving = false;
		}
	}
	else if (Elevator && !Elevator->CanMovingPlatform)
	{
		FVector TargetToMove = FMath::VInterpTo(GetActorLocation(), InitialLocation, DeltaTime, 0.5f);
		SetActorLocation(TargetToMove);
	}
}

void APlatformGround::OnBeginCollider(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Elevator)
	{
		if (OtherActor)
		{
			AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
			if (Character)
			{
				if (Elevator->CanMovingPlatform)
				{
					GetWorldTimerManager().SetTimer(MovingHandle, FTimerDelegate::CreateLambda([this]
						{
							IsMoving = true;
						}), 1.5f, false);
				}
			}
		}
	}
}

