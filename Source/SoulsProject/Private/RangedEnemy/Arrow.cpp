#include "RangedEnemy/Arrow.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"
#include <EnemyRangedCharacter.h>

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	ArrowMesh->SetupAttachment(RootComponent);

	ArrowHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowHitBox"));
	ArrowHitBox->SetupAttachment(ArrowMesh);

}

void AArrow::BeginPlay()
{
	Super::BeginPlay();


	ArrowHitBox->OnComponentBeginOverlap.AddDynamic(this, &AArrow::ArrowBeginOverlap);

}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->GetDamage(ArrowDamage);
			Destroy();
		}
		else
		{
			GetWorldTimerManager().SetTimer(ArrowDestroyHandle, FTimerDelegate::CreateLambda([this]
				{
					Destroy();
				}), 3.5f, false);
		}
	}
}

