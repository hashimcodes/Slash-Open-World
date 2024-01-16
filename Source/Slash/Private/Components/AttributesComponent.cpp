// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributesComponent::GetDamaged(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

float UAttributesComponent::GetHealthPrecent()
{
	return Health / MaxHealth;
}

bool UAttributesComponent::IsAlive()
{
	return Health > 0.f;
}


void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

