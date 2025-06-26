#include "RangedEnemy/BookHeadBossCharacter.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include <Components/SlateWrapperTypes.h>
#include "Components/ProgressBar.h"

ABookHeadBossCharacter::ABookHeadBossCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

}


void ABookHeadBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAnimInstance = GetMesh()->GetAnimInstance();

	GetWorldTimerManager().SetTimer(AttackHandle, this, &ABookHeadBossCharacter::BossAttack, 2.5f, true);

	GetWorldTimerManager().SetTimer(JumpAttackHandle, this, &ABookHeadBossCharacter::JumpAttackAnim, 12.f, true);

	CanMove = true;

	/*GetWorldTimerManager().SetTimer(RandomMovementTimer, this, &ABookHeadBossCharacter::SetNewRandomTarget, 7.f, true);*/
	
}

void ABookHeadBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!EnemyAnimInstance || IsBossDie) { return; }

	

	if (CanMove && Distance() < FollowRange)
	{
		Look(DeltaTime);

		if (!EnemyAnimInstance->Montage_IsPlaying(JumpAttackMontage))
		{
			if (Distance() < 700.f)
			{
				BaseMovement(DeltaTime, FollowSpeed);
			}
			else
			{
				BaseMovement(DeltaTime, RunSpeed);
			}
		}
		else
		{
			JumpAttack(DeltaTime);
		}
	}
}

void ABookHeadBossCharacter::EnemyTakeDamage(float amount, float StunnedTimer)
{
	if (IsBossDie) { return; }
	BossHealthBarEffect(amount);
}

void ABookHeadBossCharacter::Respawn()
{
	if (IsValid(this))
	{
		CurrentHealth = Health;
		if (BossHealthBarProgress)
		{
			BossHealthBarProgress->SetPercent(CurrentHealth / Health);
		}
		SetActorLocation(SpawnBaseEnemyLocation);
		SetActorRotation(SpawnBaseEnemyRotation);
	}
}

void ABookHeadBossCharacter::JumpAttack(float DeltaTime)
{
	FVector CurrenLocation = GetActorLocation();
	FVector TargetLocation = PlayerCharacter->GetActorLocation();
	FVector Direction = (TargetLocation - CurrenLocation).GetSafeNormal();
	FVector SmoothMove = FMath::VInterpTo(CurrenLocation, TargetLocation, DeltaTime, 1.2f);
	SetActorLocation(SmoothMove);
}

void ABookHeadBossCharacter::JumpAttackAnim()
{
	if (IsBossDie) { return; }
	if (Distance() < 1200.f && Distance() > 500.f)
	{
		EnemyAnimInstance->Montage_Play(JumpAttackMontage);
	}
}

void ABookHeadBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

