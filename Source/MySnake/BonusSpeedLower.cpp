// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusSpeedLower.h"

// Sets default values
ABonusSpeedLower::ABonusSpeedLower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//создаем root компонент дл€ бонуса
	MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
	//назначаем root компонент
	RootComponent = MyRootComponent;

	//берем стандартную сферу движка
	BonusLowerMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	//создаем компонент дл€ бонуса
	BonusLowerChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

	//загружаем сферу бонуса в компонент
	BonusLowerChank->SetStaticMesh(BonusLowerMesh);

	//загружаем цвет дл€ бонуса из движка
	BonusLowerColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/M_Water_Lake_Inst.M_Water_Lake_Inst'")).Get();

	//задаем начальную позицию дл€ бонуса
	BonusLowerChank->SetRelativeLocation(FVector(0, 0, 0));
	//задаем цвет бонуса
	BonusLowerChank->SetMaterial(0, BonusLowerColor);
	//крепим бонус к root
	BonusLowerChank->SetupAttachment(MyRootComponent);
	//включаем симул€цию физики
	BonusLowerChank->SetSimulatePhysics(true);

	//добавл€ем реакцию на коллизии
	BonusLowerChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BonusLowerChank->SetCollisionObjectType(ECC_WorldDynamic);
	BonusLowerChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void ABonusSpeedLower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABonusSpeedLower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeBufer += DeltaTime;
	if (TimeBufer > StepDelay)
	{
		Destroy(true, true);
	}
	CollectBonusLower();
}

//функци€ сбора бонусов
void ABonusSpeedLower::CollectBonusLower()
{
	//объвл€ем массив куда будем заносить всех с кем столкнулись
	TArray<AActor*> CollectedActors;
	//перебираем всех актеров с кем столкнулись
	GetOverlappingActors(CollectedActors);

	//перебираем массив и увеличиваем скорость змейки
	for (int32 i = 0; i < CollectedActors.Num(); ++i)
	{
		AMySnakeActor* const Test = Cast<AMySnakeActor>(CollectedActors[i]);
		if (Test)
		{
			//уменьшаем скорость змейки
			Test->StepDelay *= 2;
			//звук получени€ бонуса
			Test->OnGetBonusSound->Play();
			//уничтожаем бонус
			Destroy(true, true);
			break; // останавливаем цикл так как нашли бонус
		}
	}
}

