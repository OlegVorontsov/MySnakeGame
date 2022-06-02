// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MySnakeActor.h"

#include "BonusSpeedLower.generated.h"

UCLASS()
class MYSNAKE_API ABonusSpeedLower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABonusSpeedLower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//��������� ����� �������� ��� ������
	class USphereComponent* MyRootComponent;

	//��������� ������ ��� ������
	class UStaticMesh* BonusLowerMesh;

	//��������� ������ ������
	UPROPERTY(EditAnyWhere)
	class UStaticMeshComponent* BonusLowerChank;

	//��������� ���� ��� ������
	class UMaterialInstance* BonusLowerColor;

	//������� ����� �������
	void CollectBonusLower();

	//����������� ������� ����� �������
	float StepDelay = 8.f;
	//���������� �������
	float TimeBufer = 0;
};
