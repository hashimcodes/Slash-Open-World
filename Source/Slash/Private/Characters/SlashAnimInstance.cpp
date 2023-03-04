// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter) 
	{
		SlashCharacterMovementComponent = SlashCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovementComponent->Velocity);
		IsFalling = SlashCharacterMovementComponent->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();
	}
}
