// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusSpeedLower.h"

// Sets default values
ABonusSpeedLower::ABonusSpeedLower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������� root ��������� ��� ������
	MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
	//��������� root ���������
	RootComponent = MyRootComponent;

	//����� ����������� ����� ������
	BonusLowerMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

	//������� ��������� ��� ������
	BonusLowerChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

	//��������� ����� ������ � ���������
	BonusLowerChank->SetStaticMesh(BonusLowerMesh);

	//��������� ���� ��� ������ �� ������
	BonusLowerColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/M_Water_Lake_Inst.M_Water_Lake_Inst'")).Get();

	//������ ��������� ������� ��� ������
	BonusLowerChank->SetRelativeLocation(FVector(0, 0, 0));
	//������ ���� ������
	BonusLowerChank->SetMaterial(0, BonusLowerColor);
	//������ ����� � root
	BonusLowerChank->SetupAttachment(MyRootComponent);
	//�������� ��������� ������
	BonusLowerChank->SetSimulatePhysics(true);

	//��������� ������� �� ��������
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

//������� ����� �������
void ABonusSpeedLower::CollectBonusLower()
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
			//��������� �������� ������
			Test->StepDelay *= 2;
			//���� ��������� ������
			Test->OnGetBonusSound->Play();
			//���������� �����
			Destroy(true, true);
			break; // ������������� ���� ��� ��� ����� �����
		}
	}
}

