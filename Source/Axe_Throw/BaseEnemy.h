// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class AXE_THROW_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UPROPERTY(EditAnywhere, Category = Enemy)
		int EnemyTypeDamageValue = 0;

	//UFUNCTION(BlueprintCallable, Category = Enemy)
	//void EnemyDamage();

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* HealthBar_UI;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* CrossHair_UI;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Is_Damaged = false;

	bool Is_LockedOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		int Health = 100;

private:

	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = Enemy)
		UAnimMontage* HitAnim;

	void EnemyDead();

	void LockOnWidget();

};
