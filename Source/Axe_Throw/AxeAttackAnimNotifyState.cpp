// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeAttackAnimNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Axe_ThrowCharacter.h"
#include "Axe.h"
#include "Enemies.h"

void UAxeAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Player = Cast<AAxe_ThrowCharacter>(MeshComp->GetOwner());

	if (Player)
	{
		Axe_ref = Cast<AAxe>(Player->Axe);

		Weapon = Axe_ref->AxeMesh;
		

		
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Axe Not Found")));
	}
}

void UAxeAttackAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (Player)
	{
		TArray<FHitResult> OutHit;

		StartLocation = Axe_ref->AxeMesh->GetSocketLocation(TEXT("Top"));
		EndLocation = Axe_ref->AxeMesh->GetSocketLocation(TEXT("Bottom"));

		ActorsToIgnore = {Axe_ref->ref_axe};

		UKismetSystemLibrary::SphereTraceMulti(Axe_ref->GetWorld(), StartLocation, EndLocation, 20.f,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true,
			FLinearColor::Red, FLinearColor::Green, 1);

		for (int i = 0; i < OutHit.Num(); i++)
		{
			//AActor* HitActor = OutHit[i].GetActor();

			AEnemies* HitActor = Cast<AEnemies>(OutHit[i].GetActor());

			if (!HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);

				UGameplayStatics::ApplyDamage(HitActor, DamageValue, EventInstigator, Axe_ref, DamageTypeClass);
			}
			

		}
	}
	
}

void UAxeAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	HitActors.Empty();
}
