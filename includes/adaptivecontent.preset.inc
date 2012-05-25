<?php

function adaptivecontent_preset_form($form_state) {
  $form = array();
  return system_settings_form($form);
}

function adaptivecontent_theme_preset_form($form_state) {
  $form = array();
  return system_settings_form($form);
}
