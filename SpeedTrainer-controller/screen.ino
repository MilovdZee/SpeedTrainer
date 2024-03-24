static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 10];

lv_obj_t *screen_main;

static lv_obj_t *keyboard = nullptr;

lv_obj_t *status_label = nullptr;

void setup_screen() {
  tft.begin();
  tft.setRotation(1);

  Settings settings = get_settings();
  tft.setBrightness(settings.brightness * 240 / 100 + 15);

  // Initialize lvgl library
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = SCREEN_WIDTH;
  disp_drv.ver_res = SCREEN_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  set_status_label_style();
  set_default_style();
  set_small_style();

  screen_main = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(screen_main, lv_color_black(), LV_PART_MAIN);
  lv_obj_add_style(screen_main, &style_default, 0);
  lv_scr_load(screen_main);
}

// /* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void set_status_label_style() {
  lv_style_init(&style_status_label);
  lv_style_set_radius(&style_status_label, 5);
  lv_style_set_bg_opa(&style_status_label, LV_OPA_COVER);
  lv_style_set_bg_color(&style_status_label, lv_palette_darken(LV_PALETTE_GREY, 4));
  lv_style_set_border_width(&style_status_label, 2);
  lv_style_set_border_color(&style_status_label, lv_palette_darken(LV_PALETTE_BLUE, 2));
  lv_style_set_pad_all(&style_status_label, 10);
  lv_style_set_text_font(&style_status_label, &lv_font_montserrat_22);
  lv_style_set_text_line_space(&style_status_label, 10);
  lv_style_set_text_align(&style_status_label, LV_TEXT_ALIGN_CENTER);
  lv_style_set_text_color(&style_status_label, lv_color_hex(0xffffff));
}

void set_default_style() {
  lv_style_init(&style_default);
  lv_style_set_text_font(&style_default, &lv_font_montserrat_22);
}

void set_small_style() {
  lv_style_init(&style_small);
  lv_style_set_text_font(&style_small, &lv_font_montserrat_14);
}

void show_status_label(const char *statusText, boolean add_close_btn) {
  char *title_text = nullptr;
  if (add_close_btn) title_text = "#ff0000 Alert#";
  status_label = lv_msgbox_create(NULL, title_text, statusText, NULL, add_close_btn);
  lv_obj_center(status_label);

  if (add_close_btn) {
    lv_obj_t *title = lv_msgbox_get_title(status_label);
    lv_label_set_recolor(title, true);
  }
  lv_obj_t *text = lv_msgbox_get_text(status_label);
  lv_label_set_recolor(text, true);
  lv_obj_add_style(text, &style_default, 0);
}

void hide_status_label() {
  if (status_label != nullptr) {
    lv_msgbox_close(status_label);
    status_label = nullptr;
  }
}

lv_obj_t *get_screen_main() {
  return screen_main;
}

static void ta_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *ta = lv_event_get_target(event);
  lv_obj_t *win = (lv_obj_t *)lv_event_get_user_data(event);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    if (!keyboard) {
      keyboard = lv_keyboard_create(win);
      lv_obj_set_size(keyboard, LV_HOR_RES, (LV_VER_RES - HEADER_HEIGHT) / 2);
    }
    lv_keyboard_set_textarea(keyboard, ta);
  } else if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
    lv_obj_del(keyboard);
    keyboard = nullptr;
  }
}

lv_obj_t *input_line(lv_obj_t *win, const char *label, const int y_pos, const char *value) {
  lv_obj_t *cont = lv_win_get_content(win);

  lv_obj_t *text_area = lv_textarea_create(cont);
  lv_textarea_set_one_line(text_area, true);
  lv_textarea_set_password_mode(text_area, false);
  lv_textarea_set_text(text_area, value);
  lv_obj_set_width(text_area, lv_pct(60));
  lv_obj_add_event_cb(text_area, ta_event_cb, LV_EVENT_ALL, win);
  lv_obj_align(text_area, LV_ALIGN_TOP_RIGHT, 0, y_pos);

  /*Create a label and position it above the text box*/
  lv_obj_t *ta_label = lv_label_create(cont);
  lv_label_set_text(ta_label, label);
  lv_obj_align_to(ta_label, text_area, LV_ALIGN_OUT_LEFT_MID, 0, 0);

  return text_area;
}

lv_obj_t *input_slider(lv_obj_t *win, const char *label, const int y_pos, int value) {
  lv_obj_t *cont = lv_win_get_content(win);

  lv_obj_t *slider = lv_slider_create(cont);
  lv_slider_set_value(slider, value, LV_ANIM_ON);
  lv_obj_set_width(slider, lv_pct(60));
  lv_obj_align(slider, LV_ALIGN_TOP_RIGHT, 0, y_pos);

  lv_obj_t *slider_label = lv_label_create(cont);
  lv_label_set_text(slider_label, label);
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_LEFT_MID, 0, 0);

  return slider;
}

lv_obj_t *button(lv_obj_t *win, const char *label, const int width, const int height, const int x_offset, const int y_offset, lv_event_cb_t event_cb) {
  lv_obj_t *cont = lv_win_get_content(win);

  lv_obj_t *btn = lv_btn_create(cont);
  lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_set_size(btn, width, height);
  lv_obj_align(btn, LV_ALIGN_TOP_MID, x_offset, y_offset);
  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, label);
  lv_obj_center(btn_label);

  return btn;
}

lv_obj_t *label(lv_obj_t *win, const char *label_text, const int x_offset, const int y_offset) {
  lv_obj_t *cont = lv_win_get_content(win);

  lv_obj_t *label = lv_label_create(cont);
  lv_obj_align(label, LV_ALIGN_TOP_MID, x_offset, y_offset);
  lv_label_set_text(label, label_text);
  lv_label_set_recolor(label, true);

  return label;
}

void add_state_on_all_objects_of_class(lv_obj_t *parent, const lv_obj_class_t *class_p, lv_state_t state) {
  int child_count = lv_obj_get_child_cnt(parent);
  for (int i = 0; i < child_count; i++) {
    lv_obj_t *child = lv_obj_get_child(parent, i);
    if (lv_obj_check_type(child, class_p)) {
      lv_obj_add_state(child, state);
    }
    add_state_on_all_objects_of_class(child, class_p, state);
  }
}

void clear_state_on_all_objects_of_class(lv_obj_t *parent, const lv_obj_class_t *class_p, lv_state_t state) {
  int child_count = lv_obj_get_child_cnt(parent);
  for (int i = 0; i < child_count; i++) {
    lv_obj_t *child = lv_obj_get_child(parent, i);
    if (lv_obj_check_type(child, class_p)) {
      lv_obj_clear_state(child, state);
    }
    clear_state_on_all_objects_of_class(child, class_p, state);
  }
}
