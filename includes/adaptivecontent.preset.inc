<?php
/**
 * @file
 * 
 */

/**
 * Preset administration interface to define media query based layouts.
 * 
 */
function adaptivecontent_preset_form($form_state) {
  $form = array();
  
  $form['presets'] = array(
    '#title' => t('Presets'),
    '#type' => 'fieldset',
    '#description' => 'Configure presets for which content should be loaded adaptivelly.'
  );
  
  $form['presets']['adaptivecontent_preset_normal'] = array(
    '#title' => t('Preset 1'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  
  $form['presets']['adaptivecontent_preset_normal']['title'] = array(
    '#title' => t('Preset name'),
    '#type' => 'textfield',
    '#default_value' => t('Normal'),
  );

  $form['presets']['adaptivecontent_preset_normal']['query'] = array(
    '#title' => t('Media query'),
    '#type' => 'textfield',
    '#description' => t('The @media query that this layout should respond to.'),
    '#default_value' => 'all and (min-width: 740px) and (min-device-width: 740px), (max-device-width: 800px) and (min-width: 740px) and (orientation:landscape)',
  );
  
  return system_settings_form($form);
}

function adaptivecontent_theme_preset_form($form, $form_state, $theme) {
  $form = array();

  // Find all regions for the selected theme.
  $themes_info = list_themes();
  $theme = $themes_info[$theme];
  $regions = $theme->info['regions'];
  
  
  
  return system_settings_form($form);
}
