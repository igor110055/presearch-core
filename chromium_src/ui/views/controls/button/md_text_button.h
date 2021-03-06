// Copyright (c) 2019 The Presearch Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PRESEARCH_CHROMIUM_SRC_UI_VIEWS_CONTROLS_BUTTON_MD_TEXT_BUTTON_H_
#define PRESEARCH_CHROMIUM_SRC_UI_VIEWS_CONTROLS_BUTTON_MD_TEXT_BUTTON_H_

// Make all private members into protected instead of using `friend`. This is
// because we are renaming the class into MdTextButtonBase and making our own
// MdTextButton, so the friend statement would get renamed too.
#define PRESEARCH_MD_TEXT_BUTTON_H_ \
 protected:
// PRESEARCH_MD_TEXT_BUTTON_H_

// Rename MdTextButton to MdTextButtonBase
#define MdTextButton MdTextButtonBase
#include "../../../../../../ui/views/controls/button/md_text_button.h"
#undef MdTextButton
#undef PRESEARCH_MD_TEXT_BUTTON_H_

namespace views {

// Make visual changes to MdTextButton in line with Presearch visual style:
//  - More rounded rectangle (for regular border, focus ring and ink drop)
//  - Different hover text and boder color for non-prominent button
//  - Differenet hover bg color for prominent background
//  - No shadow for prominent background
class VIEWS_EXPORT MdTextButton : public MdTextButtonBase {
 public:
  explicit MdTextButton(PressedCallback callback = PressedCallback(),
                        const base::string16& text = base::string16(),
                        int button_context = style::CONTEXT_BUTTON_MD);
  ~MdTextButton() override;

  // InkDrop
  std::unique_ptr<InkDrop> CreateInkDrop() override;
  std::unique_ptr<views::InkDropHighlight> CreateInkDropHighlight()
      const override;

  SkPath GetHighlightPath() const;

 protected:
  void OnPaintBackground(gfx::Canvas* canvas) override;

 private:
  void UpdateColors() override;
  DISALLOW_COPY_AND_ASSIGN(MdTextButton);
};

}  // namespace views

DEFINE_VIEW_BUILDER(VIEWS_EXPORT, MdTextButton)

#endif  // PRESEARCH_CHROMIUM_SRC_UI_VIEWS_CONTROLS_BUTTON_MD_TEXT_BUTTON_H_
