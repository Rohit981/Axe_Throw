// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AxeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Axe"));
	AxeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	RootComponent = AxeMesh;

	
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	/*HitCollider->SetSimulatePhysics(true);
	HitCollider->SetNotifyRigidBodyCollision(true);*/
	
	//HitCollider->OnComponentHit.AddDynamic(this, &AAxe::OnCompHit);

	HitCollider->AttachToComponent(AxeMesh, FAttachmentTransformRules::KeepRelativeTransform);

	AxePrimitive = Cast<UPrimitiveComponent>(this->GetComponentByClass(UPrimitiveComponent::StaticClass()));

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	
	
}





// Called when the game starts or when spawned
void AAxe::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ref_axe = Cast<AAxe_ThrowCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	/*if (physicsTime <= 0)
	{
		
		AxeMesh->SetSimulatePhysics(false);

	}*/

	ProjectileComponent->InitialSpeed = 500;
	ProjectileComponent->MaxSpeed = 500;
	
	
	HitDirection(DeltaTime);

	if (IS_Acting_Force == true)
	{

		
		AxeThrowHit();

		AxeTransform();

		//ref_axe->AxeMoveDirection();
		

		
	}


	if (ref_axe->Is_Axe_Throwing == true)
	{
		ThrowAxe();
	}

	if (ref_axe->Return == true)
	{
		IS_Acting_Force = false;
		ReturnAxe(DeltaTime);

		
		AxeThrowHit();
	}

	/*if (GetActorLocation().X > ref_axe->splineImpactPoint)
	{
		ref_axe->startTrajectory = false;
	}*/

}



void AAxe::SphereTraceCollider(float Radius)
{
	FVector Start = AxeMesh->GetSocketLocation(TEXT("Top"));
	FVector End = AxeMesh->GetSocketLocation(TEXT("Bottom"));

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(ref_axe);

	    Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Radius,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true,
				FLinearColor::Red, FLinearColor::Green, 1);

}

void AAxe::AxeThrowHit()
{
	SphereTraceCollider(AxeThrowSphereRadius);



	for (int i = 0; i < OutHit.Num(); i++)
	{
		enemies = Cast<ABaseEnemy>(OutHit[i].GetActor());

		if (Hit)
		{
			
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Hit: %s"), *OutHit[i].GetComponent()->GetName()));

			//ref_axe->AxeCurveTimeline.

			IS_Acting_Force = false;

			if (enemies != nullptr)
			{
				//enemies->EnemyDamage();
				
				if (ref_axe->Return == true)
				{

					IS_Acting_Force = false;
					GetWorldTimerManager().SetTimer(Time_Handle_Manager, this, &AAxe::AxeThrowDMG, 0.01f, false);


					ref_axe->InAir = false;

				}
				else
				{
					if (enemies->ActorHasTag(FName(TEXT("Stun"))))
					{
						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stun Enemy")));

						//IS_Acting_Force = false;

						AxeHitLocation = OutHit[i].GetComponent()->GetRelativeLocation();

						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Axe Hit Location: %s"), *AxeHitLocation.ToString()));

						GetWorldTimerManager().SetTimer(Time_Handle_Manager, this, &AAxe::AxeThrowDMG, 0.01f, false);

						Is_Changing_Direction = true;

					}
				}
			}
			else
			{
				if (ref_axe->Return == false)
				{
					IS_Acting_Force = false;
					
					FRotator Rot = ref_axe->GetFollowCamera()->GetComponentRotation();

					//AxeMesh->SetWorldRotation(CameraRot);

					SetActorRotation(FRotator(Rot.Roll + GetActorRotation().Roll /*+ 180*/ , OutHit[i].GetActor()->GetActorRotation().Pitch, Rot.Yaw +GetActorRotation().Yaw /*- 45*/));
					

				}
			}

			
		}
		
	}

	
}

void AAxe::AxeThrowDMG()
{
	UGameplayStatics::ApplyDamage(enemies, 10, EventInstigator, this, DamageTypeClass);

}

void AAxe::ThrowAxe()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Thrown")));

	this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));

	IS_Acting_Force = true;

	ref_axe->Is_Axe_Throwing = false;


}

void AAxe::AxeTransform()
{
	//FRotator AxeRotation = this->GetActorRotation();

	


	//AxeMesh->AddRadialForce(ref_axe->AxeDirection(), 20, AxeMoveSpeed, ERadialImpulseFalloff::RIF_Constant, true);

	//AxeMesh->AddForce(ref_axe->AxeDirection() * AxeMoveSpeed * AxeMesh->GetMass());
	

	//AxeMesh->AddAngularImpulse(ref_axe->AxeDirection() * AxeMoveSpeed);

	//AxeMesh->AddAngularImpulse(ref_axe->AxeDirection() * AxeMoveSpeed);
	//AxeMesh->AddTorque(ref_axe->AxeDirection() * AxeMoveSpeed);

	//AxeMesh->AddForceAtLocation(AxeMesh->GetForwardVector() * AxeMoveSpeed, AxeMesh->GetForwardVector());


	
	//ProjectileComponent->bSimulationEnabled = 1;
	//ProjectileComponent->bAutoActivate = 1;
	if (IsphysicsEnabled == false)
	{
		AxeMesh->AddRelativeLocation(ref_axe->AxeDirection()* AxeMoveSpeed, false, nullptr, ETeleportType::TeleportPhysics);

		AxeMesh->AddLocalRotation(FRotator( AxeRotationSpeed, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
	}

	projectileTime += GetWorld()->DeltaTimeSeconds;

	if (projectileTime >= 1.5)
	{
		IsphysicsEnabled = true;

		//AxeMesh->SetSimulatePhysics(true);

		//AxeMesh->AddForce(GetActorForwardVector() * AxeMoveSpeed);

		ProjectileComponent->bSimulationEnabled = true;

		ProjectileComponent->Velocity = GetActorForwardVector() * ProjectileComponent->InitialSpeed;

		
		//AxeMesh->AddLocalRotation(UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), GetActorForwardVector()+ AxeMoveSpeed));


	}
	//ProjectileComponent->Velocity = ref_axe->AxeDirection() * ProjectileComponent->InitialSpeed;

	/*AxeMesh->SetSimulatePhysics(true);

	

	

	//ProjectileComponent->SetVelocityInLocalSpace(AxeMesh->GetForwardVector());

	//
	/*for (int i = 0; i < ref_axe->storehitArray.Num(); i++)
	{*/

		

	

		//SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), ref_axe->storehitArray[i].Projection(), GetWorld()->DeltaTimeSeconds, AxeMoveSpeed));

		/*FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;*/

		//UKismetSystemLibrary::MoveComponentTo(RootComponent, ref_axe->storehitArray[i], FRotator(0,0,0),false, false, AxeMoveSpeed* GetWorld()->DeltaTimeSeconds,true ,EMoveComponentAction::Move , LatentInfo);
		//SetActorLocation()

		
		
		
	/*}*/

		

		//AxeMesh->AddRelativeLocation(FMath::VInterpConstantTo(this->GetActorLocation(), ref_axe->ProjectileOutHit.ImpactPoint, DeltaTime, AxeMoveSpeed));


}

void AAxe::ReturnAxe(float DeltaTime)
{

	//ref_axe->AxeCurveTimeline.Reverse();
	
	ProjectileComponent->bSimulationEnabled = false;

	AxeMesh->SetSimulatePhysics(false);


	physicsTime = 0;

	ref_axe->InAir = true;

	ref_axe->IsAdsing = true;

	CurrentDistance = (GetActorLocation() - ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket"))).Size();
	HalfDistance = ((ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket")) - GetActorLocation()).Size())/4;

	float Alpha = -abs(HalfDistance - CurrentDistance) / (HalfDistance);
	
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	
	FVector Target = ((GetActorRightVector() * FMath::Lerp(HalfDistance, 0.f, Alpha)) + ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket")));

	AxeMesh->AddLocalRotation(FRotator(-AxeRotationSpeed, 0, 0));

	SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), Target, DeltaTime, AxeReturnSpeed),false, nullptr, ETeleportType::TeleportPhysics);

	if (CurrentDistance <= 5)
	{
		ref_axe->Return = false;
		ref_axe->InAir = false;
		ref_axe->hasAxe = true;

		this->AttachToComponent(ref_axe->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Axe_Socket"));
		
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ref_CamShake, 1.0f);

		ref_axe->IsAdsing = false;

		projectileTime = 0;

		IsphysicsEnabled = false;
	}

	
}

void AAxe::HitDirection(float DeltaTime)
{

	if (Is_Changing_Direction == true)
	{
		IS_Acting_Force = false;

		FVector AxeTargetLocation = FVector(AxeHitLocation.X + 500, this->GetActorLocation().Y, AxeHitLocation.Z + 600);

		//FRotator AxeRotation = FMath::RInterpConstantTo(this->GetActorRotation(), FRotator(this->GetActorRotation().Euler().X, 0,0), DeltaTime, 50);
		AxeMesh->AddLocalRotation(FRotator(AxeRotationSpeed, 0, 0));

		SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), AxeTargetLocation, DeltaTime, 400));

		physicsTime += DeltaTime;
	
	}

	if (physicsTime >= 1.5)
	{
		Is_Changing_Direction = false;

		IS_Axe_Colliding = true;

		AxeMesh->SetSimulatePhysics(true);

		//SetActorTransform(AxeMesh->GetComponentTransform());

		

	}
}





