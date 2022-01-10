
#ifndef __GFXCANVAS_H__
#define    __GFXCANVAS_H__

#include "OGLCanvas.h"
#include "General/ListenerAnnouncer.h"

// Enumeration for view types
enum {
    GFXVIEW_DEFAULT,
    GFXVIEW_CENTERED,
    GFXVIEW_SPRITE,
    GFXVIEW_HUD,
    GFXVIEW_TILED,
};


class SImage;


class SBrush;


class GLTexture;


class Translation;


class GfxCanvas : public OGLCanvas, Listener {
private:
    SImage *image;
    int view_type;    // 0=default, 1=centered, 2=sprite offsets, 3=hud offsets, 4=tiled
    double scale;
    fpoint2_t offset;        // panning offsets (not image offsets)
    GLTexture *tex_image;
    bool update_texture;
    bool image_hilight;
    bool image_split;
    bool allow_drag;
    bool allow_scroll;
    point2_t drag_pos;
    point2_t drag_origin;
    point2_t mouse_prev;
    int editing_mode;    // 0=drag offsets (default), 1: paint, 2: erase, 3: translate
    rgba_t paint_colour;    // the colour to apply to pixels in editing mode 1
    Translation *translation;    // the translation to apply to pixels in editing mode 3
    bool drawing;        // true if a drawing operation is ongoing
    bool *drawing_mask;    // keeps track of which pixels were already modified in this pass
    SBrush *brush;            // the brush used to paint the image
    point2_t cursor_pos;        // position of cursor, relative to image
    point2_t prev_pos;        // previous position of cursor
    GLTexture *tex_brush;        // preview the effect of the brush

public:
    GfxCanvas(wxWindow *parent, int id);


    ~GfxCanvas();


    SImage *getImage() { return image; }


    void setViewType(int type) { view_type = type; }


    int getViewType() { return view_type; }


    void setScale(double scale);


    bool allowDrag() { return allow_drag; }


    void allowDrag(bool allow) { allow_drag = allow; }


    bool allowScroll() { return allow_scroll; }


    void allowScroll(bool allow) { allow_scroll = allow; }


    void setPaintColour(const rgba_t &col) { paint_colour.set(col); }


    void setEditingMode(int mode) { editing_mode = mode; }


    void setTranslation(Translation *tr) { translation = tr; }


    void setBrush(SBrush *br) { brush = br; }


    SBrush *getBrush() { return brush; }


    rgba_t getPaintColour() { return paint_colour; }


    void draw();


    void drawImage();


    void drawOffsetLines();


    void updateImageTexture();


    void endOffsetDrag();


    void paintPixel(int x, int y);


    void brushCanvas(int x, int y);


    void pickColour(int x, int y);


    void generateBrushShadow();


    void zoomToFit(bool mag = true, float padding = 0.0f);


    void resetOffsets() { offset.x = offset.y = 0; }


    bool onImage(int x, int y);


    point2_t imageCoords(int x, int y);


    void onAnnouncement(Announcer *announcer, string event_name, MemChunk &event_data);


    // Events
    void onMouseLeftDown(wxMouseEvent &e);


    void onMouseRightDown(wxMouseEvent &e);


    void onMouseLeftUp(wxMouseEvent &e);


    void onMouseMovement(wxMouseEvent &e);


    void onMouseLeaving(wxMouseEvent &e);


    void onKeyDown(wxKeyEvent &e);
};


DECLARE_EVENT_TYPE(wxEVT_GFXCANVAS_OFFSET_CHANGED, -1)
DECLARE_EVENT_TYPE(wxEVT_GFXCANVAS_PIXELS_CHANGED, -1)
DECLARE_EVENT_TYPE(wxEVT_GFXCANVAS_COLOUR_PICKED, -1)

#endif //__GFXCANVAS_H__
