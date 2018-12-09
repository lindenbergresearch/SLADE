#pragma once

#include "UI/Controls/STabCtrl.h"

class Archive;

class ThingTypeReplacePanel : public wxPanel
{
public:
	ThingTypeReplacePanel(wxWindow* parent);
	~ThingTypeReplacePanel() = default;

	void doReplace(Archive* archive) const;

private:
	wxSpinCtrl* spin_from_ = nullptr;
	wxSpinCtrl* spin_to_   = nullptr;
};

class SpecialReplacePanel : public wxPanel
{
public:
	SpecialReplacePanel(wxWindow* parent);
	~SpecialReplacePanel() = default;

	void doReplace(Archive* archive) const;

private:
	wxSpinCtrl* spin_from_         = nullptr;
	wxSpinCtrl* spin_to_           = nullptr;
	wxCheckBox* cb_line_specials_  = nullptr;
	wxCheckBox* cb_thing_specials_ = nullptr;
	wxSpinCtrl* spin_args_from_[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
	wxSpinCtrl* spin_args_to_[5]   = { nullptr, nullptr, nullptr, nullptr, nullptr };
	wxCheckBox* cb_args_[5]        = { nullptr, nullptr, nullptr, nullptr, nullptr };
};

class TextureReplacePanel : public wxPanel
{
public:
	TextureReplacePanel(wxWindow* parent);
	~TextureReplacePanel() = default;

	void doReplace(Archive* archive) const;

private:
	wxTextCtrl* text_from_  = nullptr;
	wxTextCtrl* text_to_    = nullptr;
	wxCheckBox* cb_floor_   = nullptr;
	wxCheckBox* cb_ceiling_ = nullptr;
	wxCheckBox* cb_lower_   = nullptr;
	wxCheckBox* cb_middle_  = nullptr;
	wxCheckBox* cb_upper_   = nullptr;
};

class MapReplaceDialog : public wxDialog
{
public:
	MapReplaceDialog(wxWindow* parent = nullptr, Archive* archive = nullptr);
	~MapReplaceDialog() = default;

private:
	Archive* archive_ = nullptr;

	TabControl*            stc_tabs_      = nullptr;
	ThingTypeReplacePanel* panel_thing_   = nullptr;
	SpecialReplacePanel*   panel_special_ = nullptr;
	TextureReplacePanel*   panel_texture_ = nullptr;
	wxButton*              btn_replace_   = nullptr;
	wxButton*              btn_done_      = nullptr;

	void onBtnReplace(wxCommandEvent& e);
};
