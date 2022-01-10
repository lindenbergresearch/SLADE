
// ----------------------------------------------------------------------------
// SLADE - It's a Doom Editor
// Copyright(C) 2008 - 2017 Simon Judd
//
// Email:       sirjuddington@gmail.com
// Web:         http://slade.mancubus.net
// Filename:    SToolBarButton.cpp
// Description: SToolBarButton class - a simple toolbar button for use on an
//              SToolBar, can be displayed as an icon or icon + text
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
#include "SToolBarButton.h"
#include "Graphics/Icons.h"
#include "OpenGL/Drawing.h"
#include "MainEditor/UI/MainWindow.h"
#include "UI/WxUtils.h"


// ----------------------------------------------------------------------------
//
// Variables
//
// ----------------------------------------------------------------------------
CVAR(Bool, toolbar_button_flat, true, CVAR_SAVE)
wxDEFINE_EVENT(wxEVT_STOOLBAR_BUTTON_CLICKED, wxCommandEvent);


// ----------------------------------------------------------------------------
//
// External Variables
//
// ----------------------------------------------------------------------------
EXTERN_CVAR(Int, toolbar_size);


// ----------------------------------------------------------------------------
//
// SToolBarButton Class Functions
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// SToolBarButton::SToolBarButton
//
// SToolBarButton class constructor
// ----------------------------------------------------------------------------
SToolBarButton::SToolBarButton(wxWindow *parent, string action, string icon, bool show_name) :
    wxControl(parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE, wxDefaultValidator, "stbutton"),
    action_{ SAction::fromId(action) },
    show_name_{ show_name },
    action_id_{ action_->getId() },
    action_name_{ action_->getText() },
    help_text_{ action_->getHelpText() },
    pad_outer_{ UI::scalePx(1) },
    pad_inner_{ UI::scalePx(2) },
    icon_size_{ UI::scalePx(toolbar_size) } {
    // Determine width of name text if shown
    if (show_name) {
        string name = action_->getText();
        name.Replace("&", "");
        text_width_ = GetTextExtent(name).GetWidth() + pad_inner_ * 2;
    }

    // Set size
    int height = pad_outer_ * 2 + pad_inner_ * 2 + icon_size_;
    int width = height + text_width_;
    SetSizeHints(width, height, width, height);
    SetMinSize(wxSize(width, height));
    SetSize(width, height);

    // Load icon
    if (icon.IsEmpty())
        icon_ = Icons::getIcon(Icons::GENERAL, action_->getIconName(), icon_size_ > 16);
    else
        icon_ = Icons::getIcon(Icons::GENERAL, icon, icon_size_ > 16);

    // Add shortcut to help text if it exists
    string sc = action_->getShortcutText();
    if (!sc.IsEmpty())
        help_text_ += S_FMT(" (Shortcut: %s)", sc);

    // Set tooltip
    if (!show_name) {
        string tip = action_->getText();
        tip.Replace("&", "");
        if (!sc.IsEmpty()) tip += S_FMT(" (Shortcut: %s)", sc);
        SetToolTip(tip);
    } else if (!sc.IsEmpty())
        SetToolTip(S_FMT("Shortcut: %s", sc));

    // Bind events
    Bind(wxEVT_PAINT, &SToolBarButton::onPaint, this);
    Bind(wxEVT_ENTER_WINDOW, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEAVE_WINDOW, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEFT_DOWN, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEFT_UP, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_KILL_FOCUS, &SToolBarButton::onFocus, this);
    Bind(wxEVT_LEFT_DCLICK, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_ERASE_BACKGROUND, &SToolBarButton::onEraseBackground, this);
}


// ----------------------------------------------------------------------------
// SToolBarButton::SToolBarButton
//
// SToolBarButton class constructor
// ----------------------------------------------------------------------------
SToolBarButton::SToolBarButton(
    wxWindow *parent,
    const string &action_id,
    const string &action_name,
    const string &icon,
    const string &help_text,
    bool show_name
) : wxControl{ parent, -1, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE, wxDefaultValidator, "stbutton" },
    show_name_{ show_name },
    action_id_{ action_id },
    action_name_{ action_name },
    help_text_{ help_text },
    pad_outer_{ UI::scalePx(1) },
    pad_inner_{ UI::scalePx(2) },
    icon_size_{ UI::scalePx(toolbar_size) } {
    // Determine width of name text if shown
    text_width_ = show_name ? GetTextExtent(action_name).GetWidth() + pad_inner_ * 2 : 0;

    // Set size
    int height = pad_outer_ * 2 + pad_inner_ * 2 + icon_size_;
    int width = height + text_width_;
    SetSizeHints(width, height, width, height);
    SetMinSize(wxSize(width, height));
    SetSize(width, height);

    // Load icon
    icon_ = Icons::getIcon(Icons::GENERAL, icon, icon_size_ > 16);

    // Set tooltip
    if (!show_name)
        SetToolTip(action_name);

    // Bind events
    Bind(wxEVT_PAINT, &SToolBarButton::onPaint, this);
    Bind(wxEVT_ENTER_WINDOW, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEAVE_WINDOW, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEFT_DOWN, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_LEFT_UP, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_KILL_FOCUS, &SToolBarButton::onFocus, this);
    Bind(wxEVT_LEFT_DCLICK, &SToolBarButton::onMouseEvent, this);
    Bind(wxEVT_ERASE_BACKGROUND, &SToolBarButton::onEraseBackground, this);
}


// ----------------------------------------------------------------------------
// SToolBarButton::setIcon
//
// Allows to dynamically change the button's icon
// ----------------------------------------------------------------------------
void SToolBarButton::setIcon(string icon) {
    if (!icon.IsEmpty())
        icon_ = Icons::getIcon(Icons::GENERAL, icon, icon_size_ > 16);
}


// ----------------------------------------------------------------------------
// SToolBarButton::pixelHeight
//
// Returns the pixel height of all SToolBarButtons
// ----------------------------------------------------------------------------
int SToolBarButton::pixelHeight() {
    return UI::scalePx(toolbar_size + 8);
}


// ----------------------------------------------------------------------------
// SToolBarButton::sendClickedEvent
//
// Sends a button clicked event
// ----------------------------------------------------------------------------
void SToolBarButton::sendClickedEvent() {
    // Generate event
    wxCommandEvent ev(wxEVT_STOOLBAR_BUTTON_CLICKED, GetId());
    ev.SetEventObject(this);
    ev.SetString(action_id_);
    ProcessWindowEvent(ev);
}


// ----------------------------------------------------------------------------
//
// SToolBarButton Class Events
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// SToolBarButton::onPaint
//
// Called when the button needs to be (re)drawn
// ----------------------------------------------------------------------------
void SToolBarButton::onPaint(wxPaintEvent &e) {
    wxPaintDC dc(this);

    // Get system colours needed
    wxColour col_background = GetBackgroundColour();
    wxColour col_hilight = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);

    // Draw background
    dc.SetBackground(wxBrush(col_background));
    dc.Clear();

    // Create graphics context
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if (!gc)
        return;

    // Get width of name text if shown
    int name_height = 0;
    string name = action_name_;
    if (show_name_) {
        name.Replace("&", "");
        wxSize name_size = GetTextExtent(name);
        name_height = name_size.y;
    }

    int height = icon_size_ + pad_inner_ * 2;
    int width = height + text_width_;
    auto scale_px = UI::scaleFactor();

    // Draw toggled border/background
    if (action_ && action_->isChecked()) {
        // Use greyscale version of hilight colour
        uint8_t r = col_hilight.Red();
        uint8_t g = col_hilight.Green();
        uint8_t b = col_hilight.Blue();
        wxColour::MakeGrey(&r, &g, &b);
        wxColour col_toggle(r, g, b, 255);
        wxColour col_trans(r, g, b, 150);

        if (toolbar_button_flat) {
            // Draw border
            col_trans.Set(COLWX(col_trans), 80);
            gc->SetBrush(col_trans);
            gc->SetPen(wxPen(Drawing::darkColour(col_toggle, 5.0f), scale_px));
            gc->DrawRectangle(pad_outer_, pad_outer_, width, height);
        } else {
            // Draw border
            col_trans.Set(COLWX(col_trans), 80);
            gc->SetBrush(col_trans);
            gc->SetPen(wxPen(col_hilight, scale_px));
            gc->DrawRoundedRectangle(pad_outer_, pad_outer_, width, height, 2 * scale_px);
        }
    }

    // Draw border on mouseover
    if (state_ == STATE_MOUSEOVER || state_ == STATE_MOUSEDOWN) {
        // Determine transparency level
        int trans = 160;
        if (state_ == STATE_MOUSEDOWN)
            trans = 200;

        // Create semitransparent hilight colour
        wxColour col_trans(COLWX(col_hilight), trans);

        if (toolbar_button_flat) {
            // Draw border
            col_trans.Set(COLWX(col_trans), 80);
            gc->SetBrush(col_trans);
            gc->SetPen(wxPen(col_hilight, scale_px));
            gc->DrawRectangle(pad_outer_, pad_outer_, width, height);
        } else {
            // Draw border
            col_trans.Set(COLWX(col_trans), 80);
            gc->SetBrush(col_trans);
            gc->SetPen(wxPen(col_hilight));
            gc->DrawRoundedRectangle(pad_outer_, pad_outer_, width, height, 2 * scale_px);
        }
    }

    if (icon_.IsOk()) {
        // Draw disabled icon if disabled
        if (!IsEnabled()) {
            // Determine toolbar background brightness
            uint8_t r, g, b;
            r = col_background.Red();
            g = col_background.Green();
            b = col_background.Blue();
            wxColor::MakeGrey(&r, &g, &b);

            // Draw disabled icon
            gc->DrawBitmap(
                icon_.ConvertToDisabled(r),
                pad_outer_ + pad_inner_,
                pad_outer_ + pad_inner_,
                icon_size_,
                icon_size_
            );
        }

            // Otherwise draw normal icon
        else
            gc->DrawBitmap(
                icon_,
                pad_outer_ + pad_inner_,
                pad_outer_ + pad_inner_,
                icon_size_,
                icon_size_
            );
    }

    if (show_name_) {
        int top = ((double) GetSize().y * 0.5) - ((double) name_height * 0.5);
        int left = pad_outer_ * 2 + pad_inner_ * 2 + icon_size_;
        dc.DrawText(name, left, top);
    }

    delete gc;
}


// ----------------------------------------------------------------------------
// SToolBarButton::onMouseEvent
//
// Called when a mouse event happens within the control
// ----------------------------------------------------------------------------
void SToolBarButton::onMouseEvent(wxMouseEvent &e) {
    wxFrame *parent_window = (wxFrame *) wxGetTopLevelParent(this);

    // Mouse enter
    if (e.GetEventType() == wxEVT_ENTER_WINDOW) {
        // Set state to mouseover
        state_ = STATE_MOUSEOVER;

        // Set status bar help text
        if (parent_window && parent_window->GetStatusBar())
            parent_window->SetStatusText(help_text_);
    }

    // Mouse leave
    if (e.GetEventType() == wxEVT_LEAVE_WINDOW) {
        // Set state to normal
        state_ = STATE_NORMAL;

        // Clear status bar help text
        if (parent_window && parent_window->GetStatusBar())
            parent_window->SetStatusText("");
    }

    // Left button down
    if (e.GetEventType() == wxEVT_LEFT_DOWN || e.GetEventType() == wxEVT_LEFT_DCLICK) {
        state_ = STATE_MOUSEDOWN;

        if (action_) {
            if (action_->isRadio())
                GetParent()->Refresh();
            SActionHandler::doAction(action_->getId());
        } else
            sendClickedEvent();
    }

    // Left button up
    if (e.GetEventType() == wxEVT_LEFT_UP) {
        state_ = STATE_MOUSEOVER;

        // Clear status bar help text
        if (parent_window && parent_window->GetStatusBar())
            parent_window->SetStatusText("");
    }

    Refresh();

    //e.Skip();
}


// ----------------------------------------------------------------------------
// SToolBarButton::onFocus
//
// Called when the control gains or loses focus
// ----------------------------------------------------------------------------
void SToolBarButton::onFocus(wxFocusEvent &e) {
    // Redraw
    state_ = STATE_NORMAL;
    Update();
    Refresh();

    e.Skip();
}


// ----------------------------------------------------------------------------
// SToolBarButton::onEraseBackground
//
// Called when the background needs erasing
// ----------------------------------------------------------------------------
void SToolBarButton::onEraseBackground(wxEraseEvent &e) {
}
