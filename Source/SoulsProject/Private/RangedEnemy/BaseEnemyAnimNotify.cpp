#include "RangedEnemy/BaseEnemyAnimNotify.h"
#include <BaseEnemy.h>
#include <MyCharacter.h>
#include "KrakenBossCharacter.h"
#include "Components/BoxComponent.h"

void UBaseEnemyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) { return; }
	EnemyCharacter = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (!EnemyCharacter) { return; }
	if (EnemyCharacter->IsDie) { return; }
	switch (EnemyType)
	{
	case EENemyType::Kraken:
		if (KrakenAttackState == EKrakenAttackState::OpenCollider)
		{
			KrakenBossAttack();
		}
		else if(KrakenAttackState == EKrakenAttackState::CloseCollider)
		{
			
			KrakenCloseCollider();
		}
		else if (KrakenAttackState == EKrakenAttackState::Movable)
		{
			BossMoving();
		}
		else if (KrakenAttackState == EKrakenAttackState::NotMovable)
		{
			StopMoving();
		}
		break;
	case EENemyType::Zombie:
		MeleeEnemyAttack();
		break;
	default:
		break;
	}
	
	
}

void UBaseEnemyAnimNotify::MeleeEnemyAttack()
{
	FVector Start = EnemyCharacter->GetMesh()->GetSocketLocation(FName("MeleeAttackSocket"));
	//FVector Start = GetActorLocation() + FVector(0, 0.5f, 0.f);
	FVector ForwardVector = EnemyCharacter->GetActorForwardVector();
	FVector End = Start + (ForwardVector * 130.f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(EnemyCharacter);
	FCollisionShape BoxShape = FCollisionShape::MakeBox(EnemyCharacter->BoxExtent);
	FQuat MyQuat = FQuat::Identity;
	FHitResult HitResult;

	bool IsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, MyQuat, ECC_Pawn, BoxShape, QueryParams);

	if (IsHit)
	{
		AMyCharacter* PlayerDamage = Cast<AMyCharacter>(HitResult.GetActor());
		if (PlayerDamage)
		{
			PlayerDamage->GetDamage(EnemyCharacter->EnemyAttackDamage);
		}
	}
}

void UBaseEnemyAnimNotify::KrakenBossAttack()
{
	EnemyCharacter->IsAttacking = true;
	EnemyCharacter->HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyCharacter->HitBoxCollider->SetGenerateOverlapEvents(true);
	EnemyCharacter->HitBoxCollider->SetBoxExtent(FVector(32.f, 32.f, 32.f), true);
}

void UBaseEnemyAnimNotify::KrakenCloseCollider()
{
	EnemyCharacter->IsAttacking = false;
	EnemyCharacter->HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyCharacter->HitBoxCollider->SetGenerateOverlapEvents(false);
	EnemyCharacter->HitBoxCollider->SetBoxExtent(FVector(0, 0, 0), true);
	EnemyCharacter->HitActors.Empty();
	
}

void UBaseEnemyAnimNotify::BossMoving()
{
	EnemyCharacter->CanMove = true;
}

void UBaseEnemyAnimNotify::StopMoving()
{
	EnemyCharacter->CanMove = false;
}
