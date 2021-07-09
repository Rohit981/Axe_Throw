// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Camera.h"
#include "GameHUD.h"
#include "Montage.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimMontage.h"
#include "Axe_ThrowCharacter.generated.h"



class UCurveFloat;
UCLASS(config=Game)
class AAxe_ThrowCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axe, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* splineComp;

public:
	AAxe_ThrowCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	

protected:

	void BeginPlay();

	void Tick(float DeltaTime);

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


	// Sprint Character Variables
	void Sprinting();
	void StopSprinting();

	bool IsSprinting = false;

	//Evade
	void Evade();

	void Roll();

	//ADS
	void ADS();
	void StopADS();

	UFUNCTION()
	void Zoom(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	UCurveFloat* CurveFloat;

	Camera cam;

	void AdsCharacterOrientation();

	void EnemyDetected();

	bool enableDetectTrace = false;

	//Lock On Variables
	void CamLockon();
	
	//Montage
	Montage* AnimMontage;

	//Axe Throw	
	bool IsAxeThrow = false;

	void Throw();
	void StopThrow();

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void AxeThrow();

	//UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* ThrowMontage;

	//void DrawLineTrace();

	FVector AxeHitLoc;

	FRotator AxeRotation;
	FRotator RotationValue;
	float Axe_Z;
	FVector AxeLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Axe")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	int AxeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	int AxeRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	int AxeReturnRotationSpeed;

	void ThrowTrajectory();

	

	int drawSplineNumber = 0;

	

	//Axe 
	UPROPERTY(EditDefaultsOnly, Category = Axe)
	TSubclassOf<class AAxe> AxeSubclass;

	void SpawnAxe();

	void EquipAxe();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	bool Equiped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	bool Un_Equiped = false;

	UPROPERTY(EditAnywhere, Category = "Axe")
	bool Is_Draw = false;

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void Armed();

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* UnEquipMontage;

	UAnimInstance* AnimInstance;

	UFUNCTION()
	void AxeProcessMovementTimeline(float Value);
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Axe)
	UCurveFloat* AxeCurveFloat;

	//Light Attack
	void LightAttack();

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Attack_1_Montage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Attack_2_Montage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Attack_3_Montage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Light_Sprint_Attack_Montage;

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void DisableMovement();

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void EnableMovement();

	bool Is_Moving = true;
	bool Attack_Enable = true;

	UPROPERTY(VisibleAnywhere, Category = "Axe")
	int ComboCounter = 0;

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void IncreaseCounter();

	UPROPERTY(VisibleAnywhere, Category = "Axe")
	bool Is_Counting = false;

	UPROPERTY(VisibleAnywhere, Category = "Axe")
	float ComboTime = 0;

	void LightAttackCounter(float Delta);

	//Heavy Attack
	void HeavyAttack();

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Heavy_Attack_1_Montage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Heavy_Attack_2_Montage;

	UPROPERTY(EditAnywhere, Category = "Axe")
	UAnimMontage* Heavy_Sprint_Attack_Montage;

	UFUNCTION(BlueprintCallable, Category = "Axe")
	void SprintHeavyAttack();

	UPROPERTY(EditAnywhere, Category = "Axe")
		float HeavyAttackRadius;

	TSubclassOf<UDamageType>DamageTypeClass;

	AController* EventInstigator;//ApplyDamage function parameters //

	//Camera
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UMatineeCameraShake> ref_CamShake;

	void LockOnTargetRotation(float DeltaTime);

	//UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gun)
	AGameHUD* HUD;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool Is_Axe_Attacking = false;
	bool Is_Axe_Throwing = false;
	bool InAir = false;
	bool hasAxe = true;

	class AAxe* Axe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	bool IsAdsing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Axe")
	bool Return = false;

	bool projectileHit = false;

	FOccluderVertexArray storehitArray;

	float LaunchVelocity = 3000;

	FVector splineVector;

	//TArray<FVector> splineVectors;

	FHitResult ProjectileOutHit;

	TArray<FInterpCurveVector> splinePointsArray;

	FVector AxeDirection();

	FTimeline AxeCurveTimeline;

	bool startTrajectory = false;

	float splineImpactPoint;

	void AxeMoveDirection();
	
	
};

