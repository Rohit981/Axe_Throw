// Fill out your copyright notice in the Description page of Project Settings.


#include "OnHitTest.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOnHitTest::AOnHitTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetSimulatePhysics(true);
	Collider->SetNotifyRigidBodyCollision(true);
    Collider->BodyInstance.SetCollisionProfileName("BlockAllDynamic");

	Collider->OnComponentHit.AddDynamic(this, &AOnHitTest::OnHit);


	RootComponent = Collider;

	Mesh = CreateDefaultSubobject< UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(Collider);

}

// Called when the game starts or when spawned
void AOnHitTest::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AOnHitTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOnHitTest::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));

	}
}

