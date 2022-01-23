#pragma once

#include "common.h"

#define PAD_INNER 2
#define PAD_OUTER 1

class SAction;


class SToolBarButton : public wxControl {
public:
    SToolBarButton(wxWindow *parent, string action, string icon = "", bool show_name = false);


    SToolBarButton(
        wxWindow *parent,
        const string &action_id,
        const string &action_name,
        const string &icon,
        const string &help_text,
        bool show_name = false
    );


    ~SToolBarButton() {}


    void setIcon(string icon);


    enum {
        STATE_NORMAL,
        STATE_MOUSEOVER,
        STATE_MOUSEDOWN,
    };


    static int pixelHeight();


private:
    SAction *action_ = nullptr;
    wxBitmap icon_;
    int state_ = STATE_NORMAL;
    bool show_name_ = false;

    // For non-SAction buttons
    string action_id_;
    string action_name_;
    string help_text_;

    // Layout
    int top_margin = 4;
    int pad_outer_;
    int pad_inner_;
    int icon_size_ ;
    int text_width_ ;


    void sendClickedEvent();


    // Events
    void onPaint(wxPaintEvent &e);


    void onMouseEvent(wxMouseEvent &e);


    void onFocus(wxFocusEvent &e);


    void onEraseBackground(wxEraseEvent &e);
};


wxDECLARE_EVENT(wxEVT_STOOLBAR_BUTTON_CLICKED, wxCommandEvent);
