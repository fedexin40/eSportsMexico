<?php
/**
 *	@file
 *	Form callbacks for Image for Team
 */
/**
 * Gathers information on if a team has an image configured and offered to configure it if not. As with some other functions, this is shamelessly taken from the Ubercart code.
 */
function team_admin(){
	
  if (!module_exists('imagefield') || !module_exists('imagecache')) {
    $status = 'warning';
    $description = t('To automatically configure team image support, <a href="!url">enable</a> the <a href="http://drupal.org/project/cck">Content</a>, <a href="http://drupal.org/project/imagefield">CCK Image field</a>, and <a href="http://drupal.org/project/imagecache">Imagecache</a> modules.', array('!url' => url('admin/build/modules')));
  } else {
    module_load_include('inc', 'content', 'includes/content.crud');
    // Check for filefields on products.
    $instances = content_field_instance_read(array('field_name' => 'field_image_cache', 'type_name' => 'team'));
    $field_check = (bool) count($instances);

    if ($field_check) {
      $description = t('El modulo de equipos esta correctamente configurado');
    }
    else {
      $description = t('<a href="!url">Click here</a> to automatically configure the leaguesite team content type for team image support:', array('!url' => url('admin/base_datos_externa/team/image')));
      if (!$field_check) {
        $items[] = t('The Image file field has not been created for teams.');
      }
      $description .= theme('item_list', $items) . t('(This action is not required and should not be taken if you do not need images or have implemented your own image support.)');
    }
  }

  return $description;
}
/**
 * This adds an imagefield to a team. Code taken from uc_product.module.. Shame on me.
 * Requires imagefield, filefield and CCK.
 */
function team_add_image_field() {
  module_load_include('inc', 'imagefield', 'imagefield_widget');
  module_load_include('inc', 'filefield', 'filefield_widget');
  module_load_include('inc', 'content', 'includes/content.crud');

  $label = t('Image');
  $field = array(
    'type_name' => 'team',
    'field_name' => 'field_image_cache',
    'label' => $label,
    'type' => 'filefield',
    'widget_type' => 'imagefield_widget',
    'weight' => -2,
    'file_extensions' => 'gif jpg png',
    'custom_alt' => 1,
    'custom_title' => 1,
    'description' => '',
    'required' => 0,
    'multiple' => 1,
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'module' => 'filefield',
    'widget_module' => 'imagefield',
    'columns' => array(
      'fid' => array(
        'type' => 'int',
        'not null' => FALSE,
      ),
      'list' => array(
        'type' => 'int',
        'size' => 'tiny',
        'not null' => FALSE,
      ),
      'data' => array(
        'type' => 'text',
        'serialize' => TRUE,
      ),
    ),
    'display_settings' => array(
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
      ),
      'full' => array(
        'format' => 'team_full',
      ),
      4 => array(
        'format' => 'hidden',
      ),
    ),
  );

  $instances = content_field_instance_read(array('field_name' => 'field_image_cache', 'type_name' => 'team'));
  if (count($instances) < 1) {
    // Only add the field if it doesn't exist. Don't overwrite any changes.
    content_field_instance_create($field);
  }
  
  drupal_set_message(t('Default image support configured for Teams using CCK and ImageCache.'));

  drupal_goto('admin/base_datos_externa/team');
}