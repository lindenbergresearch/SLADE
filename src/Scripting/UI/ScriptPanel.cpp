
#include "Main.h"
#include "ScriptPanel.h"
#include "Scripting/ScriptManager.h"
#include "TextEditor/UI/TextEditorCtrl.h"

ScriptPanel::ScriptPanel(wxWindow* parent, ScriptManager::Script* script) :
	wxPanel(parent, -1),
	script_{ script }
{
	wxPanel::SetName("script");

	auto sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	
	text_editor_ = new TextEditorCtrl(this, -1);
	text_editor_->setLanguage(TextLanguage::fromId("sladescript"));
	if (script_)
		text_editor_->SetText(script_->text);
	sizer->Add(text_editor_, 1, wxEXPAND | wxALL, 10);
}

string ScriptPanel::currentText() const
{
	return text_editor_->GetText();
}
