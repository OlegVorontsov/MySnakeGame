// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeHeadTrigger.h"

// Sets default values
ASnakeHeadTrigger::ASnakeHeadTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//создаем root компонент
	MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootTrigger"));
	//назначаем root компонент
	RootComponent = MyRootComponent;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerSphere->InitSphereRadius(55.f);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	TriggerSphere->SetupAttachment(MyRootComponent);
	TriggerSphere->SetVisibility(true, true);
	TriggerSphere->SetHiddenInGame(false, false);
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASnakeHeadTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ASnakeHeadTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeHeadTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//проверка реакции головы на коллизии
void ASnakeHeadTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		FString Name = OtherActor->GetName();
		UE_LOG(LogStats, Warning, TEXT("Actor Name: %s"), *Name);
	}
	else
	{
		FString Name = OtherActor->GetName();
		UE_LOG(LogStats, Error, TEXT("Actor Name: %s"), *Name);
	}
}