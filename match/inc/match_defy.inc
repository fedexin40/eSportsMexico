<?php
/**
 * Despliega el formulario para desafiar a un equipo
 */
function add_new_match() {
	global $user;
	$tid = arg(2);
	
	if (!($modalidad = term_is_active($tid)))
    return t('Esta modalidad no esta activa en esta temporada.');
	
	if (!(is_capitan($user->uid, $modalidad->id_Modalidad_Juego_Temporada)))
		return t('Solo los capitanes pueden crear desafios');
		
  //Si el jugador cumple con todos los requisitos se despliega la forma para crear un nuevo desafio.
  if( !function_exists("node_object_prepare")) 
     include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
		 
  $type = 'match';
  // Initialize settings:
  $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '', 'Estado' => CREADO, 'away_team' => arg(5));
  
  return drupal_get_form($type .'_node_form', $node);
}