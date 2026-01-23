// Amigo Luis all rights reserved


#include "Widgets/Components/FrontEndCommonRotator.h"

#include "CommonTextBlock.h"

void UFrontEndCommonRotator::SetSelectedItemByText(const FText& InTextOption)
{
	const auto IndexOfText = TextLabels.IndexOfByPredicate([&InTextOption](const FText& TextOption)
	{
		return TextOption.EqualTo(InTextOption);
	});
	if (TextLabels.IsValidIndex(IndexOfText))
	{
		SetSelectedItem(IndexOfText);
	}
	else
	{
		MyText->SetText(InTextOption);
	}
}
