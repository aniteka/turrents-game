// TurretGame by Team #1. AlphaNova courses

#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGMenuWidget.h"
#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "GameMode/TGGameMode.h"
#include "GameMode/TGMenuGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGTurret.h"
#include "Player/TGPlayerController.h"

void ATG_HUD::BeginPlay()
{
    Super::BeginPlay();

    SelectWidgetByGameMode();
}

void ATG_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    bool bCanUpdateBars = UpdateOwnerPawnVar() && OverlayWidget && OverlayWidget->ShootDelayBarImage && OverlayWidget->SpeedBarImage;
    if (bCanUpdateBars)
    {
        SetPercentBar(OwnerPawn->GetShootDelayPercent(), OverlayWidget->ShootDelayBarImage);
        SetPercentBar(OwnerPawn->GetSpeedPercent(), OverlayWidget->SpeedBarImage);
    }
}

void ATG_HUD::Pause(bool bPaused)
{
    bool bWidgets = OverlayWidget && OverlayWidget->ResumeButton && OverlayWidget->MenuButton && OverlayWidget->PauseText &&
                    OverlayWidget->PauseBlur && OverlayWidget->InstructionImage && OverlayWidget->InstructionBackgroundImage;
    if (!bWidgets) return;

    if (bPaused)
    {
        ShowPauseComponents(ESlateVisibility::Visible);
    }
    else
    {
        ShowPauseComponents(ESlateVisibility::Hidden);
    }
}

void ATG_HUD::GameOver(bool bWin) 
{
    if (!OverlayWidget || !OverlayWidget->GameOverBlur || !OverlayWidget->GameOverText) return;

    OverlayWidget->GameOverBlur->SetVisibility(ESlateVisibility::Visible);
    OverlayWidget->GameOverText->SetVisibility(ESlateVisibility::Visible);

    if (bWin)
    {
        OverlayWidget->GameOverText->SetText(WinMessage);
        OverlayWidget->GameOverText->SetColorAndOpacity(FLinearColor::Green);
        
    }
    else
    {
        OverlayWidget->GameOverText->SetText(LoseMessage);
        OverlayWidget->GameOverText->SetColorAndOpacity(FLinearColor::Red);
    }
}

void ATG_HUD::AddMenuWidget()
{
    if (!GetOwningPlayerController() && MenuWidgetClass) return;

    MenuWidget = CreateWidget<UTGMenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
    if (!MenuWidget) return;

    MenuWidget->AddToViewport();
}

void ATG_HUD::AddOverlayWidget()
{
    if (!GetOwningPlayerController() && OverlayWidgetClass) return;

    OverlayWidget = CreateWidget<UTGOverlayWidget>(GetOwningPlayerController(), OverlayWidgetClass);
    if (!OverlayWidget) return;

    OverlayWidget->AddToViewport();
}

void ATG_HUD::OnResumeButtonClicked()
{
    Pause(false);

    if (!GetTGPlayerController()) return;
    GetTGPlayerController()->SetInputModeGameOnly();
}

void ATG_HUD::OnMenuButtonClicked()
{
    if (!GetTGPlayerController()) return;
    GetTGPlayerController()->GoToMenu();
}

void ATG_HUD::SelectWidgetByGameMode()
{
    if (!GetWorld()) return;

    const auto GameModeMenu = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (GameModeMenu)
    {
        AddMenuWidget();
    }
    else
    {
        const auto GameMode = GetWorld()->GetAuthGameMode<ATGGameMode>();
        if (GameMode)
        {
            AddOverlayWidget();
            OverlayWidgetCreateHandle();

            if (OverlayWidget && OverlayWidget->ResumeButton && OverlayWidget->MenuButton)
            {
                OverlayWidget->ResumeButton->OnClicked.AddDynamic(this, &ATG_HUD::OnResumeButtonClicked);
                OverlayWidget->MenuButton->OnClicked.AddDynamic(this, &ATG_HUD::OnMenuButtonClicked);
            }
        }
    }
}

void ATG_HUD::OverlayWidgetCreateHandle()
{
    if (!GetOwningPawn()) return;

    ATGTank* PlayerTank = Cast<ATGTank>(GetOwningPawn());
    bool bValidImages = OverlayWidget && OverlayWidget->HealthBarImage && OverlayWidget->ShootDelayBarImage && OverlayWidget->SpeedBarImage;

    if (PlayerTank && bValidImages)
    {
        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
        EnableBar(OverlayWidget->SpeedBarImage, true);

        SetPercentBar(PlayerTank->GetHealthPercent(), OverlayWidget->HealthBarImage);
    }
    else if (bValidImages)
    {
        ATGTurret* PlayerTurret = Cast<ATGTurret>(GetOwningPawn());
        if (!PlayerTurret) return;

        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
    }
}

void ATG_HUD::EnableBar(UImage* BarToEnable, bool bEnable)
{
    if (!BarToEnable) return;
    (bEnable) ? BarToEnable->SetVisibility(ESlateVisibility::Visible) : BarToEnable->SetVisibility(ESlateVisibility::Hidden);
}

void ATG_HUD::SetPercentBar(float Percent, UImage* BarToChange)
{
    if (!BarToChange || BarToChange->GetVisibility() != ESlateVisibility::Visible) return;

    auto DymMaterial = BarToChange->GetDynamicMaterial();
    if (!DymMaterial) return;

    DymMaterial->SetScalarParameterValue(FName(TEXT("ProgressAlpha")), Percent);
}

bool ATG_HUD::UpdateOwnerPawnVar()
{
    OwnerPawn = (!OwnerPawn) ? Cast<ATGBasePawn>(GetOwningPawn()) : OwnerPawn;
    return OwnerPawn ? true : false;
}

void ATG_HUD::ShowPauseComponents(const ESlateVisibility& Visibility)
{
    OverlayWidget->ResumeButton->SetVisibility(Visibility);
    OverlayWidget->MenuButton->SetVisibility(Visibility);
    OverlayWidget->PauseText->SetVisibility(Visibility);
    OverlayWidget->PauseBlur->SetVisibility(Visibility);
    OverlayWidget->InstructionImage->SetVisibility(Visibility);
    OverlayWidget->InstructionBackgroundImage->SetVisibility(Visibility);
}

ATGPlayerController* ATG_HUD::GetTGPlayerController()
{
    if (!GetOwningPlayerController()) return nullptr;
    return Cast<ATGPlayerController>(GetOwningPlayerController());
}
