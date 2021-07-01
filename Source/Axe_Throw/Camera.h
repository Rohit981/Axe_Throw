// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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

	float EndFOV;
	float StartFOV;

	FTimeline CurveTimeline;
	

private:
	
};
