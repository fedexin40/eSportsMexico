<?php
/**
 * Despliega una forma de confirmaci�n para asegurarnos
 * de que no se ha pulsado el link de aceptar por error
 */
function match_aceptar_desafio() {
  
  return confirm_form($form, 'Seguro quieres aceptar este desafio?', 'node/'. arg(1), 'Una vez aceptado esta desafio no se podra cancelar ni cambiar las condiciones', 'Si, aceptar desafio', 'Cancel'); //Had better luck leaving off last param 'name'	
}
/**
 * Valida que el desafio se pueda confirmar
 */
function match_aceptar_desafio_validate($form, &$form_state) {
  $node = node_load(array('nid' => arg(1)));
  
  //Revisa que el estado del desafio tenga el estado de creado y manda error en caso contrario
  if ($node->Estado != CREADO)
	form_set_error(NULL, 'Este desafio no admite esta operaci�n');
	
  //Revisa que el usuario ejecutando la acci�n sea capit�n del equipo retado
  if ()
  
}
/**
 * Acepta el desafio:
 * 	- Actualiza el estado del desaf�o en la base de datos
 * 	- Env�a notificaciones al capit�n del equipo contrario
 */
function match_aceptar_desafio_submit($form, &$form_state) {
	$node = node_load(array('nid' => arg(1)));
	
	//Cambia el estado del match en la base de datos
	db_set_active('eSM');
		db_query('UPDATE {Reto} SET Estado = %d WHERE nid_Reto = %d', PROGRAMADO, $node->nid);
	db_set_active('default');
	
	//Env�a la notificaci�n de que se acept� el match al equipo creador
  //Primero obtenemos los datos de los capitanes de los equipos  
    $home_team = node_load(array('nid' => $node->home_team));
    $away_team = node_load(array('nid' => $node->away_team));
    
    $home_capitan = user_load(array('uid' => $home_team->uid));
    $away_capitan = user_load(array('uid' => $away_team->uid));
	
	rules_invoke_event('desafio_aceptado', $away_capitan, $home_capitan, $node);
    drupal_set_message('El desafio ha sido aceptado correctamente.');
	drupal_goto('node/'. $node->nid);
}