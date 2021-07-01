// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OnHitTest.generated.h"

class UBoxComponent;

UCLASS()
class AXE_THROW_API AOnHitTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnHitTest();

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	UBoxComponent* Collider;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
