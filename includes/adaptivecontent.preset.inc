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

  // Return the HTML table.
  return $html;
}

/**
 * Preset edit form.
 *
 */
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

/**
 * Preset edit form validate handler.
 *
 */
function adaptivecontent_preset_create_form_validate($form, &$form_state) {
  /**
   * @todo: Make some validation of the preset.
   */
}

/**
 * Preset edit form submit handler.
 *
 */
function adaptivecontent_preset_create_form_submit($form, &$form_state) {
  if (isset($form_state['values']['pid'])) {
    _adaptivecontent_save_preset($form_state['values']['name'],
                                $form_state['values']['human_name'],
                                $form_state['values']['query'],
                                $form_state['values']['pid']);
    drupal_set_message(t('Preset "' . $form_state['values']['name'] . '" have been updated.'), 'status');
  }
  else {
    _adaptivecontent_save_preset($form_state['values']['name'],
                                $form_state['values']['human_name'],
                                $form_state['values']['query']);
    drupal_set_message(t('Preset "' . $form_state['values']['name'] . '" have been created.'), 'status');
  }
  drupal_goto('admin/config/content/adaptivecontent/presets');
}

/**
 * Preset delete form.
 *
 */
function adaptivecontent_preset_delete_form($form, $form_state, $name) {
  // Load preset, if name is given.
  $default = array();
  if (isset($name)) {
    $preset = _adaptivecontent_load_presets($name);
    if (isset($preset) && $preset) {
      $form = array(
        'pid' => array(
          '#type' => 'hidden',
          '#value' => $preset['pid'],
        ),
      );
      return confirm_form($form,
        t('Are you sure you want to delete this preset?'),
        isset($_GET['destination']) ? $_GET['destination'] : "admin/config/content/adaptivecontent/presets",
        t('This action cannot be undone.'),
        t('Delete'),
        t('Cancel'));
    }
  }
  drupal_set_message(t('The preset "' . $name . '" you tried to delete do not exists.'), 'error');
  drupal_goto('admin/config/content/adaptivecontent/presets');
}

/**
 * Preset delete form submit handler.
 *
 */
function adaptivecontent_preset_delete_form_submit($form, &$form_state) {
	if ($form_state['values']['confirm']) {
		_adaptivecontent_delete_preset($form_state['values']['pid']);
		drupal_set_message(t('Preset has been deleted successfully.'));
  }

	drupal_goto("admin/config/content/adaptivecontent/presets");
}