<?php

function adaptivecontent_setting_form($form_state) {
  $form = array();
  
  krumo('test');
  
  return system_settings_form($form);
}