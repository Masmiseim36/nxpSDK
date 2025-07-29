/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

void setup_scr_screen(lv_ui *ui)
{
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    ui->rect = lv_obj_create(ui->screen);
    lv_obj_align(ui->rect, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui->rect, LV_PCT(40), LV_PCT(40));

    ui->label = lv_label_create(ui->rect);
    lv_label_set_text(ui->label, "Drag Me");
    lv_obj_center(ui->label);

    lv_obj_update_layout(ui->screen);
}
