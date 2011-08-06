<?php
/**
 * Despliega una forma de confirmación para asegurarnos
 * de que no se ha pulsado el link de aceptar por error
 */
function match_aceptar_desafio() {
  return confirm_form($form, 'Seguro quieres aceptar este desafio?', 'node/'. arg(1), 'Una vez aceptado esta desafio no se podra cancelar ni cambiar las condiciones', 'Si, aceptar desafio', 'Cancel'); //Had better luck leaving off last param 'name'	
}
/**
 * Valida que el desafio se pueda confirmar
 */
function match_aceptar_desafio_validate($form, &$form_state) {
  global $user;
  $node = node_load(array('nid' => arg(1)));
  $away_team = node_load(array('nid' => $node->nid_Equipo_Visitante));
  
  //Revisa que el estado del desafio tenga el estado de creado y manda error en caso contrario
  if ($node->Estado != CREADO)
	form_set_error($form, 'El desafio no admite esta operacion');
	
  //Revisa que el usuario ejecutando la acción sea capitán del equipo retado
  if (!team_user_is_team_capitan($user->uid, $away_team->nid))
	  form_set_error($form, 'Solo el capitan del equipo '. $away_team->title .' puede aceptar este reto');
  
}
/**
 * Acepta el desafio:
 * 	- Actualiza el estado del desafío en la base de datos
 * 	- Envía notificaciones al capitán del equipo contrario
 */
function match_aceptar_desafio_submit($form, &$form_state) {
	//Revisa que no se hayan colocado errores en la forma
	
	if (is_array($errores = drupal_get_messages('error'))) {
		//Despliega los errores asignados a la forma
		
		foreach($errores['error'] AS $index => $error) {
			if (!(strncmp('warning', $error, 7) == 0))
			drupal_set_message($error, 'error');
		}
		
		drupal_goto('node/'. arg(1));
	}
	else {
		//No hay errores, por lo tanto acepta el reto
		
		$node = node_load(array('nid' => arg(1)));
	
		//Cambia el estado del match en la base de datos
		db_set_active('eSM');
			db_query('UPDATE {Reto} SET Estado = %d WHERE nid_Reto = %d', PROGRAMADO, $node->nid);
		db_set_active('default');
		
		//Envía la notificación de que se aceptó el match al equipo creador
	  //Primero obtenemos los datos de los capitanes de los equipos  
		$home_team = node_load(array('nid' => $node->home_team));
		$away_team = node_load(array('nid' => $node->away_team));
		
		$home_capitan = user_load(array('uid' => $home_team->uid));
		$away_capitan = user_load(array('uid' => $away_team->uid));
		
		rules_invoke_event('desafio_aceptado', $away_capitan, $home_capitan, $node);
		drupal_set_message('El desafio ha sido aceptado correctamente.');
		drupal_goto('node/'. $node->nid);
	}
}