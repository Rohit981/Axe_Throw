// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
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

void Camera::LockON(UCameraComponent* FollowCamera, AActor* Player)
{
	FVector Start = Player->GetActorLocation();
	FVector End = (FollowCamera->GetForwardVector() * 1000) + Start;

	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(FollowCamera->GetOwner());
	ActorsToIgnore.Add(Player);

	bool IsHit = false;

	IsHit =  UKismetSystemLibrary::SphereTraceSingle(Player->GetWorld(), Start, End, 10, ETraceTypeQuery::TraceTypeQuery4, 
		                                   false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Green, 1.f);



	 if (IsHit == true)
	 {
		 if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ray Hit: %s"), *OutHit.GetActor()->GetName()));

		 IsLocked = true;

		 LockedTarget = OutHit.GetActor();

		 enemies = Cast<AEnemies>(LockedTarget);

		 enemies->Is_LockedOn = true;


		
	 }

}

