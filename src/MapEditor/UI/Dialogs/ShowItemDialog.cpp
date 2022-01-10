
// ----------------------------------------------------------------------------
// SLADE - It's a Doom Editor
// Copyright(C) 2008 - 2017 Simon Judd
//
// Email:       sirjuddington@gmail.com
// Web:         https://slade.mancubus.net
// Filename:    ShowItemDialog.cpp
// Description: A dialog allowing the user to select a map object type
//              (line / thing / etc) and enter an index.Used for the
//              'Show Item...' menu item in the map editor
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110 - 1301, USA.
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
// Includes
//
// ----------------------------------------------------------------------------
#include "Main.h"
#include "ShowItemDialog.h"
#include "UI/WxUtils.h"


// ----------------------------------------------------------------------------
//
// ShowItemDialog Class Functions
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// ShowItemDialog::ShowItemDialog
//
// ShowItemDialog class constructor
// ----------------------------------------------------------------------------
ShowItemDialog::ShowItemDialog(wxWindow *parent) : wxDialog(parent, -1, "Show Item") {
    // Setup sizer
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    wxGridBagSizer *gb_sizer = new wxGridBagSizer(UI::pad(), UI::pad());
    sizer->Add(gb_sizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, UI::padLarge());

    // Object type
    string types[] = { "Vertex", "Line", "Side", "Sector", "Thing" };
    gb_sizer->Add(new wxStaticText(this, -1, "Type:"), wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    choice_type_ = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, 5, types);
    gb_sizer->Add(choice_type_, wxGBPosition(0, 1), wxDefaultSpan, wxEXPAND);

    // Index
    gb_sizer->Add(new wxStaticText(this, -1, "Index:"), wxGBPosition(1, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
    text_index_ = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    gb_sizer->Add(text_index_, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);

    // Dialog buttons
    sizer->AddSpacer(UI::pad());
    sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, UI::padLarge());

    // Init layout
    gb_sizer->AddGrowableCol(1, 1);
    SetInitialSize(WxUtils::scaledSize(300, -1));
    CenterOnParent();
    Layout();
    text_index_->SetFocus();
    text_index_->SetFocusFromKbd();
}


// ----------------------------------------------------------------------------
// ShowItemDialog::getType
//
// Returns the selected object type
// ----------------------------------------------------------------------------
int ShowItemDialog::getType() const {
    return choice_type_->GetSelection() + 1;
}


// ----------------------------------------------------------------------------
// ShowItemDialog::getIndex
//
// Returns the entered index, or -1 if invalid
// ----------------------------------------------------------------------------
int ShowItemDialog::getIndex() const {
    long value;
    if (text_index_->GetValue().ToLong(&value))
        return value;
    else
        return -1;
}


// ----------------------------------------------------------------------------
// ShowItemDialog::setType
//
// Sets the object type dropdown to [type]
// ----------------------------------------------------------------------------
void ShowItemDialog::setType(int type) const {
    choice_type_->Select(type - 1);
}
