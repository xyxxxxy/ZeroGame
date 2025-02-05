
#include "UI/DialogueTextBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Styling/SlateStyle.h"

TSharedRef<SWidget> UDialogueTextBlock::RebuildWidget()
{
	UpdateStyleData();

	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;
	CreateDecorators(CreatedDecorators);

	TextLayoutMarshaller = FRichTextLayoutMarshaller::Create(CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators, StyleInstance.Get());
	
	MyRichTextBlock =
			SNew(SRichTextBlock)
			.TextStyle(bOverrideDefaultStyle ? &GetCurrentDefaultTextStyle() : &GetDefaultTextStyle())
			.Marshaller(TextLayoutMarshaller)
			.CreateSlateTextLayout(
				FCreateSlateTextLayout::CreateWeakLambda(this,[this](SWidget* InOwner,const FTextBlockStyle& InDefaultTextStyle) mutable 
				{
					TextLayout = FSlateTextLayout::Create(InOwner,InDefaultTextStyle);
					return StaticCastSharedPtr<FSlateTextLayout>(TextLayout).ToSharedRef();
				})
			);
	
	return MyRichTextBlock.ToSharedRef();
}
