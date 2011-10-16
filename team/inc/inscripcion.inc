<?php
/**
 * Archivo para desplegar las formas que controlan la inscripción a las diferentes modalidades
 * la función de inscripción es la principal que revisa si la modalidad actual es válida en
 * la temporada activa, en caso de que no esté activa despliega el mensaje y regresa a la vista de la modalidad
 * Si la modalidad actual es válida en la temporada activa, revisa si es individual o por equipos
 * y despliega las opciones correspondintes.

/**
 * función que revisa el estado de la modalidad que se está visitando y si es válida en la temporada activa
 * despliega las opciones de acuerdo al máximo de jugadores para los equipos.
 */
function inscripcion( ) {
  global $user;
  
  if( !function_exists("node_object_prepare")) {
     include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
  }
  $type = 'team';
  
  // Initialize settings:
  $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');
  
  return drupal_get_form($type .'_node_form', $node);
}