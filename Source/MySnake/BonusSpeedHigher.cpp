// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusSpeedHigher.h"

// Sets default values
ABonusSpeedHigher::ABonusSpeedHigher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//создаем root компонент дл€ бонуса
	MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
	//назначаем root компонент
	RootComponent = MyRootComponent;

	//берем стандартную сферу движка
	BonusHigherMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	//создаем компонент дл€ бонуса
	BonusHigherChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

	//загружаем сферу бонуса в компонент
	BonusHigherChank->SetStaticMesh(BonusHigherMesh);

	//загружаем цвет дл€ бонуса из движка
	BonusHigherColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/M_Metal_Copper_Inst.M_Metal_Copper_Inst'")).Get();

	//задаем начальную позицию дл€ бонуса
	BonusHigherChank->SetRelativeLocation(FVector(0, 0, 0));
	//задаем цвет бонуса
	BonusHigherChank->SetMaterial(0, BonusHigherColor);
	//крепим бонус к root
	BonusHigherChank->SetupAttachment(MyRootComponent);
	//включаем симул€цию физики
	BonusHigherChank->SetSimulatePhysics(true);

	//добавл€ем реакцию на коллизии
	BonusHigherChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BonusHigherChank->SetCollisionObjectType(ECC_WorldDynamic);
	BonusHigherChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

}

// Called when the game starts or when spawned
void ABonusSpeedHigher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABonusSpeedHigher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeBufer += DeltaTime;
	if (TimeBufer > StepDelay)
	{
		Destroy(true, true);
	}
	CollectBonusHigher();
}

//функци€ сбора бонусов
void ABonusSpeedHigher::CollectBonusHigher()
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
			//увеличиваем скорость змейки
			Test->StepDelay *= 0.5;
			//звук получени€ бонуса
			Test->OnGetBonusSound->Play();
			//уничтожаем бонус
			Destroy(true, true);
			break; // останавливаем цикл так как нашли бонус
		}
	}
}

