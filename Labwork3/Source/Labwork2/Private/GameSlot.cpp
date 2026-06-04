#include "GameSlot.h"
#include "UnitBase.h"
#include "TBPlayerController.h"

// Sets default values
AGameSlot::AGameSlot()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(50.f, 50.f, 2.f));
	Box->SetCollisionResponseToAllChannels(ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		DefaultSlotMesh(TEXT("/Engine/BasicShapes/Plane"));

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(RootComponent);

	if (DefaultSlotMesh.Succeeded())
	{
		Plane->SetStaticMesh(DefaultSlotMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		DefaultMat(TEXT("Material'/Game/Materials/MAT_GridSlot.MAT_GridSlot'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		HighlightMat(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Highlighted.MAT_GridSlot_Highlighted'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		OffensiveMat(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Offensive.MAT_GridSlot_Offensive'"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		SupportiveMat(TEXT("MaterialInstanceConstant'/Game/Materials/MAT_GridSlot_Supportive.MAT_GridSlot_Supportive'"));

	DefaultMaterial = DefaultMat.Object;
	HighlightMaterial = HighlightMat.Object;
	OffensiveMaterial = OffensiveMat.Object;
	SupportiveMaterial = SupportiveMat.Object;

	if (DefaultMaterial)
	{
		Plane->SetMaterial(0, DefaultMaterial);
	}
}

void AGameSlot::BeginPlay()
{
	Super::BeginPlay();

	OnClicked.AddUniqueDynamic(this, &AGameSlot::OnGridClicked);
}

void AGameSlot::OnGridClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (ATBPlayerController* PlayerController =
		GWorld->GetFirstPlayerController<ATBPlayerController>())
	{
		PlayerController->OnActorClicked(this, ButtonPressed);
	}
}

void AGameSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameSlot::SetState(EGridState NewState)
{
	GridState = NewState;

	switch (NewState)
	{
	case GS_Default:
		if (DefaultMaterial)
			Plane->SetMaterial(0, DefaultMaterial);
		break;

	case GS_Highlighted:
		if (HighlightMaterial)
			Plane->SetMaterial(0, HighlightMaterial);
		break;

	case GS_Offensive:
		if (OffensiveMaterial)
			Plane->SetMaterial(0, OffensiveMaterial);
		break;

	case GS_Supportive:
		if (SupportiveMaterial)
			Plane->SetMaterial(0, SupportiveMaterial);
		break;
	}
}

void AGameSlot::SpawnUnitHere(TSubclassOf<AUnitBase>& UnitClass)
{
	FVector Location = GetActorLocation();

	AUnitBase* NewUnit =
		Cast<AUnitBase>(GWorld->SpawnActor(UnitClass, &Location));

	if (NewUnit)
	{
		NewUnit->AssignToSlot(this);
	}
}