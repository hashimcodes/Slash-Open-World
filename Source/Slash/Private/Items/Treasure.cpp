// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SlashCharacter.h"


void ATreasure::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		if (PickSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickSound,
				GetActorLocation()
			);
		}

		Destroy();
	}
}
