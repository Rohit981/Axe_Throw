// Fill out your copyright notice in the Description page of Project Settings.


#include "Montage.h"

Montage::Montage()
{
	
}

Montage::~Montage()
{
}


void Montage::PlayAxeThrow(UAnimInstance* AnimInstance, UAnimMontage* ThrowMontage)
{
	
	AnimInstance->Montage_Play(ThrowMontage, 1.5f, EMontagePlayReturnType::MontageLength, 0);
	
}
