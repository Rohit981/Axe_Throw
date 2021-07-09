// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

void AGameHUD::BeginPlay()
{
	if (CrossHair_HUD) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it.
		CrossHair_Ref = CreateWidget<UUserWidget>(GetWorld(), CrossHair_HUD);

		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (CrossHair_Ref)
		{
			//let add it to the view port
			CrossHair_Ref->AddToViewport();

			CrossHair_Ref->SetVisibility(ESlateVisibility::Hidden);
		}

	}
}
