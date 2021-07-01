// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemies::AEnemies()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Enemy"));
	EnemyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsual"));
	Collider->AttachToComponent(EnemyMesh, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AEnemies::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyDamage();

}

void AEnemies::EnemyDamage()
{
	if (Is_Damaged == true)
	{
		Health -= 10;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Got Hit")));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %i"), Health));

		AnimInstance = EnemyMesh->GetAnimInstance();

		AnimInstance->Montage_Play(HitAnim, 1.f, EMontagePlayReturnType::MontageLength, 0.f);

		Is_Damaged = false;

		if (Health <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Dead")));
		}

	}
}

