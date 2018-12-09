#pragma once

#include "OpenGL/GLTexture.h"
#include "UI/Canvas/OGLCanvas.h"

class SImage;
class NumberTextCtrl;
class GLTexture;

class CropCanvas : public OGLCanvas
{
public:
	CropCanvas(wxWindow* parent, SImage* image, Palette* palette);

	const Recti& cropRect() const { return crop_rect_; }
	void         setCropRect(Recti& rect) { crop_rect_.set(rect); }

	void draw() override;

private:
	std::unique_ptr<GLTexture> texture_;
	Recti                      crop_rect_;
};

class GfxCropDialog : public wxDialog
{
public:
	GfxCropDialog(wxWindow* parent, SImage* image, Palette* palette);
	~GfxCropDialog() = default;

	const Recti& cropRect() const { return crop_rect_; }
	void         updatePreview();

private:
	CropCanvas*     canvas_preview_ = nullptr;
	NumberTextCtrl* text_left_      = nullptr;
	NumberTextCtrl* text_top_       = nullptr;
	NumberTextCtrl* text_right_     = nullptr;
	NumberTextCtrl* text_bottom_    = nullptr;
	wxRadioButton*  rb_absolute_    = nullptr;
	wxRadioButton*  rb_relative_    = nullptr;

	int   max_width_ = 0;
	int   max_height_ = 0;
	Recti crop_rect_;

	void bindEvents();

	void updateValues() const;
	void setLeft();
	void setTop();
	void setRight();
	void setBottom();

	// Events
	void onTextEnter(wxCommandEvent& e);
};
