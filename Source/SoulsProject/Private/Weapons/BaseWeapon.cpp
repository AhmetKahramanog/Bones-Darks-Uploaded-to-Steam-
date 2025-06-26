#include "Weapons/BaseWeapon.h"
#include <BaseEnemy.h>
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include <Menu/MySaveGame.h>
#include <Menu/MySaveManager.h>


ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SetRootComponent(WeaponMesh);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(WeaponMesh);

	BloodEffectScene = CreateDefaultSubobject<USceneComponent>(TEXT("BloodEffect"));
	BloodEffectScene->SetupAttachment(WeaponMesh);

}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBeginOverlap);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnEndOverlap);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnHitOverlap);

	BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollider->SetGenerateOverlapEvents(false);

	

	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"),0))
	{
		UMySaveGame* SaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
		if (!SaveGame) { return; }

		if (SaveGame->SavedWeaponsId.Contains(WeaponID))
		{
			Destroy();
		}
	}

}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABaseWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character)
	{
		Character->GetSword(this);
	}
}

void ABaseWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character)
	{
		Character->GetSword(nullptr);
	}
}

void ABaseWeapon::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(OtherActor);
		if (EnemyCharacter && OtherActor != GetOwner())
		{
			if (!HitActors.Contains(OtherActor))
			{
				EnemyCharacter->EnemyTakeDamage(AttackDamage,TakeStunnedTimer);
				SpawnBloodEffect();
				HitActors.Add(OtherActor);
			}
		}
	}
}


void ABaseWeapon::WeaponHitCollider(ECollisionEnabled::Type Collision, bool canHit)
{
	if (BoxCollider != nullptr)
	{
		BoxCollider->SetCollisionEnabled(Collision);
		BoxCollider->SetGenerateOverlapEvents(canHit);
		if (!canHit)
		{
			HitActors.Empty();
		}
	}
}

void ABaseWeapon::SpawnBloodEffect()
{
	FVector SpawnLocation = BloodEffectScene->GetComponentLocation();
	if (BloodEffect)
	{
		DestroyBloodEffect();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitFleshSound, GetActorLocation());
		SpawnedBloodEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, SpawnLocation, FRotator::ZeroRotator);
		GetWorld()->GetTimerManager().SetTimer(DestroySpawnedBloodEffectHandle, this, &ABaseWeapon::DestroyBloodEffect, 1.5f, false);
	}
}

void ABaseWeapon::DestroyBloodEffect()
{
	if (SpawnedBloodEffect)
	{
		SpawnedBloodEffect->SetAutoDestroy(true);
		SpawnedBloodEffect->Deactivate();
		SpawnedBloodEffect->DestroyComponent();
		SpawnedBloodEffect = nullptr;
	}
}

