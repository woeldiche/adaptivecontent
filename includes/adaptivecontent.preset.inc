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
    array('data' => t('Name'), 'field' => 'name', 'sort' => 'asc'),
    array('data' => t('Usage'), 'field' => 'themes'),
    array('data' => t('Operations'), 'field' => 'Operations'),
  );

  // Load presets.
  $presets = variable_get('adaptivecontent_presets', FALSE);

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
          $name,
          '',
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

function adaptivecontent_preset_create_form($form, $form_state) {
  $form = array();

  $form['adaptivecontent_name'] = array(
    '#title' => t('Preset name'),
    '#type' => 'textfield',
    '#description' => t('Name to recognize this preset by (e.g. narrow, normal or wide).'),
    '#required' => TRUE,
  );

  $form['adaptivecontent_query'] = array(
    '#title' => t('Media query'),
    '#type' => 'textfield',
    '#description' => t('The @media query that this layout should respond to.'),
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
  $presets = variable_get('adaptivecontent_presets', array());
  if (isset($presets[$form_state['values']['adaptivecontent_name']])) {
    form_set_error('adaptivecontent_name', t('Preset with that name allready exists. Change the name.'));
    return FALSE;
  }
}

function adaptivecontent_preset_create_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $presets = variable_get('adaptivecontent_presets', array());
  $presets[$form_state['values']['adaptivecontent_name']] = $form_state['values']['adaptivecontent_query'];
  variable_set('adaptivecontent_presets', $presets);

  drupal_set_message(t('Preset "' . $form_state['values']['adaptivecontent_name'] . '" have been created.'), 'status');
  drupal_goto('admin/config/content/adaptivecontent/presets');
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
