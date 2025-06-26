#include "AnimNotifies/MyCharacterAnimNotify.h"
#include <Kismet/GameplayStatics.h>
#include <MyCharacter.h>

void UMyCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		AMyCharacter* Character = Cast<AMyCharacter>(MeshComp->GetOwner());
		if (Character)
		{
			FVector Location = Character->GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootStepSound, Location,0.2f);
		}
	}
}
