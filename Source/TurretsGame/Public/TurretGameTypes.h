#pragma once

UENUM(BlueprintType)
enum class EGameType : uint8
{
    EGT_PlayTank UMETA(DisplayName = "Play Tank"),
    EGT_PlayTurret UMETA(DisplayName = "Play Turret")
};

UENUM(BlueprintType)
enum class EPawnGameplayState : uint8
{
    /** Pawn Hidden in a Bush */
    EPGS_Hidden UMETA(DisplayName = "Hidden"),

    EPGS_Visible UMETA(DisplayName = "Visible"),
};
