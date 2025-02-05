
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

class SComboButton;
class SScrollBox;

DECLARE_DELEGATE_RetVal_OneParam(FName, FGetObjectName, UObject*);
DECLARE_DELEGATE_OneParam(FOnPickerSelect, UObject*);

class SDialogueObjectPicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueObjectPicker) {}
	SLATE_ARGUMENT(TSharedPtr<SComboButton>, ParentButton)
	SLATE_ARGUMENT(TArray<UObject*>, Collection)
	SLATE_ARGUMENT(FGetObjectName, NameGetter)
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs);

private:
	void InitCollection(const TArray<UObject*>& InCollection, FGetObjectName NameGetter);

	/**
	* Sets up the array of filtered names. 
	*/
	void InitFilteredNames();

	void BuildPickerWidget();
	void AddSearchWidget(TSharedRef<SVerticalBox> MainBox);
	void AddOptionsWidget(TSharedRef<SVerticalBox> MainBox);

	void RefreshOptionsWidget();

	/**
	* Filters the collection based on the provided search text. 
	* 
	* @param SearchText - FText&, the filter text. 
	*/
	void FilterCollection(const FText& SearchText);

	TSharedRef<SButton> CreateOptionButton(FName& InName);
	FReply BroadcastSelectedOption(FName SelectionName);

	FSlateFontInfo GetFontStyle() const;

public:
	void ClearSearchText();
	void RefocusSearchBox();

private:
	/**
	* Focuses the search box after constructing this object so that
	* the user can begin typing immediately. Borrowed almost 
	* entirely from SAssetPicker.cpp.
	* 
	* @param InCurrentTime - double, the current time.
	* @param InDeltaTime = float, the current delta time.
	* @return EActiveTimerReturnType - timer return. 
	*/
	EActiveTimerReturnType SetFocusPostConstruct(double InCurrentTime, float InDeltaTime);	

public:
	FOnPickerSelect OnSelectOption;

private: 
	TMap<FName, UObject*> Collection;

	TArray<FName> FilteredNames;

	/** The combo button that opens this picker */
	TSharedPtr<SComboButton> ParentButton;

	TSharedPtr<SSearchBox> SearchBox;
	TSharedPtr<SScrollBox> OptionsBox;

	const float MAX_PICKER_HEIGHT = 150.f;
	const float OPTIONS_BOX_Y_PADDING = 20.f;
	const float OPTION_PADDING = 2.f;
	const ETextJustify::Type TEXT_JUSTIFY = ETextJustify::Center;
};