
#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "DialogueTextBlock.generated.h"

class FRichTextLayoutMarshaller;
class FSlateTextLayout;

UCLASS()
class UDialogueTextBlock : public URichTextBlock
{
	GENERATED_BODY()
public:
	FORCEINLINE TSharedPtr<FSlateTextLayout> GetTextLayout() const{return TextLayout;}
	FORCEINLINE TSharedPtr<FRichTextLayoutMarshaller> GetTextLayoutMarshaller() const {return TextLayoutMarshaller;}
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	
private:
	TSharedPtr<FSlateTextLayout> TextLayout;
	TSharedPtr<FRichTextLayoutMarshaller> TextLayoutMarshaller;
};
