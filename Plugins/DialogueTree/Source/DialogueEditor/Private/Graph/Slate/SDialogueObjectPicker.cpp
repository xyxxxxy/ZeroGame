
#include "Graph/Slate/SDialogueObjectPicker.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "SDialogueObjectPicker"

void SDialogueObjectPicker::Construct(const FArguments& InArgs)
{
	check(InArgs._Collection.IsEmpty() == false && InArgs._ParentButton.IsValid() && InArgs._NameGetter.IsBound());

	//Set up the collection 
	InitCollection(InArgs._Collection, InArgs._NameGetter);

	ParentButton = InArgs._ParentButton;
	check(ParentButton.IsValid());

	InitFilteredNames();

	BuildPickerWidget();

	RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SDialogueObjectPicker::SetFocusPostConstruct));
}

void SDialogueObjectPicker::InitCollection(const TArray<UObject*>& InCollection, FGetObjectName NameGetter)
{
	Collection.Empty();
	bool bNameGetterBound = NameGetter.IsBound();

	for (UObject* Obj : InCollection)
	{
		FName ObjName = bNameGetterBound ? NameGetter.Execute(Obj) : Obj->GetFName();

		Collection.Add(ObjName, Obj);
	}
}

void SDialogueObjectPicker::InitFilteredNames()
{
	TSet<FName> NamesSet;
	Collection.GetKeys(NamesSet);
	FilteredNames = NamesSet.Array();

	//Sort names alphabetically 字母排序
	FilteredNames.Sort(
		[](FName Name1, FName Name2)
		{
			return Name1.LexicalLess(Name2);
		}
	);
}

void SDialogueObjectPicker::BuildPickerWidget()
{
	TSharedRef<SVerticalBox> MainBox = SNew(SVerticalBox);
	ChildSlot
	[
		MainBox
	];

	AddSearchWidget(MainBox);
	AddOptionsWidget(MainBox);
}

void SDialogueObjectPicker::AddSearchWidget(TSharedRef<SVerticalBox> MainBox)
{
	//Delegate to call for filtering when text changes {FOnTextChanged}
	FOnTextChanged OnTextChanged;
	OnTextChanged.BindSP(this, &SDialogueObjectPicker::FilterCollection);

	MainBox->AddSlot()
	.AutoHeight()
	[
		SAssignNew(SearchBox, SSearchBox)
		.OnTextChanged(OnTextChanged)
	];
}

void SDialogueObjectPicker::AddOptionsWidget(TSharedRef<SVerticalBox> MainBox)
{
	MainBox->AddSlot()
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::Get().GetBrush("Brushes.Panel"))
		[
			SNew(SBox)
			.MaxDesiredHeight(MAX_PICKER_HEIGHT)
			[
				SAssignNew(OptionsBox, SScrollBox)
			]
		]
	];

	RefreshOptionsWidget();
}

void SDialogueObjectPicker::RefreshOptionsWidget()
{
	check(OptionsBox.IsValid());

	OptionsBox->ClearChildren();

	if (FilteredNames.IsEmpty())
	{
		OptionsBox->AddSlot()
		.Padding(0.f, OPTIONS_BOX_Y_PADDING)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("EmptyOptionsText", "No valid items"))
			.Font(GetFontStyle())
			.Justification(TEXT_JUSTIFY)
		];

		return;
	}

	OptionsBox->AddSlot()
	[
		SNew(SSpacer)
		.Size(FVector2D(OPTION_PADDING))
	];

	//Fill the box with option buttons 
	for (FName OptionName : FilteredNames)
	{
		OptionsBox->AddSlot()
		.Padding(OPTION_PADDING, OPTION_PADDING / 2.f)
		[
			CreateOptionButton(OptionName)
		];
	}
}

void SDialogueObjectPicker::FilterCollection(const FText& SearchText)
{
	FilteredNames.Empty();

	if (SearchText.IsEmptyOrWhitespace())
	{
		Collection.GetKeys(FilteredNames);
		RefreshOptionsWidget();
		return;
	}

	FString SearchString = SearchText.ToString();
	SearchString.RemoveSpacesInline();

	TArray<FName> Options;
	Collection.GetKeys(Options);

	for (FName Option : Options)
	{
		FString OptionString = Option.ToString();
		OptionString.RemoveSpacesInline();

		if (OptionString.Contains(SearchString))
		{
			FilteredNames.Add(Option);
		}
	}

	RefreshOptionsWidget();
}

FReply SDialogueObjectPicker::BroadcastSelectedOption(FName SelectionName)
{
	check(Collection.Contains(SelectionName));
	UObject* NewValue = Collection.FindChecked(SelectionName);

	OnSelectOption.ExecuteIfBound(NewValue);

	if (ParentButton.IsValid())
	{
		ParentButton->SetIsOpen(false);
	}

	return FReply::Handled();
}

FSlateFontInfo SDialogueObjectPicker::GetFontStyle() const
{
	return FAppStyle::GetFontStyle("PropertyWindow.NormalFont");
}

TSharedRef<SButton> SDialogueObjectPicker::CreateOptionButton(
	FName& InName) 
{
	//Behavior for clicking button {FOnClicked}
	FOnClicked OnButtonClick;
	OnButtonClick.BindSP(
		this,
		&SDialogueObjectPicker::BroadcastSelectedOption,
		InName
	);

	//Create button for option 
	return SNew(SButton)
		.Text(FText::FromName(InName))
		.OnClicked(OnButtonClick);
}

void SDialogueObjectPicker::ClearSearchText()
{
	if (SearchBox.IsValid())
	{
		SearchBox->SetText(FText());
	}
}

void SDialogueObjectPicker::RefocusSearchBox()
{
	RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SDialogueObjectPicker::SetFocusPostConstruct));
}

EActiveTimerReturnType SDialogueObjectPicker::SetFocusPostConstruct(
	double InCurrentTime, float InDeltaTime)
{
	if (SearchBox.IsValid())
	{
		FWidgetPath WidgetToFocusPath;
		FSlateApplication::Get().GeneratePathToWidgetUnchecked(
			SearchBox.ToSharedRef(), WidgetToFocusPath
		);
		FSlateApplication::Get().SetKeyboardFocus(
			WidgetToFocusPath, 
			EFocusCause::SetDirectly
		);
		WidgetToFocusPath.GetWindow()->SetWidgetToFocusOnActivate(SearchBox);

		return EActiveTimerReturnType::Stop;
	}

	return EActiveTimerReturnType::Continue;
}

#undef LOCTEXT_NAMESPACE