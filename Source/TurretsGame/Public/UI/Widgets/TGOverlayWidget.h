// TurretGame by Team #1. AlphaNova courses

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TGOverlayWidget.generated.h"

class UProgressBar;
class UImage;
class UBackgroundBlur;
class UTextBlock;
class UButton;
class USoundCue;

UCLASS()
class TURRETSGAME_API UTGOverlayWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    UImage* HealthBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* ShootDelayBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* SpeedBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* EnemyHealthBarImage;

    UPROPERTY(meta = (BindWidget))
    UImage* InstructionImage;

    UPROPERTY(meta = (BindWidget))
    UImage* InstructionBackgroundImage;

    UPROPERTY(meta = (BindWidget))
    UBackgroundBlur* PauseBlur;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PauseText;

    UPROPERTY(meta = (BindWidget))
    UBackgroundBlur* GameOverBlur;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* GameOverText;

    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MenuButton;

    UPROPERTY(EditDefaultsOnly, Category = "TG|Sounds")
    USoundCue* HoveredSound;

private:
    UFUNCTION()
    void OnResumeButtonHovered();

    UFUNCTION()
    void OnMenuButtonHovered();

    void HideAllComponents();
    void BindButtonsHovered();
    void PlayHoveredSound();
};
