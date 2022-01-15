/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "lvgl/lvgl.h"
#include "gui_guider.h"

void setup_ui(lv_ui *ui)
{
    ui->btn = lv_btn_create(lv_scr_act());

    ui->label = lv_label_create(ui->btn);
    lv_label_set_text(ui->label, "Button");
}
