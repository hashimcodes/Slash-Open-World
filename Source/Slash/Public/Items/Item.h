// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UFUNCTION(BlueprintPure)
	float TransformedSine();

	UFUNCTION(BlueprintPure)
	float TransformedCos();
	float RunningTime = 0;

	UPROPERTY(EditAnywhere)
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere)
	float TimeConstant = 5.f;
	


	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
};
