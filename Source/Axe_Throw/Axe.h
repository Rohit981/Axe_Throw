// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Axe_ThrowCharacter.h"
#include "Enemies.h"
#include "Axe.generated.h"


class UBoxComponent;

UCLASS()
class AXE_THROW_API AAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAxe();

	UPROPERTY(EditAnywhere, Category = Mesh)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* AxeMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	class UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, Category = Axe)
	AAxe_ThrowCharacter* ref_axe;
	

private:
	float CurrentDistance;
	float HalfDistance;

	bool Hit;
	TArray<FHitResult> OutHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Collision Check

	void SphereTraceCollider(float Radius);

	void AxeAttackHit();

	void AxeThrowHit();

	//Axe Throw and Return
	void ThrowAxe();

	void AxeTransform();

	void ReturnAxe(float DeltaTime);

	//Variables for Axe throw and Return
	UPROPERTY(EditAnywhere, Category = Axe)
	bool IS_Axe_Colliding = false;

	UPROPERTY(EditAnywhere, Category = Axe)
	float SphereRadius;

	UPROPERTY(EditAnywhere, Category = Axe)
	float AxeThrowSphereRadius;

	UPROPERTY(EditAnywhere, Category = AxeThrow)
	float AxeRotationSpeed;

	UPROPERTY(EditAnywhere, Category = AxeThrow)
	float AxeMoveSpeed;

	UPROPERTY(EditAnywhere, Category = AxeThrow)
	float AxeReturnSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UMatineeCameraShake> ref_CamShake;

	UPrimitiveComponent* AxePrimitive;

	bool IS_Acting_Force = false;

public:


};
