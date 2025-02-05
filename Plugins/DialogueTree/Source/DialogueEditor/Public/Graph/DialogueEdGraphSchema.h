
#pragma once

#include "EdGraph/EdGraphSchema.h"

#include "DialogueEdGraphSchema.generated.h"

struct FDialogueSchemaAction_NewNode;
class UDialogueTransition;
class UDialogueSpeakerSocket;
class UGraphNodeDialogueBase;

struct FConnectionArgs
{
	const UEdGraphPin* PinA;
	const UEdGraphPin* PinB;
	const UGraphNodeDialogueBase* NodeA;
	const UGraphNodeDialogueBase* NodeB;
};

UCLASS()
class DIALOGUEEDITOR_API UDialogueEdGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
public:
	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor,
		const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
	virtual void OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB, const FVector2D& GraphPosition) const override;
	// Graph right click
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	// Node or Pin right click
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;

	//static TSharedPtr<FDialogueAssetEditor> GetEditor(const UEdGraph* InGraph);
	
private:
	void GetPinContextMenu(UToolMenu* Menu, UEdGraphPin* Pin, FText PinText) const;

	void GetNodeContextMenu(UToolMenu* Menu) const;
	void GetNodeMenuActions(FGraphContextMenuBuilder& ContextMenuBuilder) const;

	void GetSpeechNodeMenuActions(FGraphContextMenuBuilder& ContextMenuBuilder) const;
	TSharedPtr<FDialogueSchemaAction_NewNode> MakeCreateSpeechNodeAction(UDialogueSpeakerSocket* SpeakerSocket, TSubclassOf<UDialogueTransition> TransitionType, UObject* Outer) const;

	void CheckForDisallowedConnection(const FConnectionArgs& InArgs, FPinConnectionResponse& OutResponse) const;
	bool NodeIsDirectParent(const UGraphNodeDialogueBase* NodeA, const UGraphNodeDialogueBase* NodeB) const;
	bool ConnectionHasConnectionLimit(const FConnectionArgs& InArgs, FPinConnectionResponse& OutResponse) const;

private:
	static int32 CurrentCacheRefreshID;
};
