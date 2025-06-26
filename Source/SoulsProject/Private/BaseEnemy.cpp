#include "BaseEnemy.h"
#include "Components/WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacter.h"
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <RangedEnemy/EnemyManager.h>
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"
#include "Components/ProgressBar.h"
#include "Components/BoxComponent.h"
#include "RangedEnemy/BookHeadBossCharacter.h"
#include <Menu/MySaveManager.h>
#include <Menu/MySaveGame.h>

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyHealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBarWidgetComponent"));
	EnemyHealthBarWidgetComponent->SetupAttachment(RootComponent);
	EnemyHealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	HitBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxCollider"));
	HitBoxCollider->SetupAttachment(RootComponent);

}



void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	CurrentHealth = Health;

	SpawnBaseEnemyLocation = GetActorLocation();
	SpawnBaseEnemyRotation = GetActorRotation();

	if (EnemyHealthBarWidgetComponent)
	{
		EnemyHealthBarWidget = EnemyHealthBarWidgetComponent->GetWidget();
		if (EnemyHealthBarWidget)
		{
			HealthBar = Cast<UProgressBar>(EnemyHealthBarWidget->GetWidgetFromName(ProgressHealthBarName));
			EnemyHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (BossHealthBarWidgetClass)
	{
		BossHealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), BossHealthBarWidgetClass);
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->AddToViewport();
			BossHealthBarProgress = Cast<UProgressBar>(BossHealthBarWidget->GetWidgetFromName(ProgressHealthBarName));
			BossHealthBarActive(ESlateVisibility::Hidden);
		}
	}

	HitBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnHitBeginOverlap);

	HitBoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBoxCollider->SetGenerateOverlapEvents(false);

	HitBoxCollider->SetBoxExtent(FVector(0, 0, 0), true);

	
	
	AEnemyManager* EnemyManager = Cast<AEnemyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass()));
	if (EnemyManager)
	{
		EnemyManager->RegisterEnemy(this);
	}

	UMySaveGame* LoadGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("BossDeathSaveSlot"), 0));
	if (LoadGame)
	{
		if (LoadGame->SavedDeadBosses.Contains(BossName) && LoadGame->SavedDeadBosses[BossName])
		{
			IsBossDie = LoadGame->SavedBossDeath;
			Destroy();
		}
	}

}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyHealthBarWidget)
	{
		Distance() < EnableHudRange ? EnemyHealthBarWidget->SetVisibility(ESlateVisibility::Visible) : EnemyHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		if (IsDie)
		{
			EnemyHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABaseEnemy::BaseMovement(float DeltaTime,float Speed)
{
	if (Distance() < FollowRange && !IsStunned)
	{
		Look(DeltaTime);
		if (Distance() > AttackIntervalRange)
		{
			FVector MovementDirection = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
			AddMovementInput(MovementDirection, Speed * DeltaTime);
		}
	}
}


void ABaseEnemy::Look(float DeltaTime)
{
	FVector EnemyLocation = GetActorLocation();
	LookDirection = (PlayerCharacter->GetActorLocation() - EnemyLocation).Rotation();
	LookDirection.Pitch = 0.f;
	LookDirection += LookDirectionValue;
	FRotator SmoothLook = FMath::RInterpTo(GetActorRotation(), LookDirection, DeltaTime, LookSpeed);
	SetActorRotation(SmoothLook);
}

void ABaseEnemy::RandomMovingAroundPlayer(float DeltaTime)
{
	FVector RandomMoveDirection = (TargetLocationRandom - GetActorLocation()).GetSafeNormal();
	AddMovementInput(RandomMoveDirection, MoveSpeed * DeltaTime);
}

FVector ABaseEnemy::GetRandomLocation()
{
	//FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float RandomX = FMath::FRandRange(-360.f, 360.f);

	float RandomY = FMath::FRandRange(-360.f, 360.f);

	FVector FinalDirection = FVector(RandomX, RandomY, 0.f);

	FinalDirection.Z = GetActorLocation().Z;

	return FinalDirection + GetActorLocation();

}

void ABaseEnemy::SetNewRandomTarget()
{
	if (IsDie) { return; }
	TargetLocationRandom = GetRandomLocation();
	RandomIsMoving = true;
	GetWorldTimerManager().SetTimer(StopRandomMovementHandle, this, &ABaseEnemy::StopRandomMovement, RandomMoveTimer, false);
}


void ABaseEnemy::StopRandomMovement()
{
	RandomIsMoving = false;
}

void ABaseEnemy::BossDeath()
{
	Destroy();
}

void ABaseEnemy::ReturnDefaultHeatlhBar()
{
	BossHealthBarProgress->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Gray);
}

void ABaseEnemy::BossHealthBarEffect(float amount)
{
	if (IsBossDie) { return; }
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHitSound, GetActorLocation());
	CurrentHealth -= amount;
	if (BossHealthBarProgress)
	{
		BossHealthBarProgress->SetPercent(CurrentHealth / Health);
		BossHealthBarProgress->WidgetStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Yellow);
		GetWorld()->GetTimerManager().SetTimer(ReturnDefaultHealthBarHandle, this, &ABaseEnemy::ReturnDefaultHeatlhBar, 1.2f, false);
	}
	if (EnemyAnimInstance && !EnemyAnimInstance->IsAnyMontagePlaying() && CurrentHealth > 0)
	{
		EnemyAnimInstance->Montage_Play(HitReactionMontage);
	}
	if (CurrentHealth <= 0 && EnemyAnimInstance)
	{
		EnemyAnimInstance->Montage_Stop(0.2f, AttackMontages[AttackIndex]);
		EnemyAnimInstance->Montage_Play(EnemyDeathMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, false);
		IsBossDie = true;
		UMySaveManager* BossSaveManager = Cast<UMySaveManager>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (BossSaveManager)
		{
			BossSaveManager->BossDeathSave(this);
		}
		GetWorldTimerManager().SetTimer(BossDeathHandle, this, &ABaseEnemy::BossDeath, 9.f, false);
	}
}

void ABaseEnemy::EnemyTakeDamage(float amount,float StunnedTimer)
{
	if (IsDie) { return; }
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyHitSound, GetActorLocation());
	CurrentHealth -= amount;
	HitReactionImpulse();
	Stunned(StunnedTimer);
	//BloodEffectSpawn();
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / Health);
	}
	if (CurrentHealth <= 0)
	{
		if (AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
		{
			IIInteractable* SoulTaker = Cast<IIInteractable>(PlayerActor);
			if (SoulTaker)
			{
				SoulTaker->GetSoulAmountFlex(SoulAmount,0);
			}
		}
		IsDie = true;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ABaseEnemy::Die, 4.5f, false);
	}
}

void ABaseEnemy::Respawn()
{
	IsDie = false;
	CurrentHealth = Health;
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / Health);
	}
	SetActorLocation(SpawnBaseEnemyLocation);
	SetActorRotation(SpawnBaseEnemyRotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ABaseEnemy::Die()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ABaseEnemy::Stunned(float StunnedTimer)
{
	IsStunned = true;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
		{
			IsStunned = false;
		}), StunnedTimer, false);
	//1.3f stun timer
}

void ABaseEnemy::BloodEffectSpawn()
{
	if (BloodEffect)
	{
		BloodEffect->Deactivate();
		BloodEffect->DestroyComponent();
		BloodEffect = nullptr;
	}

	FVector HitLocation = GetActorLocation();

	FVector HitOffset = GetActorForwardVector() * -50.f;
	FVector HitNormal(0, 0, 1);
	FRotator BloodRotation = (-GetActorForwardVector()).Rotation();
	if (BloodNiagaraEffect)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("BloodEffect is spawned"));
		BloodEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), BloodNiagaraEffect, HitLocation + HitOffset, BloodRotation, FVector(1.f)
		);
	}

	if (BloodEffect)
	{
		BloodEffect->SetAutoDestroy(true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]
			{
				if (BloodEffect)
				{
					BloodEffect->Deactivate();
					BloodEffect->DestroyComponent();
					BloodEffect = nullptr;
				}
			}), 5.2f, false);

	}
}

void ABaseEnemy::BossHealthBarActive(ESlateVisibility Visible)
{
	if (BossHealthBarWidget && BossHealthBarProgress)
	{
		BossHealthBarProgress->SetPercent(CurrentHealth / Health);
		BossHealthBarWidget->SetVisibility(Visible);
	}
}

void ABaseEnemy::OnHitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
		if (Player && OtherActor != GetOwner())
		{
			if (!HitActors.Contains(OtherActor))
			{
				Player->GetDamage(EnemyAttackDamage);
				HitActors.Add(OtherActor);
			}
		}
	}
}

void ABaseEnemy::HitReactionImpulse()
{
	FVector EnemyLocation = GetActorForwardVector();
	FVector GetHit(EnemyLocation * ForceReaction * -1);
	LaunchCharacter(GetHit, true, false);
}

void ABaseEnemy::EnemyAttackRaycast()
{
	if (IsDie) { return; }
	//FVector Start = GetMesh()->GetSocketLocation(FName("MeleeAttackSocket"));
	////FVector Start = GetActorLocation() + FVector(0, 0.5f, 0.f);
	//FVector ForwardVector = GetActorForwardVector();
	//FVector End = Start + (ForwardVector * 100.f);
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(this);
	//FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxExtent);
	//FQuat MyQuat = FQuat::Identity;

	//IsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, MyQuat, ECC_Pawn, BoxShape, QueryParams);

	//if (IsHit)
	//{
	//	AMyCharacter* PlayerDamage = Cast<AMyCharacter>(HitResult.GetActor());
	//	if (PlayerDamage)
	//	{
	//		PlayerDamage->GetDamage(EnemyAttackDamage);
	//	}
	//}

	/*DrawDebugSphere(GetWorld(), Start, 150.f, 12, FColor::Red, false, 1.0f);
	DrawDebugSphere(GetWorld(), End, 150.f, 12, FColor::Red, false, 1.0f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.f);*/
}

void ABaseEnemy::BossAttack()
{
	if (IsBossDie) { return; }
	if (Distance() < AttackRange && EnemyAnimInstance)
	{
		int32 RandomizeAttackIndex = FMath::FRandRange(0.f, AttackMontages.Num());
		if (AttackIndex < AttackMontages.Num())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EnemyAttackSound, GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("RandomAttacIndexValue : %d"), RandomizeAttackIndex));
			EnemyAnimInstance->Montage_Play(AttackMontages[RandomizeAttackIndex],AttackAnimationSpeed);
			//KrakenAttackRaycast();
			/*FOnMontageEnded MontageEnded;
			MontageEnded.BindUObject(this, &AKrakenBossCharacter::KrakenAttackOnMontageEnded);
			EnemyAnimInstance->Montage_SetEndDelegate(MontageEnded, AttackMontages[AttackIndex]);*/
			AttackIndex++;
		}
		else
		{
			AttackIndex = 0;
		}
	}
}


float ABaseEnemy::Distance()
{
	if (PlayerCharacter)
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector EnemyLocation = GetActorLocation();
		float distance = FVector::Dist(PlayerLocation, EnemyLocation);
		return distance;
	}
	return 0;
}


void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

