// Copyright (c) 2021 LootLocker

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "LootLockerResponse.h"
#include "LootLockerConfig.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLootLockerGameSDK, Log, All);

UENUM(BlueprintType)
enum class ELootLockerHTTPMethod : uint8
{
	GET = 0         UMETA(DisplayName = "GET"),
	POST = 1        UMETA(DisplayName = "POST"),
	DELETE = 2      UMETA(DisplayName = "DELETE"),
	PUT = 3         UMETA(DisplayName = "PUT"),
	HEAD = 4        UMETA(DisplayName = "HEAD"),
	CREATE = 5      UMETA(DisplayName = "CREATE"),
	OPTIONS = 6     UMETA(DisplayName = "OPTIONS"),
	PATCH = 7       UMETA(DisplayName = "PATCH"),
	UPLOAD = 8      UMETA(DisplayName = "UPLOAD")
};

UENUM(BlueprintType)
enum class ELootLockerPlatformType : uint8
{
	Android = 0				UMETA(DisplayName = "Android"),
	//WhiteLabel = 0			UMETA(DisplayName = "WhiteLabel"),
	Ios = 1					UMETA(DisplayName = "Ios"),
	Steam = 2				UMETA(DisplayName = "Steam"),
	NintendoSwitch = 3		UMETA(DisplayName = "NintendoSwitch"),
	PlayStationNetwork = 4  UMETA(DisplayName = "PlayStationNetwork"),
	Xbox = 5				UMETA(DisplayName = "Xbox")
};

USTRUCT(BlueprintType)
struct FLootLockerEndPoints
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
	FString endpoint;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
	ELootLockerHTTPMethod requestMethod = ELootLockerHTTPMethod::GET;
};

USTRUCT(BlueprintType)
struct FLootLockerGetRequests
{
	GENERATED_BODY()
	TArray<FStringFormatArg> args;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FResponseCallbackBP, FLootLockerResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLootLockerResponseCallback, FLootLockerResponse, Response);
DECLARE_DELEGATE_OneParam(FResponseCallback, FLootLockerResponse);

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "LootLocker SDK Settings"))
class LOOTLOCKERSDK_API ULootLockerConfig : public UObject
{
	GENERATED_BODY()
public:
	static FString GetEnum(const TCHAR* Enum, int32 EnumValue);
    static FString GetRequestMethodString(ELootLockerHTTPMethod RequestMethod)
    {
        return GetEnum(TEXT("ELootLockerHTTPMethod"), static_cast<int32>(RequestMethod));
    }
	UFUNCTION()
	bool IsLegacyAPIKey() const
    {
		return LootLockerGameKey.Find("dev_", ESearchCase::CaseSensitive) == -1 && LootLockerGameKey.Find("prod_", ESearchCase::CaseSensitive) == -1;
    }
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        if(PropertyChangedEvent.GetPropertyName() == "LootLockerGameKey")
        {
			IsLegacyKey = IsLegacyAPIKey();
        }
		UObject::PostEditChangeProperty(PropertyChangedEvent);
    }
	virtual void PostInitProperties() override
    {
		UObject::PostInitProperties();
		IsLegacyKey = IsLegacyAPIKey();
    }
public:
	// API Key used to talk to LootLocker. The API key can be found in `Settings > API Keys` in the Web Console: https://console.lootlocker.com/settings/api-keys
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker", Meta = (DisplayName = "LootLocker API Key"))
	FString LootLockerGameKey;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category = "LootLocker", Meta = (EditCondition = "IsLegacyKey", EditConditionHides), Meta = (MultiLine = true), Meta = (DisplayName = "WARNING: "), Transient)
    FString LegacyKeyWarning = "You are using a legacy API Key, please generate a new one here: https://console.lootlocker.com/settings/api-keys";
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
    FString GameVersion;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
	bool AllowTokenRefresh = true;
	// Domain Key used to talk to LootLocker. The Domain key can be found in `Settings > API Keys` in the Web Console: https://console.lootlocker.com/settings/api-keys
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
	FString DomainKey;
	// Development Mode is only used for legacy API keys and signifies if the API Key is for the Stage or the Live environment (true = Stage, false = Live)
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker", Meta = (EditCondition = "IsLegacyKey"), Meta = (DisplayName = "Development Mode"))
    bool OnDevelopmentMode;
	[[deprecated("The Platform property has been deprecated, please use the appropriate Start Session method for your needs instead")]]
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "LootLocker")
    ELootLockerPlatformType Platform;
private:
	UPROPERTY(Config, VisibleInstanceOnly, Meta = (EditCondition = "1 == 0", EditConditionHides), Transient)
    bool IsLegacyKey;
};


