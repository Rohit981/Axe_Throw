// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "BaseEnemy.h"
#include "Camera/CameraComponent.h"

/**
 * 
 */
class UCurveFloat;

class AXE_THROW_API Camera
{
public:
	Camera();
	~Camera();

	UFUNCTION()
	void InitalizeTimeLine(UCurveFloat* CurveFloat, UObject* Object, FName function);

	void LockON(UCameraComponent* FollowCamera, AActor* Player);

	float EndFOV = 90;
	float StartFOV = 75;

	FTimeline CurveTimeline;

	AActor* LockedTarget;

	bool IsLocked = false;

	ABaseEnemy* enemies;
	

private:
	
};
