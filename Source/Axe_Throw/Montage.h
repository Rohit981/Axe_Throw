// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"

/**
 * 
 */
class AXE_THROW_API Montage
{
public:
	Montage();
	~Montage();

	void PlayAxeThrow(UAnimInstance* AnimInstance, UAnimMontage* ThrowMontage);



private:
	
};
