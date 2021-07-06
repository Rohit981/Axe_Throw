// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Enemies.generated.h"

class UCapsuleComponent;

UCLASS()
class AXE_THROW_API AEnemies : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemies();

	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* EnemyMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	class UCapsuleComponent* Collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	int Health = 100;

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void EnemyDamage();

	/*UFUNCTION()
	void TakeDamage(AActor*OnTakeAnyDamage,AActor* DamagedActor, float Damage, const class UDamageType*DamageType, class AController*InstigatedBy, AActor*DamageCauser);*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Is_Damaged = false;

private:

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = Enemy)
    UAnimMontage* HitAnim;

};
