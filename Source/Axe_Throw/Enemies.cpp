// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemies::AEnemies()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SceneComponent->SetupAttachment(RootComponent);

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsual"));
	Collider->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Enemy"));
	EnemyMesh->AttachToComponent(Collider, FAttachmentTransformRules::KeepRelativeTransform);

	LockOnUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockON"));
	LockOnUI->AttachToComponent(EnemyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	
}

// Called when the game starts or when spawned
void AEnemies::BeginPlay()
{
	Super::BeginPlay();

	
	this->OnTakeAnyDamage.AddDynamic(this, &AEnemies::TakeDamage);

    
	
}

// Called every frame
void AEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (IsHit == true)
	//{
	//	FVector forwardVector = this->GetActorForwardVector();

	//	//Collider->AddRelativeLocation(forwardVector * 0.5f);

	//	AnimTime += DeltaTime;
	//}

	//

	//if (AnimTime >= 1.3)
	//{
	//	IsHit = false;

	//	AnimTime = 0;
	//}


	LockOnWidget();

}

//void AEnemies::EnemyDamage()
//{
//	
//	/*Health -= 10;
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Got Hit")));
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %i"), Health));
//
//	AnimInstance = EnemyMesh->GetAnimInstance();
//
//	AnimInstance->Montage_Play(HitAnim, 1.f, EMontagePlayReturnType::MontageLength, 0.f);
//
//	Is_Damaged = false;
//
//	if (Health <= 0)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Dead")));
//	}*/
//
//	
//}

void AEnemies::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//Damage = EnemyTypeDamageValue;

	if (Damage <= 0)
	{
		IsHit = false;
	}

	Health -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Got Hit")));
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %i"), Health));

	AnimInstance = EnemyMesh->GetAnimInstance();
	
	AnimInstance->Montage_Play(HitAnim, 1.f, EMontagePlayReturnType::MontageLength, 0.f);

	

	IsHit = true;
	

}

void AEnemies::LockOnWidget()
{
	if (Is_LockedOn == true)
	{
		LockOnUI->SetVisibility(true);

		LockOnUI->AddRelativeRotation(FRotator(0,0,1));

	}
	else
	{
		LockOnUI->SetVisibility(false);

	}
}





