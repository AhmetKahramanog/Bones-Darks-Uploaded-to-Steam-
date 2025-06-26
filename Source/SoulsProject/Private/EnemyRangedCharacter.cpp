#include "EnemyRangedCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "RangedEnemy/Arrow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Components/PoseableMeshComponent.h"
#include <EnemyRangedCharacterAnimInstance.h>
#include <Kismet/GameplayStatics.h>


AEnemyRangedCharacter::AEnemyRangedCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bow"));
	BowMesh->SetupAttachment(GetMesh(), FName("BowSocket"));

	ArrowSpawnSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowSpawnSocket"));
	ArrowSpawnSocket->SetupAttachment(GetMesh(),FName("ArrowSocket"));


}

void AEnemyRangedCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	GetWorld()->GetTimerManager().SetTimer(RangedAttackInitialize, this, &AEnemyRangedCharacter::RangedAttack, 3.f, true);

	if (AnimInstance)
	{
		RangedAnimInstance = Cast<UEnemyRangedCharacterAnimInstance>(AnimInstance);
	}
	
}

void AEnemyRangedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDie) { return; }

	
	if (Distance() < FollowRange)
	{
		BaseMovement(DeltaTime, FollowSpeed);
		RangerLook(DeltaTime);
	}
}

void AEnemyRangedCharacter::BaseMovement(float DeltaTime,float Speed)
{
	Super::BaseMovement(DeltaTime,Speed);
}

void AEnemyRangedCharacter::RangerLook(float DeltaTime)
{
	if (AnimInstance)
	{
		UEnemyRangedCharacterAnimInstance* RangerEnemyAnimInstance = Cast<UEnemyRangedCharacterAnimInstance>(AnimInstance);
		if (RangerEnemyAnimInstance)
		{
			FVector TargetLoc = PlayerCharacter->GetActorLocation();
			float AbsDirection = FMath::Abs(TargetLoc.Z - GetActorLocation().Z);
			//BaseMovement(DeltaTime,0.f);
			if (AbsDirection > 40.f)
			{
				RangerEnemyAnimInstance->AlphaValue = 0.8f;
				RangerEnemyAnimInstance->UpperBodyDirection = TargetLoc;
			}
			else
			{
				RangerEnemyAnimInstance->AlphaValue = 0.f;
			}
		}
	}
}

void AEnemyRangedCharacter::EnemyTakeDamage(float amount,float StunnedTimer)
{
	Super::EnemyTakeDamage(amount,StunnedTimer);
	if (!AnimInstance) { return; }
	if (CurrentHealth > 0)
	{
		AnimInstance->Montage_Play(HitReactionMontage);
	}
	AnimInstance->Montage_Stop(0.5f, BeginAttack);
	AnimInstance->Montage_Stop(0.5f, AttackIdle);
	IsAttackIdle = false;
	if (SpawnedArrow)
	{
		SpawnedArrow->Destroy();
	}
}

void AEnemyRangedCharacter::RangedAttack()
{
	if (AnimInstance && !IsAttackIdle && Distance() < AttackIntervalRange && !IsStunned && !IsDie)
	{
		AnimInstance->Montage_Play(BeginAttack,0.7f);
	}
}

void AEnemyRangedCharacter::AttackIdleAnimation()
{
	AnimInstance->Montage_Play(AttackIdle, 0.7f);
	SpawnArrow();
	IsAttackIdle = true;
}

void AEnemyRangedCharacter::RangedEndAttack()
{
	AnimInstance->Montage_Play(AttackEnd, 0.7f);
	IsAttackIdle = false;
	FireArrow();
	IsFollowArrow = true;
}

void AEnemyRangedCharacter::Respawn()
{
	Super::Respawn();
}

void AEnemyRangedCharacter::SpawnArrow()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("ArrowSocket"));
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(FName("ArrowSocket"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnedArrow = GetWorld()->SpawnActor<AArrow>(
		ArrowClass, SpawnLocation, SpawnRotation, SpawnParams
	);
	if (SpawnedArrow)
	{
		SpawnedArrow->ArrowMesh->SetSimulatePhysics(false);
		SpawnedArrow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,FName("ArrowSocket"));
	}
}

void AEnemyRangedCharacter::FireArrow()
{
	if (SpawnedArrow)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyAttackSound, GetActorLocation());
		SpawnedArrow->ArrowMesh->SetSimulatePhysics(true);
		FVector TargetLocation = PlayerCharacter->GetActorLocation();
		FVector ArrowLocation = SpawnedArrow->GetActorLocation();
		FVector ArrowDirection = (TargetLocation - ArrowLocation).GetSafeNormal();
		SpawnedArrow->ArrowMesh->AddImpulse(ArrowDirection * ArrowForce);
	}
}


void AEnemyRangedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

