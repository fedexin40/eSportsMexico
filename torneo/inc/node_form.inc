<?php

/**
 * Alters the node form to inject the appropriate per-node torneo settings.
 */
function torneo_alter_node_form(&$form, &$form_state, $form_id) {
  global $user;

  // Node is not saved but previewed (nid is empty).
  if (isset($form['#node']->build_mode) && $form['#node']->build_mode == NODE_BUILD_PREVIEW) {
    $node = $form['#node'];
  }
  // Load the node if it already exists.
  elseif (!empty($form['nid']['#value'])) {
    $node = node_load($form['nid']['#value']);
  }
  else {
    $node = NULL;
  }
  $node_type = $form['type']['#value'];

  $signup_type_default = variable_get('signup_node_default_state_'. $node_type, 'disabled');

  // If signups are possible, and the current user either has the global
  // 'administer all signups' permission or has the 'administer signups
  // for own content' permission and is creating new content or editing
  // their own content, add a fieldset for signup-related settings.

  // Signups are possible if they're not explicitly disallowed for this
  // node type, or if this node is already signup-enabled (in case an
  // admin erroneously marks a node-type to disallow signups when there
  // are already nodes of that type with signups enabled).
  $signups_possible = $signup_type_default != 'disabled' || (!empty($node) && !empty($node->nid) && !empty($node->signup));
  $admin_all = user_access('administer all signups');
  $admin_own = user_access('administer signups for own content') && (empty($node) || ($node->uid == $user->uid));
  if ($signups_possible && ($admin_all || $admin_own)) {
    $form['signup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Signup settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 30,
      '#group' => 'additional_settings',
    );

    // Figure out what the options should be.  If there are already
    // people signed-up for this node, we need a 3rd choice: disable
    // signups and remove all signup data.
    $has_signups = !empty($node) && db_result(db_query("SELECT COUNT(*) from {signup_log} WHERE nid = %d", $node->nid));
    $radio_options[1] = t('Enabled');
    if ($has_signups) {
      $radio_options[0] = t('Disabled, but save existing signup information');
      $radio_options[2] = t('Disabled, and remove all signup information') .' <strong>('. t('This can not be undone, use with extreme caution!') .')</strong>';
    }
    else {
      $radio_options[0] = t('Disabled');
    }

    // Figure out what the default selection for signups should be.
    if (isset($node->signup)) {
      $default_option = $node->signup;
    }
    else {
      $default_option = $signup_type_default == 'enabled_on' ? 1 : 0;
    }
    // Add the form element to toggle if signups are allowed.
    $form['signup']['signup_enabled'] = array(
      '#type' => 'radios',
      '#options' => $radio_options,
      '#default_value' => $default_option,
      '#description' => t('If enabled, you can control whether users may sign up by visiting the !signup_admin tab and toggling if signups are %open or %closed for this %node_type. Other signup-related settings can be defined at the !signup_settings tab.', array('!signup_admin' => !empty($node->signup) ? l(t('Signups: Administer'), 'node/'. $node->nid .'/signups/admin') : theme('placeholder', t('Signups: Administer')), '!signup_settings' => !empty($node->signup) ? l(t('Signups: Settings'), 'node/'. $node->nid .'/signups/settings') : theme('placeholder', t('Signups: Settings')), '%open' => t('open'), '%closed' => t('closed'), '%node_type' => node_get_types('name', $node_type))),
    );
    $form['signup']['signup_user_reg'] = array(
      '#type' => 'checkbox',
      '#title' => t('Users can sign up for this event from the user registration form.'),
      '#description' => t('If selected, new users will be able to sign up for this event when they register.'),
      '#default_value' => $node->signup_user_reg_form,
    );
  }
}
