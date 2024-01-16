// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributesComponent;
class UHealthBarComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
	//Montage Functions
	void PlayHitReactMontage(const FName& SectionName);
	void Die();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	UPROPERTY(VisibleAnywhere)
	UAttributesComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarComponent;

	//Animation Montages
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticles;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	void DirectionalHit(const FVector& ImpactPoint);

};
