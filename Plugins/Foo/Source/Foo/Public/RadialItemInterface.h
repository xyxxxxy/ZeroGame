#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RadialItemInterface.generated.h"

/*
此类无需修改。
用于反射系统可见性的空白类。
使用UINTERFACE宏。
继承自UInterface。
*/
UINTERFACE(MinimalAPI, Blueprintable)
class URadialItemInterface : public UInterface
{
	GENERATED_BODY()
};

/* 实际接口声明。 */
class IRadialItemInterface
{
	GENERATED_BODY()

	// 将接口函数添加到此类。此类将被继承以实现此接口。
public:
	// 在此处添加接口函数声明
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RadialWheelMenu")
	bool OnSelected();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RadialWheelMenu")
	bool OnDeselected();
};