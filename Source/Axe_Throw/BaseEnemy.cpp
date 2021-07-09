// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBar_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar_UI->AttachTo(RootComponent);

	CrossHair_UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("CrossHair"));
	CrossHair_UI->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->OnTakeAnyDamage.AddDynamic(this, &ABaseEnemy::TakeDamage);
	
}


// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyDead();

	LockOnWidget();
}

void ABaseEnemy::EnemyDead()
{
	if (Health <= 0)
	{
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		//SetActorEnableCollision(true);
	}
}

void ABaseEnemy::LockOnWidget()
{
	if (Is_LockedOn == true)
	{
		CrossHair_UI->SetVisibility(true);
	}
	else
	{
		CrossHair_UI->SetVisibility(false);

	}
}

void ABaseEnemy::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	Health -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Got Hit")));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %i"), Health));

	AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_Play(HitAnim, 1.f, EMontagePlayReturnType::MontageLength, 0.f);
}


