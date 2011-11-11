<?php
/**
 * Función para confirmar que el usuario quiere solicitar su ingreso a un equipo.
 */
function team_solicitud() {
  //Verifica que el usuario esté registrado como agente libre
  global $user;
  $team = node_load(array('nid' => arg(1)));
	$modalidad = get_modalidad($team->id_Modalidad_Juego_Temporada);
  $equipo_jugador = tiene_equipo($user->uid, $team->id_Modalidad_Juego_Temporada);
  

  if (!($equipo_jugador == AGENTE_LIBRE)) {
    $form['mensaje'] = array(
			'#value' => t('Para poder solicitar unirte a este equipo, ingresa tu disponibilidad de tiempo en: </br></br>') . l('Buscar Equipo', 'taxonomy/term/'. $modalidad->tid .'/free_agent/register'),
		);
		return $form;
  }
		
  //Pasando las variables necesarias para procesar después de confirmar la acción
  $form[ 'nid_Equipo' ] = array (
    '#type' => 'hidden',
    '#value' => $team->nid,  //Agrega a la forma el nid del equipo que obtenemos de la barra de dirección
  );
  
	$form['Mensaje'] = array (
		'#type' => 'textarea',
		'#title' => t('Explica brevemente porque quieres unirte este equipo.'),
		'#required' => TRUE,
  );
  $form['Submit'] = array (
      '#type' => 'submit',
      '#value' => t( 'Unirme' ),
  );
  return $form;
}
/**
  * Validate handler for previus confirm form
  */
function team_solicitud_validate($form, &$form_state) {
	global $user;
	$nid_Equipo = $form_state['values']['nid_Equipo'];
	//Verifica que no se inserten dos solicitudes por parte del jugador
	db_set_active( 'eSM' );
		$solicitud = db_fetch_object(db_query('SELECT ie.Estado FROM {Integrantes_Equipo} AS ie
											  INNER JOIN {Jugador} AS j ON ie.id_Jugador = j.id_Jugador
											  WHERE j.uid = %d AND ie.nid_Equipo = %d',
											  $user->uid, $nid_Equipo));
	db_set_active( 'default' );
	if (is_object($solicitud)) {
		if($solicitud->Estado == INVITADO) {
			form_set_error('nid_Equipo', t('El capitan de este equipo ya te ha invitado.'));
		}
			
		if($solicitud->Estado == SOLICITA) {
			form_set_error('nid_Equipo', t('Ya has enviado esta solicitud anteriormente.'));
		}
	}
	//Verifica que el equipo tenga espacio para aceptar al jugador
	$equipo = node_load(array('nid' => $nid_Equipo));
	$modalidad = get_modalidad($equipo->id_Modalidad_Juego_Temporada);
	$num_players = _team_num_players($equipo->nid);
	if ($num_players == $modalidad->Maximo_Jugadores) {
		form_set_error('nid_Equipo', t('Este equipo ya tiene el máximo de jugadores permitidos en esta modalidad. </br>No puedes enviar esta solicitud.'));
	}
	//Verifica que el usuario no tenga equipo todavía
	$estado = tiene_equipo($user->uid, $modalidad->id_Modalidad_Juego_Temporada);
	if ($estado == TIENE_EQUIPO) {
	  form_set_error('nid_Equipo', t('Ya perteneces a un equipo en esta modalidad.'));
	}
}
/**
* Submit handler for previus confirm form
*/
function team_solicitud_submit($form, &$form_state) {
	global $user;
	$equipo = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );
	
	db_set_active('eSM');
		$jugador = db_fetch_object(db_query('SELECT * FROM {Jugador}
																				WHERE uid = %d AND id_Modalidad_Juego_Temporada = %d',
																				$user->uid, $equipo->id_Modalidad_Juego_Temporada));
	db_set_active('default');
	
	//Insertar el registro de solicitud en la base de datos
	db_set_active( 'eSM' );
		db_query ('INSERT INTO { Integrantes_Equipo }
							( nid_Equipo, id_Jugador, Estado, Mensaje )
							VALUES ( %d, %d, %d, "%s" )',
							$equipo->nid, $jugador->id_Jugador, SOLICITA, $form_state['values']['Mensaje']);
	db_set_active('default');
  
  //Notifica al capitán del equipo utilizando el módulo rules para disparar una acción configurable
  rules_invoke_event('solicitud_usuario', $user, user_load(array('uid' => $team->uid)), $team);
  
  //set a message
  drupal_set_message( '¡Tu solicitud ha sido enviada con éxito!' );
}