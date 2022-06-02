// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusSpeedHigher.h"

// Sets default values
ABonusSpeedHigher::ABonusSpeedHigher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������� root ��������� ��� ������
	MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
	//��������� root ���������
	RootComponent = MyRootComponent;

	//����� ����������� ����� ������
	BonusHigherMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	//������� ��������� ��� ������
	BonusHigherChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

	//��������� ����� ������ � ���������
	BonusHigherChank->SetStaticMesh(BonusHigherMesh);

	//��������� ���� ��� ������ �� ������
	BonusHigherColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/M_Metal_Copper_Inst.M_Metal_Copper_Inst'")).Get();

	//������ ��������� ������� ��� ������
	BonusHigherChank->SetRelativeLocation(FVector(0, 0, 0));
	//������ ���� ������
	BonusHigherChank->SetMaterial(0, BonusHigherColor);
	//������ ����� � root
	BonusHigherChank->SetupAttachment(MyRootComponent);
	//�������� ��������� ������
	BonusHigherChank->SetSimulatePhysics(true);

	//��������� ������� �� ��������
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

//������� ����� �������
void ABonusSpeedHigher::CollectBonusHigher()
{
	//�������� ������ ���� ����� �������� ���� � ��� �����������
	TArray<AActor*> CollectedActors;
	//���������� ���� ������� � ��� �����������
	GetOverlappingActors(CollectedActors);

	//���������� ������ � ����������� �������� ������
	for (int32 i = 0; i < CollectedActors.Num(); ++i)
	{
		AMySnakeActor* const Test = Cast<AMySnakeActor>(CollectedActors[i]);
		if (Test)
		{
			//����������� �������� ������
			Test->StepDelay *= 0.5;
			//���� ��������� ������
			Test->OnGetBonusSound->Play();
			//���������� �����
			Destroy(true, true);
			break; // ������������� ���� ��� ��� ����� �����
		}
	}
}

