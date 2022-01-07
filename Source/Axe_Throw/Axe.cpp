// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAxe::AAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AxeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Axe"));
	AxeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	
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

	if (AxeMesh->IsSimulatingPhysics() == true)
	{
		physicsTime -= DeltaTime;
	}
	
	if (physicsTime <= 0.f && AxeMesh->IsSimulatingPhysics() == true)
	{
		AxeMesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::SimulationUpatesComponentTransform;

		AxeMesh->SetSimulatePhysics(false);


	}
	
	
	HitDirection(DeltaTime);

	if (IS_Acting_Force == true)
	{
		AxeThrowHit();

		AxeTransform();
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

}



void AAxe::SphereTraceCollider(float Radius)
{
	FVector Start = AxeMesh->GetSocketLocation(TEXT("Top"));
	FVector End = AxeMesh->GetSocketLocation(TEXT("Bottom"));

	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(ref_axe);

	    Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Radius,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true,
				FLinearColor::Red, FLinearColor::Green, 1);

}

void AAxe::AxeThrowHit()
{
	SphereTraceCollider(AxeThrowSphereRadius);

	for (int i = 0; i < OutHit.Num(); i++)
	{
		AEnemies* enemies = Cast<AEnemies>(OutHit[i].GetActor());

		if (Hit)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Hit: %s"), *OutHit[i].GetComponent()->GetName()));

			if (enemies != nullptr)
			{
				if (enemies->ActorHasTag(FName(TEXT("Stun"))))
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stun Enemy")));

					//IS_Acting_Force = false;

					AxeHitLocation = OutHit[i].GetComponent()->GetRelativeLocation();

					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Axe Hit Location: %s"), *AxeHitLocation.ToString()));

					//enemies->Is_Damaged = true;

					Is_Changing_Direction = true;

				}

				/*enemies->EnemyDamage();
				
				if (ref_axe->Return == false)
				{

					IS_Acting_Force = false;

					ref_axe->InAir = false;

				}*/
			}

			
		}
	}

	
}

void AAxe::ThrowAxe()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Thrown")));

	this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));

	IS_Acting_Force = true;

	ref_axe->Is_Axe_Throwing = false;



}

void AAxe::AxeTransform()
{
	FRotator AxeRotation = this->GetActorRotation();

	AxeMesh->AddLocalRotation(FRotator( AxeRotationSpeed, 0, 0));

	AxeMesh->AddRelativeLocation(FVector(ref_axe->AxeDirection() * AxeMoveSpeed));

	//ProjectileComponent->bSimulationEnabled = true;

}

void AAxe::ReturnAxe(float DeltaTime)
{

	AxeMesh->SetSimulatePhysics(false);

	
	ref_axe->InAir = true;

	ref_axe->IsAdsing = true;

	CurrentDistance = (GetActorLocation() - ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket"))).Size();
	HalfDistance = ((ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket")) - GetActorLocation()).Size())/4;

	float Alpha = -abs(HalfDistance - CurrentDistance) / (HalfDistance);
	
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	
	FVector Target = ((GetActorRightVector() * FMath::Lerp(HalfDistance, 0.f, Alpha)) + ref_axe->GetMesh()->GetSocketLocation(TEXT("Axe_Socket")));

	
	AxeMesh->AddLocalRotation(FRotator(-AxeRotationSpeed, 0, 0));

	SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), Target, DeltaTime, AxeReturnSpeed));

	

	if (CurrentDistance <= 5)
	{
		ref_axe->Return = false;
		ref_axe->InAir = false;
		ref_axe->hasAxe = true;

		this->AttachToComponent(ref_axe->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Axe_Socket"));
		
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(ref_CamShake, 1.0f);

		ref_axe->IsAdsing = false;


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





