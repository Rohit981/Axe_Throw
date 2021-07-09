// Copyright Epic Games, Inc. All Rights Reserved.

#include "Axe_ThrowCharacter.h"
#include "Axe.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BaseEnemy.h"

//////////////////////////////////////////////////////////////////////////
// AAxe_ThrowCharacter

AAxe_ThrowCharacter::AAxe_ThrowCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	splineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	splineComp->SetupAttachment(GetMesh(), TEXT("Axe_Socket"));
	
	splineComp->bDrawDebug = true;
	//splineComp->SetHiddenInGame(false);


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontageObject(TEXT("AnimMontage'/Game/Character/Animations/Montage/Axe_Throw_Montage.Axe_Throw_Montage'"));

	if (ThrowMontageObject.Succeeded())
	{
		ThrowMontage = ThrowMontageObject.Object;
	}
	
	
}

void AAxe_ThrowCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnAxe();

	cam.InitalizeTimeLine(CurveFloat, this, "Zoom");

	HUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	//HUD->CrossHair_Ref->SetVisibility(ESlateVisibility::Hidden);
	

}

void AAxe_ThrowCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (AxeCurveTimeline.IsPlaying() || AxeCurveTimeline.IsReversing())
	{
		AxeCurveTimeline.TickTimeline(DeltaTime);
	}

	if (AxeCurveTimeline.IsReversing())
	{
		AxeCurveTimeline.SetPlayRate(3);
	}
	else
	{
		AxeCurveTimeline.SetPlayRate(1);

	}*/

	cam.CurveTimeline.TickTimeline(DeltaTime);

	AnimInstance = GetMesh()->GetAnimInstance();

	AdsCharacterOrientation();

	LightAttackCounter(DeltaTime);
	
	LockOnTargetRotation(DeltaTime);

	if (enableDetectTrace == true)
	{
		EnemyDetected();
	}

	//ThrowTrajectory();
	
}




void AAxe_ThrowCharacter::ThrowTrajectory()
{

	FVector StartPos = GetMesh()->GetSocketLocation(TEXT("Axe_Socket"));

	FVector EndPos = (FollowCamera->GetForwardVector() * LaunchVelocity);

	FVector LastTraceDes = StartPos + EndPos;
	splineComp->SetHiddenInGame(false);

	TArray<AActor*> ActorsToIgnore;

	
	FOccluderVertexArray hitArray;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Axe);

	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	/*FPredictProjectilePathParams Params;
	FPredictProjectilePathResult PathResults;

	Params.StartLocation = 
	Params.LaunchVelocity = 
	Params.ProjectileRadius = 5;*/


	if (startTrajectory == true /*&& hasAxe == true*/)
	{
		
		projectileHit = UGameplayStatics::PredictProjectilePath(GetWorld(), ProjectileOutHit, hitArray, LastTraceDes, StartPos, EndPos, true, 5, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, 1, 15, 2,0);

		splineImpactPoint = ProjectileOutHit.ImpactPoint.X;
		//UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PathResults);

		/*for (int i = 0; i< hitArray.Num(); i++)
		{

			storehitArray.Add(hitArray[i]);


		}*/
		splineComp->SetSplinePoints(hitArray, ESplineCoordinateSpace::World, true);

		//float currentSplineTime = (GetWorld()->GetTimeSeconds()) / 1000;
		
		
		
		
		//float splinePoints = splineComp->GetSplineLength();


		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Spline Points: %f"), splinePoints));

		
	}

	





	//splineVector = splineComp->GetLocationAtDistanceAlongSpline(splineDistance, ESplineCoordinateSpace::World);
}

void AAxe_ThrowCharacter::SpawnAxe()
{
	if (AxeSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FVector Location = GetActorLocation();
			FRotator Rotation = GetActorRotation();

			Axe = World->SpawnActor<AAxe>(AxeSubclass, Location, Rotation);

			

			if (Axe != nullptr)
			{
				Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Unequip_Axe"));
			}
		}
	}
}

void AAxe_ThrowCharacter::AxeProcessMovementTimeline(float Value)
{
	//float splineStartPoint = GetMesh()->GetSocketLocation(TEXT("Axe_Socket")).Y;
	float splineEndPoint = splineComp->GetSplineLength();

	float splinePoints = FMath::Lerp(0.f, splineEndPoint, Value);

	splineVector = splineComp->GetLocationAtDistanceAlongSpline(splinePoints, ESplineCoordinateSpace::World);

	Axe->AxeMesh->SetWorldLocation(splineVector);

	//Axe->AxeMesh->SetWorldLocationAndRotation(splineVector, splineRotation);
}

void AAxe_ThrowCharacter::AxeMoveDirection()
{
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("AxeProcessMovementTimeline"));
	AxeCurveTimeline.AddInterpFloat(AxeCurveFloat, TimelineProgress);

	AxeCurveTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	AxeCurveTimeline.Play();
}


//////////////////////////////////////////////////////////////////////////
// Input
void AAxe_ThrowCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAxe_ThrowCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAxe_ThrowCharacter::StopSprinting);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AAxe_ThrowCharacter::ADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AAxe_ThrowCharacter::StopADS);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AAxe_ThrowCharacter::EquipAxe);

	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &AAxe_ThrowCharacter::LightAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &AAxe_ThrowCharacter::HeavyAttack);


	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AAxe_ThrowCharacter::Throw);
	PlayerInputComponent->BindAction("Throw", IE_Released, this, &AAxe_ThrowCharacter::StopThrow);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAxe_ThrowCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAxe_ThrowCharacter::MoveRight);
	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &AAxe_ThrowCharacter::Evade);

	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &AAxe_ThrowCharacter::CamLockon);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAxe_ThrowCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAxe_ThrowCharacter::LookUpAtRate);

	
}



#pragma region Camera Lock ON

void AAxe_ThrowCharacter::LockOnTargetRotation(float DeltaTime)
{
	if (cam.IsLocked == true)
	{

		FRotator Current = GetController()->GetPawn()->GetActorRotation();

		FRotator Target = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), cam.LockedTarget->GetActorLocation());

		FRotator TargetRotation = FMath::RInterpTo(Current, Target, DeltaTime, 4.f);

		GetController()->SetControlRotation(TargetRotation);



	}
}


void AAxe_ThrowCharacter::CamLockon()
{
	if (cam.IsLocked == false)
	{
		cam.LockON(FollowCamera, this);
		
		if (cam.IsLocked == true)
		{
			bUseControllerRotationYaw = true;

		}

	}
	else
	{
		cam.IsLocked = false;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Cam is Locked: %s"), cam.IsLocked ? "True" : "False"));

		cam.enemies = Cast<ABaseEnemy>(cam.LockedTarget);

		cam.enemies->Is_LockedOn = false;

		cam.LockedTarget = nullptr;
		bUseControllerRotationYaw = false;

	}

	


}
#pragma endregion

#pragma region Melee Attack
void AAxe_ThrowCharacter::EquipAxe()
{

	if (Is_Draw == false)
	{

		AnimInstance->Montage_Play(EquipMontage, 1.5f, EMontagePlayReturnType::MontageLength, 0);

		Is_Draw = true;
	}
	else
	{

		AnimInstance->Montage_Play(UnEquipMontage, 1.5f, EMontagePlayReturnType::MontageLength, 0);

		Is_Draw = false;
	}

}



void AAxe_ThrowCharacter::Armed()
{
	if (Is_Draw == true)
	{
		Axe->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));

		Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Axe_Socket"));

	}
	else
	{
		Axe->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));

		Axe->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Unequip_Axe"));
	}


}



void AAxe_ThrowCharacter::LightAttack()
{
	if (IsAdsing == false && hasAxe == true)
	{
		if (Attack_Enable == true)
		{
			if (IsSprinting == true)
			{
				AnimInstance->Montage_Play(Light_Sprint_Attack_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);


			}
			else
			{

				if (Is_Draw == false && ComboCounter == 0)
				{
					AnimInstance->Montage_Play(EquipMontage, 1.5f, EMontagePlayReturnType::MontageLength, 0);

					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Attack Button Pressed")));

					AnimInstance->Montage_Play(Attack_1_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);

					Is_Draw = true;
					Is_Counting = true;

					Attack_Enable = false;

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Attack Button Pressed")));

					AnimInstance->Montage_Play(Attack_1_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);

					Is_Counting = true;

					Attack_Enable = false;

				}

				if (Is_Counting == true && ComboCounter == 1)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Light Attack 2 Pressed")));

					AnimInstance->Montage_Play(Attack_2_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);

					Attack_Enable = false;
				}

				if (Is_Counting == true && ComboCounter == 2)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Light Attack 3 Pressed")));

					AnimInstance->Montage_Play(Attack_3_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);

					Attack_Enable = false;

				}


			}
		}


	}
	else
	{
		if (hasAxe == true)
		{
			//AnimInstance->Montage_Play(ThrowMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0);

		    AnimMontage->PlayAxeThrow(AnimInstance, ThrowMontage);

		}
	}

}

void AAxe_ThrowCharacter::HeavyAttack()
{
	if (IsSprinting == true)
	{
		AnimInstance->Montage_Play(Heavy_Sprint_Attack_Montage, 1.2f, EMontagePlayReturnType::MontageLength, 0);


	}

	else
	{
		ComboCounter += 1;

		if (Is_Draw == false && ComboCounter == 1)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.5f, EMontagePlayReturnType::MontageLength, 0);

			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Heavy Attack Button Pressed")));

			AnimInstance->Montage_Play(Heavy_Attack_1_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0);

			Is_Draw = true;


		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Heavy Attack Button Pressed")));

			AnimInstance->Montage_Play(Heavy_Attack_1_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0);


		}

		if (ComboCounter == 2)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Heavy Attack 2 Button Pressed")));

			AnimInstance->Montage_Play(Heavy_Attack_2_Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0);

			ComboCounter = 0;


		}
	}
}

void AAxe_ThrowCharacter::SprintHeavyAttack()
{
	FVector Start = Axe->AxeMesh->GetSocketLocation(TEXT("Top"));
	FVector End = Axe->AxeMesh->GetSocketLocation(TEXT("Bottom"));

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(Axe);
	ActorsToIgnore.Add(this);

	TArray<FHitResult> OutHit;

	bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, HeavyAttackRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true,
		FLinearColor::Red, FLinearColor::Green, 1);

	for (int i = 0; i < OutHit.Num(); i++)
	{
		ABaseEnemy* enemies = Cast<ABaseEnemy>(OutHit[i].GetActor());

		if (Hit)
		{
			if (OutHit[i].GetActor() == enemies)
			{
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Hit: %s"), *OutHit[i].GetComponent()->GetName()));

				if(OutHit[i].GetComponent()->GetName() == "CollisionCylinder")
					UGameplayStatics::ApplyDamage(enemies, 40, EventInstigator, this, DamageTypeClass);
			}

			
		}
	}
}

void AAxe_ThrowCharacter::IncreaseCounter()
{
	Attack_Enable = true;
	ComboCounter += 1;
}

void AAxe_ThrowCharacter::LightAttackCounter(float Delta)
{
	if (Is_Counting == true)
	{
		ComboTime += Delta;
	}

	if (ComboTime >= 3)
	{
		ComboCounter = 0;
		ComboTime = 0;
		Is_Counting = false;
		Attack_Enable = true;
	}
}

#pragma endregion

#pragma region Movement
void AAxe_ThrowCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void AAxe_ThrowCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAxe_ThrowCharacter::MoveForward(float Value)
{

	if ((Controller != nullptr) && (Value != 0.0f) && Is_Moving == true)
	{
		// find out which way is forward
	
		if (GetCharacterMovement()->bOrientRotationToMovement == true)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		

		if (!IsSprinting)
		{
			Value *= 0.4;

		}

		AddMovementInput(GetActorForwardVector(), Value);

	}
}

void AAxe_ThrowCharacter::MoveRight(float Value)
{

	if ((Controller != nullptr) && (Value != 0.0f) && Is_Moving == true)
	{

		if (GetCharacterMovement()->bOrientRotationToMovement == true)
		{
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		if (!IsSprinting)
		{
			Value *= 0.4;
		}

		AddMovementInput(GetActorRightVector(), Value);

	}
}

void AAxe_ThrowCharacter::DisableMovement()
{
	Is_Moving = false;
}
void AAxe_ThrowCharacter::EnableMovement()
{
	Is_Moving = true;

}

void AAxe_ThrowCharacter::Evade()
{
	AddMovementInput(GetActorForwardVector(), 100);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Evade Pressed")));

}

void AAxe_ThrowCharacter::Roll()
{

}

#pragma endregion

#pragma region Sprint
void AAxe_ThrowCharacter::Sprinting()
{
	if (GetInputAxisValue("MoveForward") > 0)
	{
		cam.CurveTimeline.PlayFromStart();
		IsSprinting = true;

	}
	
}

void AAxe_ThrowCharacter::StopSprinting()
{

	cam.CurveTimeline.Reverse();
	IsSprinting = false;
	

}

#pragma endregion

#pragma region ADS

void AAxe_ThrowCharacter::ADS()
{
	if ( hasAxe == true && Is_Draw == true && IsSprinting == false)
	{
		IsAdsing = true;
		cam.CurveTimeline.PlayFromStart();

		
		HUD->CrossHair_Ref->SetVisibility(ESlateVisibility::Visible);

		
		enableDetectTrace = true;

		startTrajectory = true;
		
	}
	
}

void AAxe_ThrowCharacter::StopADS()
{
	
	IsAdsing = false;
	cam.CurveTimeline.Reverse();

	

	enableDetectTrace = false;

	//startTrajectory = false;

	
	HUD->CrossHair_Ref->SetVisibility(ESlateVisibility::Hidden);



}

void AAxe_ThrowCharacter::Zoom(float Value)
{

	if (IsSprinting)
	{
		cam.StartFOV = 75;

		cam.EndFOV = 90;
	}

	if (IsAdsing)
	{
		cam.StartFOV = 75;

		cam.EndFOV = 55;
	}
	

	float NewFOV = FMath::Lerp(cam.StartFOV, cam.EndFOV, Value);

	FollowCamera->FieldOfView = NewFOV;
}

void AAxe_ThrowCharacter::AdsCharacterOrientation()
{
	if (IsAdsing == true)
	{
		FollowCamera->bUsePawnControlRotation = true;

		bUseControllerRotationYaw = FollowCamera->bUsePawnControlRotation;

		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		FollowCamera->bUsePawnControlRotation = false;

		bUseControllerRotationYaw = FollowCamera->bUsePawnControlRotation;

		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AAxe_ThrowCharacter::EnemyDetected()
{

	FVector start = GetMesh()->GetSocketLocation(TEXT("Axe_Socket")) + FVector(0,0,50);
	FVector End = (FollowCamera->GetForwardVector() * 5000) + start;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Axe);

	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	//DrawDebugLine(GetWorld(), start, End, FColor::Red, false, 0.5f, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, End, ECC_Camera, CollisionParams);

	ABaseEnemy* enemies = Cast<ABaseEnemy>(OutHit.GetActor());

	if (isHit)
	{
		if (enemies != nullptr)
		{
		    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Hitting Actor: %s"), *OutHit.GetActor()->GetName()));

			HUD->CrossHair_Ref->SetColorAndOpacity(FLinearColor(1, 0, 0));


		}
		

	}

	else
	{
		HUD->CrossHair_Ref->SetColorAndOpacity(FLinearColor(1, 1, 1));


	}

}


#pragma endregion

#pragma region AxeThrow
void AAxe_ThrowCharacter::Throw()
{

	if (hasAxe == true)
	{
		//AnimInstance->Montage_Play(ThrowMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0);

		
		AnimMontage->PlayAxeThrow(AnimInstance, ThrowMontage);

	}
	else
	{
		if (InAir == false || InAir == true)
		{
			Return = true;
		}
	}


}

void AAxe_ThrowCharacter::StopThrow()
{
	IsAxeThrow = false;
}

void AAxe_ThrowCharacter::AxeThrow()
{
	hasAxe = false;
	Is_Axe_Throwing = true;
}



FVector AAxe_ThrowCharacter::AxeDirection()
{
	FVector CamForward = FollowCamera->GetForwardVector();

	return CamForward;
}


//void AAxe_ThrowCharacter::DrawLineTrace()
//{
//	FHitResult OutHit;
//
//	FVector start = Mesh1P->GetSocketLocation(TEXT("Axe_Socket"));
//	FVector ForwardVector = FollowCamera->GetForwardVector();
//	FVector End = ((ForwardVector * 5000) + start);
//
//	FCollisionQueryParams CollisionParams;
//
//	//DrawDebugLine(GetWorld(), start, End, FColor::Red, false, 0.5f, 0, 1);
//
//	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, End, ECC_Visibility, CollisionParams);
//
//
//
//	if (isHit)
//	{
//		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("Hitting Actor: %s"), *OutHit.GetActor()->GetName()));
//
//		FVector AxeStartingLoc = Axe->GetActorLocation();
//
//		FVector EndLoc = (AxeStartingLoc - OutHit.ImpactPoint);
//
//
//
//		EndLoc.Normalize();
//
//		EndLoc *= 25;
//
//		AxeHitLoc = EndLoc + OutHit.ImpactPoint;
//
//		AxeLoc = AxeHitLoc - AxeStartingLoc;
//
//		AxeLoc.Normalize();
//
//		AxeRotation = Axe->GetActorRotation();
//
//		RotationValue = UKismetMathLibrary::FindLookAtRotation(Axe->GetActorLocation(), AxeHitLoc);
//
//		Axe_Z = RotationValue.Yaw;
//
//		InAir = true;
//
//		hasAxe = false;
//
//		Axe->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));
//
//	}
//}
//
//void AAxe_ThrowCharacter::AxeLocationAndRotation(float DeltaTime)
//{
//	FVector DistanceVector = Axe->GetActorLocation() - AxeHitLoc;
//
//	Distance = DistanceVector.Size();
//
//	float Speed = AxeSpeed / 2;
//
//	/*if (Distance <= Speed)
//	{*/
//	if (Distance < 5)
//	{
//		InAir = false;
//
//	}
//	else
//	{
//		/*AxeRotation = FMath::RInterpConstantTo(AxeRotation, FRotator(-15, Axe_Z, 0), DeltaTime, AxeRotationSpeed);
//
//		Axe->SetActorLocationAndRotation(FMath::VInterpConstantTo(Axe->GetActorLocation(), AxeHitLoc, DeltaTime, AxeSpeed), AxeRotation);*/
//
//		AxeRotation = FMath::RInterpConstantTo(AxeRotation, FRotator(AxeRotation.Pitch + (-10), Axe_Z, 0), DeltaTime, AxeRotationSpeed);
//
//		Axe->SetActorLocationAndRotation(FMath::VInterpConstantTo(Axe->GetActorLocation(), AxeHitLoc, DeltaTime, AxeSpeed), AxeRotation);
//
//	}
//
//	//}
//
//	/*else
//	{
//
//	}*/
//
//}
//
//
//void AAxe_ThrowCharacter::Recall()
//{
//	AxeRotation = Axe->GetActorRotation();
//
//	Return = true;
//
//	InAir = true;
//
//	IsAdsing = true;
//
//	
//}
//
//void AAxe_ThrowCharacter::ReturnAxeLocationAndRotation(float DeltaTime)
//{
//	AxeRotation = FMath::RInterpConstantTo(AxeRotation, FRotator(AxeRotation.Pitch + (10),0, 0), DeltaTime, AxeRotationSpeed);
//
//	float Alpha = -abs(HalfDistance - CurrentDistance) / (HalfDistance);
//
//	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
//
//	FVector Target = ((Axe->GetActorRightVector() * FMath::Lerp(HalfDistance, 0.f, Alpha)) + Mesh1P->GetSocketLocation(TEXT("Axe_Socket")));
//
//	Axe->SetActorLocationAndRotation(FMath::VInterpConstantTo(Axe->GetActorLocation(), Target, DeltaTime, AxeReturnRotationSpeed), AxeRotation);
//
//	if (CurrentDistance <= 5)
//	{
//		Return = false;
//		InAir = false;
//		hasAxe = true;
//		Axe->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Axe_Socket"));
//
//		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ref_CamShake, 1.0f);
//
//		IsAdsing = false;
//
//	}
//}
#pragma endregion










