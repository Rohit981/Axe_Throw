// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::InitalizeTimeLine(UCurveFloat* CurveFloat, UObject* Object, FName function)
{
	if (CurveFloat)
	{

		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(Object, function);
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

	}
}

