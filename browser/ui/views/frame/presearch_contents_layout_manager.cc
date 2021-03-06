/* Copyright (c) 2021 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "presearch/browser/ui/views/frame/presearch_contents_layout_manager.h"

#include "ui/views/view.h"

PresearchContentsLayoutManager::PresearchContentsLayoutManager(
    views::View* sidebar_container_view,
    views::View* contents_container_view)
    : sidebar_container_view_(sidebar_container_view),
      contents_container_view_(contents_container_view) {
  DCHECK(sidebar_container_view_);
  DCHECK(contents_container_view_);
}

PresearchContentsLayoutManager::~PresearchContentsLayoutManager() = default;

void PresearchContentsLayoutManager::Layout(views::View* host) {
  DCHECK(host_ == host);

  int height = host->height();
  int width = host->width();

  const int sidebar_width = sidebar_container_view_->GetPreferredSize().width();
  const gfx::Rect sidebar_bounds(0, 0, sidebar_width, height);
  gfx::Rect contents_bounds(sidebar_width, 0, width - sidebar_width, height);

  sidebar_container_view_->SetBoundsRect(
      host_->GetMirroredRect(sidebar_bounds));
  contents_container_view_->SetBoundsRect(
      host_->GetMirroredRect(contents_bounds));
}

gfx::Size PresearchContentsLayoutManager::GetPreferredSize(
    const views::View* host) const {
  return gfx::Size();
}

void PresearchContentsLayoutManager::Installed(views::View* host) {
  DCHECK(!host_);
  host_ = host;
}
