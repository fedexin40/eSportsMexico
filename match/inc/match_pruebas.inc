<?php
/**
 * Despliega una forma de confirmación para asegurarnos
 * de que no se ha pulsado el link de aceptar por error
 */
function match_pruebas() {
  	//Obtiene el objeto que contien el nodo
	$node = node_load(arg(1));
	//Deffine la constante de usuario global
	global $user;
	
	$form['instrucciones'] = array (
	  '#value' => 'Introduce el link de donde esta almacenada tu prueba',
	);
		  
	//Obtiene los objetos de los equipos participantes en el reto
	if (team_user_is_team_capitan($user->uid, $node->nid_Equipo_Local)) {
	  $form['tipo'] = array (
		'#type' => 'hidden',
		'#value' => 'Prueba_Jugador_Local',
	  );
	}
	else {
	  $form['tipo'] = array (
		'#type' => 'hidden',
		'#value' => 'Prueba_Jugador_Visitante',
	  );
	}
	
	$form['prueba'] = array (
	  '#title' => 'Prueba',
	  '#type' => 'textfield',
	  '#required' => TRUE,
	  '#size' => 100,
	);
	
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enviar Prueba'),
  );
	return $form;
}
/**
 * Valida que el desafio esté en estado de conflicto
 */
function match_pruebas_validate($form, &$form_state) {
  global $user;
  $node = node_load(array('nid' => arg(1)));
  $pruebas = match_get_pruebas($node->nid);
  
  //Revisa que el estado del desafio tenga el estado de creado y manda error en caso contrario
  if ($node->Estado != CONFLICTO)
	form_set_error('prueba', 'No es necesario que envies pruebas de este desafio porque no esta en Estado de Conflicto');
  
  //Revisa que el equipo no haya enviado una prueba anterior
  if ($pruebas['Prueba_Jugador_Local'] != '' && team_user_is_team_capitan($user->uid, $node->nid_Equipo_Local))
	form_set_error('prueba', 'No puedes enviar mas que una prueba por Desafio');
	
  if ($pruebas['Prueba_Jugador_Visitante'] != '' && team_user_is_team_capitan($user->uid, $node->nid_Equipo_Visitante))
	form_set_error('prueba', 'No puedes enviar mas que una prueba por Desafio');
  
  //Verifica que el link sea válido
  if (!valid_url($form_state['values']['prueba'], TRUE))
	form_set_error('prueba', t('Invalid Web URL'));
}
/**
 * Guarda la prueba en la base de datos
 *
 */
function match_pruebas_submit($form, &$form_state) {
	global $user;
	$node = node_load(array('nid' => arg(1)));
	$pruebas = match_get_pruebas($node->nid);
	
  if (!is_array($pruebas)) {
	db_set_active('eSM');
	  db_query('INSERT INTO {Prueba} (nid_Reto) VALUES (%d)', $node->nid);
	db_set_active('default');
  }

  if (team_user_is_team_capitan($user->uid, $node->nid_Equipo_Local)) {
	db_set_active('eSM');
	  db_query('UPDATE {Prueba} SET Prueba_Jugador_Local = "%s" WHERE nid_Reto = %d', $form_state['values']['prueba'], $node->nid);
	db_set_active('default');
  }
  else {
	db_set_active('eSM');
	  db_query('UPDATE {Prueba} SET Prueba_Jugador_Visitante = "%s" WHERE nid_Reto = %d', $form_state['values']['prueba'], $node->nid);
	db_set_active('default');
  }

	drupal_set_message('Has enviado tu prueba correctamente.');
	drupal_goto('node/'. $node->nid);

}