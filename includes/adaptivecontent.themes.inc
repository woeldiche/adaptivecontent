<?php

function adaptivecontent_theme_preset_form($form, $form_state, $theme) {
  $form = array(
    '#tree' => TRUE,
  );

  // Find all regions for the selected theme.
  $themes_info = list_themes();
  $theme = $themes_info[$theme];
  $regions = $theme->info['regions'];

  // Load values.
  $default = variable_get('adaptivecontent_theme_' + $theme->name, array());

  $form['adaptivecontent_theme_' + $theme->name]['core'] = array(
    '#title' => t('Core content'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adaptivecontent_theme_' + $theme->name]['core']['regions'] = array(
    '#title' => t('Regions'),
    '#type' => 'checkboxes',
    '#options' => $regions,
    '#default_value' => isset($default['core']['regions']) ? $default['core']['regions'] : array(),
  );


  $presets = _adaptivecontent_load_presets();
  foreach ($presets as $name => $preset) {
    $form['adaptivecontent_theme_' + $theme->name][$name] = array(
      '#title' => t('Preset - %name', array('%name' => $name)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['adaptivecontent_theme_' + $theme->name][$name]['regions'] = array(
      '#title' => t('Regions'),
      '#type' => 'checkboxes',
      '#options' => $regions,
      '#default_value' => isset($default[$name]['regions']) ? $default[$name]['regions'] : array(),
    );
  }


  /**
   * @todo: Load all presets from configuration. For now only regions is
   * adaptively loaded
   */


  return system_settings_form($form);
}
