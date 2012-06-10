<?php
/**
 * @file
 *
 */

/**
 * Preset administration interface to define media query based layouts.
 *
 */
function adaptivecontent_presets() {
  // Define table headers.
  $header = array(
    array('data' => t('Name'), 'field' => 'human_name', 'sort' => 'asc'),
    array('data' => t('Machine name'), 'field' => 'name'),
    array('data' => t('Usage'), 'field' => 'themes'),
    array('data' => t('Operations'), 'field' => 'Operations'),
  );

  // Load presets.
  $presets = _adaptivecontent_load_presets();

  $rows = array();
  if ($presets) {
    foreach ($presets as $name => $preset) {
      // Define supported oprations.
      $operations = array(
        'Edit' => l(t('Edit'), 'admin/config/content/adaptivecontent/presets/' . $name . '/edit'),
        'Delete' => l(t('Delete'), 'admin/config/content/adaptivecontent/presets/' . $name . '/delete')
      );

      // Allow other modules to add more operations.
      drupal_alter('adaptivecontent_presets_operations', $operations);

      $rows[] = array(
        'data' => array(
          $preset['human_name'],
          $name,
          '', // @todo: Load preset usage... just for fun.
          implode(' ', $operations),
        ),
      );
    }
  }

  // Theme the information as a table.
  $html = theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'sticky' => TRUE,
      'empty' => t('No presets found...'),
    )
  );

  // Add a pager to the table, for those sites with really may presets
  $html .= theme('pager', array('tags' => array()));

  // Give it some styling.
//  drupal_add_css(drupal_get_path('module', 'campaignmonitor') . '/css/campaignmonitor.admin.css');

  return ($html);
}

function adaptivecontent_preset_edit_form($form, $form_state, $name = NULL) {
  $form = array();

  // Load defaults if name is given.
  $default = array();
  if (isset($name)) {
    $default = _adaptivecontent_load_presets($name);

    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $default['pid'],
    );
  }

  $form['human_name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#description' => t('Name to recognize this preset by (e.g. narrow, normal or wide).'),
    '#default_value' => isset($default['human_name']) ? $default['human_name'] : '',
    '#required' => TRUE,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#description' => t('A unique machine-readable name for this View. It must only contain lowercase letters, numbers, and underscores.'),
    '#disabled' => isset($default['name']) ? TRUE : FALSE,
    '#default_value' => isset($default['name']) ? $default['name'] : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => '_adaptivecontent_load_presets',
      'source' => array('human_name'),
    ),
  );

  $form['query'] = array(
    '#title' => t('Media query'),
    '#type' => 'textfield',
    '#description' => t('The @media query that this layout should respond to.'),
    '#default_value' => isset($default['query']) ? $default['query'] : '',
    '#maxlength' => 256,
    '#size' => 128,
    '#required' => TRUE,
  );

  $form = system_settings_form($form);
  $form['#submit'] = array('adaptivecontent_preset_create_form_submit');

  // @todo try to validate that the media query is valided
  $form['#validate'][] = 'adaptivecontent_preset_create_form_validate';

  return $form;
}

function adaptivecontent_preset_create_form_validate($form, &$form_state) {
  /**
   * @todo: Make some validation of the preset.
   */
}

function adaptivecontent_preset_create_form_submit($form, &$form_state) {
  if (isset($form_state['values']['name'])) {
    _adaptivecontent_save_preset($form_state['values']['name'],
                                $form_state['values']['human_name'],
                                $form_state['values']['query']);
    drupal_set_message(t('Preset "' . $form_state['values']['name'] . '" have been created.'), 'status');
  }
  else {
    _adaptivecontent_save_preset($form_state['values']['name'],
                                $form_state['values']['human_name'],
                                $form_state['values']['query']);
    drupal_set_message(t('Preset "' . $form_state['values']['name'] . '" have been created.'), 'status');
  }
  drupal_goto('admin/config/content/adaptivecontent/presets');
}

function adaptivecontent_preset_delete_form($form, $form_state, $name) {

}

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

  /**
   * @todo: Load all presets from configuration. For now only regions is
   * adaptively loaded
   */
  $form['adaptivecontent_theme_' + $theme->name]['preset_1'] = array(
    '#title' => t('Preset - Normal'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adaptivecontent_theme_' + $theme->name]['preset_1']['regions'] = array(
    '#title' => t('Regions'),
    '#type' => 'checkboxes',
    '#options' => $regions,
    '#default_value' => isset($default['preset_1']['regions']) ? $default['preset_1']['regions'] : array(),
  );

  return system_settings_form($form);
}
