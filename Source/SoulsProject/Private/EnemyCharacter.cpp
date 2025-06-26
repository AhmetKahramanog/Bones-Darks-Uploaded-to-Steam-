#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <MyCharacter.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}



void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = GetMesh()->GetAnimInstance();

	GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, this, &AEnemyCharacter::MeleeAttack, 1.5f, true);

	GetWorldTimerManager().SetTimer(RandomMovementTimer, this, &AEnemyCharacter::SetNewRandomTarget, 12.f, true);

}


void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDie) { return; }

	if (!IsStunned && !IsAttacking)
	{
		Movement(DeltaTime);
	}

	if (FMath::Abs(GetVelocity().Size()) > 4.f)
	{
		StopAttackMontage();
	}
}

void AEnemyCharacter::Movement(float DeltaTime)
{
	if (Distance() < FollowRange)
	{
		
		Look(DeltaTime);
		if (Distance() > 200.f)
		{
			FVector MoveDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(MoveDirection, FollowToPlayerSpeed * DeltaTime);
		}
		else if (RandomIsMoving)
		{
			RandomMovingAroundPlayer(DeltaTime);
		}
		/*else if(RandomIsMoving)
		{
			FVector RandomMoveDirection = (TargetLocationRandom - GetActorLocation()).GetSafeNormal();
			AddMovementInput(RandomMoveDirection, MoveSpeed * DeltaTime);
		}*/
	}
}

//FVector AEnemyCharacter::GetRandomLocation()
//{
//	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
//
//	float RandomX = FMath::FRandRange(-360.f, 360.f);
//
//	float RandomY = FMath::FRandRange(-360.f, 360.f);
//
//	FVector FinalDirection = FVector(RandomX, RandomY, 0.f);
//
//	FinalDirection.Z = GetActorLocation().Z;
//
//	return FinalDirection + PlayerLocation;
//	
//}

//void AEnemyCharacter::SetNewRandomTarget()
//{
//	if (IsDie) { return; }
//	TargetLocationRandom = GetRandomLocation();
//	RandomIsMoving = true;
//	GetWorldTimerManager().SetTimer(StopRandomMovementHandle, this, &AEnemyCharacter::StopRandomMovement, 2.f, false);
//}
//void AEnemyCharacter::StopRandomMovement()
//{
//	RandomIsMoving = false;
//}


void AEnemyCharacter::MeleeAttack()
{
	if (IsDie) { return; }
	if (Distance() < 200.f && !IsStunned)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			IsAttacking = true;
			AnimInstance->Montage_Play(MeleeAttackMontage, 1.4f);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyAttackSound, GetActorLocation());
			//EnemyAttackRaycast();
			GetWorld()->GetTimerManager().SetTimer(StopAttackHandle, FTimerDelegate::CreateLambda([this]
				{
					IsAttacking = false;
				}), 1.f, false);
		}
	}
}

void AEnemyCharacter::EnemyAttackRaycast()
{
	Super::EnemyAttackRaycast();
}

void AEnemyCharacter::Respawn()
{
	Super::Respawn();
}

void AEnemyCharacter::StopAttackMontage()
{
	if (EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Stop(0.5f, MeleeAttackMontage);
	}
}

void AEnemyCharacter::EnemyTakeDamage(float amount,float StunnedTimer)
{
	Super::EnemyTakeDamage(amount,StunnedTimer);
	if (EnemyAnimInstance)
	{
		if (CurrentHealth > 0)
		{
			EnemyAnimInstance->Montage_Play(HitReactionMontage, 0.7f);
		}
		else
		{
			StopAttackMontage();
		}
	}

}

//void AEnemyCharacter::Stunned()
//{
//	IsStunned = true;
//	FTimerHandle TimerHandle;
//	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
//		{
//			IsStunned = false;
//		}), 1.3f, false);
//}

//void AEnemyCharacter::BloodEffectSpawn()
//{
//	if (BloodEffect)
//	{
//		BloodEffect->Deactivate();
//		BloodEffect->DestroyComponent();
//		BloodEffect = nullptr;
//	}
//
//	FVector HitLocation = GetActorLocation();
//	FVector HitOffset = GetActorForwardVector() * -50.f;
//	FVector HitNormal(0, 0, 1);
//	FRotator BloodRotation = (-GetActorForwardVector()).Rotation();
//	if (BloodNiagaraEffect)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("BloodEffect is spawned"));
//		BloodEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
//			GetWorld(), BloodNiagaraEffect, HitLocation + HitOffset, BloodRotation, FVector(1.f)
//		);
//	}
//
//	if (BloodEffect)
//	{
//		BloodEffect->SetAutoDestroy(true);
//
//		FTimerHandle TimerHandle;
//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
//			{
//				if (BloodEffect)
//				{
//					BloodEffect->Deactivate();
//					BloodEffect->DestroyComponent();
//					BloodEffect = nullptr;
//				}
//			}), 5.2f, false);
//
//	}
//}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

