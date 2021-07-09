// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AxeAttackAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class AXE_THROW_API UAxeAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	class AAxe_ThrowCharacter* Player;
	class USkeletalMeshComponent* Weapon;
	class AAxe* Axe_ref;

	TArray<AActor*>ActorsToIgnore;
	TArray<AActor*> HitActors;

	AController* EventInstigator;//ApplyDamage function parameters //

	TSubclassOf<UDamageType>DamageTypeClass;//ApplyDamage function parameters //

	FVector StartLocation;
	FVector EndLocation;
    
	UPROPERTY(EditAnyWhere, Category = Anim)
	float DamageValue = 10;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;
	
};
