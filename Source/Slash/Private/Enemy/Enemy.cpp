// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributesComponent.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(false);
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstence = GetMesh()->GetAnimInstance();
	if (AnimInstence && HitReactMontage)
	{
		AnimInstence->Montage_Play(HitReactMontage);
		AnimInstence->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstence = GetMesh()->GetAnimInstance();
	if (AnimInstence && DeathMontage)
	{
		AnimInstence->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SelectionName = "";
		switch (Selection)
		{
			case 0:
				SelectionName = FName("Death1");
				DeathPose = EDeathPose::EDP_Death1;
				break;
			case 1:
				SelectionName = FName("Death2");
				DeathPose = EDeathPose::EDP_Death2;
				break;
			case 2:
				SelectionName = FName("Death3");
				DeathPose = EDeathPose::EDP_Death3;
				break;
			default:
				break;
		}
		AnimInstence->Montage_JumpToSection(SelectionName, DeathMontage);
	}
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Length();
		if (DistanceToTarget > CombatRadius)
		{
			CombatTarget = nullptr;
			if (HealthBarComponent)
			{
				HealthBarComponent->SetVisibility(false);
			}
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint);
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(true);
	}
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHit(ImpactPoint);
	}
	else
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarComponent)
	{
		Attributes->GetDamaged(DamageAmount);
		HealthBarComponent->SetHealthBarPercent(Attributes->GetHealthPrecent());
	}
	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

void AEnemy::DirectionalHit(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("Back");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("Right");
	}

	PlayHitReactMontage(Section);
}

