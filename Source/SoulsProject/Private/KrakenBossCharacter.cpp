#include "KrakenBossCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include <Components/BoxComponent.h>
#include <MyCharacter.h>


AKrakenBossCharacter::AKrakenBossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/*HitBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxCollider"));
	HitBoxCollider->SetupAttachment(RootComponent);*/

}


void AKrakenBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetWorldTimerManager().SetTimer(AttackHandle, this, &AKrakenBossCharacter::BossAttack, 8.f, true);

	EnemyAnimInstance = GetMesh()->GetAnimInstance();

	//if (KrakenHealthBarWidgetClass)
	//{
	//	KrakenHealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), KrakenHealthBarWidgetClass);
	//	if (KrakenHealthBarWidget)
	//	{
	//		KrakenHealthBarWidget->AddToViewport();
	//		KrakenHealthProgressBar = Cast<UProgressBar>(KrakenHealthBarWidget->GetWidgetFromName(ProgressHealthBarName));
	//		KrakenBossHealthBarActive(ESlateVisibility::Hidden);
	//		//DefaultKrakenHealthBar = KrakenHealthProgressBar->GetFillColorAndOpacity();
	//	}
	//}

	GetWorldTimerManager().SetTimer(RandomMovementTimer, this, &AKrakenBossCharacter::SetNewRandomTarget, 3.5f, true);


}


void AKrakenBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsKrakenDie) { return; }
	Movement(DeltaTime);
	/*if (IsKrakenTakeDamage)
	{
		KrakenTakeHitInterval += DeltaTime;
		if (KrakenTakeHitInterval > KrakenAttackDelay)
		{
			HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			HitBoxCollider->SetGenerateOverlapEvents(true);
		}
	}*/
	
}


//void AKrakenBossCharacter::KrakenBossHealthBarActive(ESlateVisibility Visible)
//{
//	if (KrakenHealthBarWidget && KrakenHealthProgressBar)
//	{
//		KrakenHealthProgressBar->SetPercent(CurrentHealth / Health);
//		KrakenHealthBarWidget->SetVisibility(Visible);
//	}
//}

void AKrakenBossCharacter::Movement(float DeltaTime)
{
	if (Distance() < FollowRange && !IsAttacking)
	{
		Look(DeltaTime);
		if (Distance() > 1800.f)
		{
			FVector MoveDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			AddMovementInput(MoveDirection, FollowSpeed * DeltaTime);
		}
		else if (RandomIsMoving)
		{
			RandomMovingAroundPlayer(DeltaTime);
		}
	}
}

void AKrakenBossCharacter::EnemyTakeDamage(float amount,float StunnedTimer)
{
	/*if (IsKrakenDie) { return; }*/
	Super::BossHealthBarEffect(amount);
	/*CurrentHealth -= amount;
	if (KrakenHealthProgressBar)
	{
		KrakenHealthProgressBar->SetPercent(CurrentHealth / Health);
		KrakenHealthProgressBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Yellow);
		GetWorld()->GetTimerManager().SetTimer(ReturnDefaultHealthBarHandle, this, &AKrakenBossCharacter::ReturnDefaultHealthBar, 1.2f, false);
	}
	if (EnemyAnimInstance && !EnemyAnimInstance->IsAnyMontagePlaying() && CurrentHealth > 0)
	{
		EnemyAnimInstance->Montage_Play(KrakenGetHitMontage);
	}
	if (CurrentHealth <= 0)
	{
		EnemyAnimInstance->Montage_Stop(0.2f, AttackMontages[AttackIndex]);
		EnemyAnimInstance->Montage_Play(KrakenDeathMontage,1.f,EMontagePlayReturnType::MontageLength,0.f,false);
		IsKrakenDie = true;
		GetWorldTimerManager().SetTimer(KrakenBossDeathHandle, this, &AKrakenBossCharacter::KrakenDeath, 9.f, false);
	}*/
}


//void AKrakenBossCharacter::KrakenDeath()
//{
//	Destroy();
//}
//
//void AKrakenBossCharacter::ReturnDefaultHealthBar()
//{
//	KrakenHealthProgressBar->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Gray);
//}

void AKrakenBossCharacter::Respawn()
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

void AKrakenBossCharacter::Attack()
{
	if (IsKrakenDie) { return; }
	BossAttack();
	//if (Distance() < 1800.f && EnemyAnimInstance)
	//{
	//	int32 RandomizeAttackIndex = FMath::FRandRange(0.f, AttackMontages.Num());
	//	if (AttackIndex < AttackMontages.Num())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RandomAttacIndexValue : %d"), RandomizeAttackIndex));
	//		EnemyAnimInstance->Montage_Play(AttackMontages[RandomizeAttackIndex]);
	//		//KrakenAttackRaycast();
	//		/*FOnMontageEnded MontageEnded;
	//		MontageEnded.BindUObject(this, &AKrakenBossCharacter::KrakenAttackOnMontageEnded);
	//		EnemyAnimInstance->Montage_SetEndDelegate(MontageEnded, AttackMontages[AttackIndex]);*/
	//		AttackIndex++;
	//	}
	//	else
	//	{
	//		AttackIndex = 0;
	//	}
	//}
}

//void AKrakenBossCharacter::OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor)
//	{
//		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
//		if (Player && OtherActor != GetOwner())
//		{
//			if (!HitActors.Contains(OtherActor))
//			{
//				Player->GetDamage(EnemyAttackDamage);
//				HitActors.Add(OtherActor);
//			}
//		}
//	}
//}


void AKrakenBossCharacter::KrakenAttackRaycast()
{
	IsKrakenTakeDamage = true;
	GetWorldTimerManager().SetTimer(KrakenAttackDelayHandle, this, &AKrakenBossCharacter::AttackTimerReset, 0.91f, false);
	/*if (KrakenTakeHitInterval > KrakenAttackDelay)
	{
		HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitBoxCollider->SetGenerateOverlapEvents(true);
	}*/
}

void AKrakenBossCharacter::KrakenAttackOnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	KrakenTakeHitInterval = 0.f;
	HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBoxCollider->SetGenerateOverlapEvents(false);
	if (!bInterrupted)
	{
		HitActors.Empty();
	}
}

void AKrakenBossCharacter::AttackTimerReset()
{
	IsKrakenTakeDamage = false;
	
}


void AKrakenBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

