<?php
/**
 * Archivo para administrar las solicitudes de invitación para un usuario
 *
 *	Función para administrar las invitaciones a un jugador
 *	Si no tiene equipo, podrá unirse
 *	Si tiene equipo, no podrá unirse, podrá editar sus equipos
 */
function administrar_invitaciones( ) {
	module_load_include( 'php', 'base_datos_externa', 'db_functions' );
	global $user;
	
	//Consulta para obtener todos los equipos donde tiene invitación
		$invitaciones_result = db_query('SELECT * FROM {eSM_Equipo} AS { e }
										INNER JOIN {eSM_Integrantes_Equipo } AS { ie } ON ie.{ nid_Equipo } = e.{ nid_Equipo }
										INNER JOIN {eSM_Jugador } AS { j } ON j.{ id_Jugador } = ie.{ id_Jugador }
										WHERE ie.{ Estado }	= %d AND j.{ uid } = %d AND e.{ Estado } = %d',
										INVITADO, $user->uid, ACTIVO);
	
	$count = 0;			//Contador de invitaciones
	$options = array ();
	
	while( $invitacion = db_fetch_object( $invitaciones_result ) ) {
		$team = node_load ($invitacion->nid_Equipo);
		$modalidad = get_modalidad_juego_temporada($team->id_Modalidad_Juego_Temporada);
		$row = array();
		
		$row[ 'imagen' ] = theme('team_pane', $team, variable_get('team_block_roster_picture_preset', ''));
		$row[ 'nombre' ] = l($team->title, '/node/'. $team->nid);
		$row[ 'modalidad' ] = l($modalidad->Nombre, 'taxonomy/term/'. $modalidad->tid);
		//TODO: Poner un espacio para la plataforma del juego
		$row[ 'acciones' ] = l('Accept', 'node/'. $team->nid . '/join/'. $user->uid). ' | ' .l('Decline', 'node/'. $team->nid . '/join/'. $user->uid, array('attributes'=>array('class' => 'popups')));
		++$count;
		$rows[] = $row;
	}
	
	if ($count > 0) {
		$header = array(t(''), t('Team'), t('Arena'), t('Actions'));
		$output = theme('table', $header, $rows);
	}
	else {	
		$output = '<div class="form_item">'. t('No tienes ninguna invitacion en este momento.') .'</div>';
	}

	return $output;
}
/*
 *	Función que inserta el estado ACEPTADO en la tabla Integrantes_Equipo de acuerdo
 *	a la invitación seleccionada
 */
function aceptar_invitacion () {
	global $user;
	$nid_Equipo = arg(1);
	$equipo = node_load(array('nid' => $nid_Equipo));
	
	$form['nid_Equipo'] = array (
		'#type' => 'hidden',
		'#value' => $equipo->nid,
	);
	
	$form['title'] = array (
		'#value' => t('You have been invited to join the team "'). $equipo->title .'".</br>',
	);
	$form['message'] = array (
		'#value' => t('If you wish to accept this invitation please verify/complete the information below and click the "Accept Invitation" button below. If you do not wish to join this team, you may '. l(t('decline this invitation.'), 'node/'. $nid_Equipo .'/decline') .'</br>'),
	);
	$form['submit'] = array (
		'#type' => 'submit',
		'#value' => t('Accept Invitation'),
	);
	
	return $form;
}
/**
 * Función para validad que el usuario efectivamente pueda aceptar la invitación seleccionada
 */
function aceptar_invitacion_validate ($form, $form_state) {
	global $user;
	$equipo = node_load(array('nid' => $form_state['values']['nid_Equipo']));
	$estado = tiene_equipo($user->uid, $equipo->id_Modalidad_Juego_Temporada);
	//Verifica que el jugador no esté registrado en otro equipo
	if($estado == TIENE_EQUIPO) {
		form_set_error('Equipo', t('Ya estas registrado en un equipo de esta modalidad.</br>No puedes aceptar esta invitacion.'), 'error');
	}
	//Verifica que el equipo tenga espacio para el jugador
	$modalidad = get_modalidad($equipo->id_Modadalidad_Juego_Temporada);
    $num_players = _team_num_players($equipo->nid);
	if($num_players == $modalidad->Maximo_Jugadores) {
		form_set_error('Equipo', t('El equipo ha alcanzado el máximo de jugadores permitidos en la modaliad.</b>No puedes aceptar esta solicitud.'));
	}
}
/**
 * Función para actualizar el estado del jugador aceptando la invitación seleccionada
 */
function aceptar_invitacion_submit ($form, $form_state) {
	global $user;
	$equipo = node_load($form_state['values']['nid_Equipo']);	//Obtiene los datos del equipo para saber en que modalidad está inscrito
	
	$jugador = db_fetch_object(db_query('SELECT * FROM {eSM_Jugador}
										WHERE uid = %d AND id_Modalidad_Juego_Temporada = %d',
										$user->uid, $equipo->id_Modalidad_Juego_Temporada));
	
		db_query('UPDATE {eSM_Integrantes_Equipo} SET Estado = %d
				 WHERE nid_Equipo = %d AND id_Jugador = %d',
				 ACEPTADO, $form_state['values']['nid_Equipo'], $jugador->id_Jugador);
		//Cambia el estado del jugador en la tabla de Jugadores
		db_query('UPDATE {eSM_Jugador} SET Estado = %d
				 WHERE id_Jugador = %d AND id_Modalidad_Juego_Temporada = %d',
				 TIENE_EQUIPO, $jugador->id_Jugador, $equipo->id_Modalidad_Juego_Temporada);
			
	//Envía mensaje al capitán del equipo notificando que el usuario a aceptado su invitación
	rules_invoke_event('invitacion_aceptada', $user, user_load(array('uid' => $equipo->uid)), $equipo->nid);
	drupal_set_message( t( 'Te has unido al equipo.') );
}
/**
 *	Función para que el usuario rechace invitaciones
 */
function rechazar_invitacion(  ) {
	//Pasando las variables necesarias para procesar después de confirmar la acción
	$form[ 'nid_Equipo' ] = array (
		'#type' => 'value',
		'#value' => arg( 1 ),  //Agrega a la forma el nid del equipo que obtenemos de la barra de dirección
	);

	return confirm_form (
		$form,
		t( '¿Estas seguro que quieres rechazar la invitacion del equipo?' ), //message title
		( isset( $_GET[ 'destination' ] ) ? $_GET[ 'destination' ] : 'user' ), //redirect if canceled
		t( ' ' ),
		t( 'Si' ), //confirm button text
		t( 'No' ) //cancel button text
	);
}
/**
* Submit handler for previus confirm form
*/
function rechazar_invitacion_submit( $form, &$form_state ) {
	global $user;
	$nid_Equipo = $form_state[ 'values' ][ 'nid_Equipo' ];
	$equipo = node_load(array('nid' => $nid_Equipo));
	
		$jugador = db_fetch_object(db_query('SELECT * FROM {eSM_Jugador}
											WHERE uid = %d AND id_Modalidad_Juego_Temporada = %d',
											$user->uid, $equipo->id_Modalidad_Juego_Temporada));
		
		db_query( 'UPDATE {eSM_Integrantes_Equipo}
				 SET Estado = %d WHERE
				 nid_Equipo = %d AND id_Jugador = %d)',
				 RECHAZADO, $nid_Equipo, $jugador->id_Jugador);
	
	drupal_set_message( t( 'Haz rechazado la solicitud del unirte al equipo '. $equipo->title .'.' ) );
	//Notifica al capitán del equipo que el usuario ha rechazado la invitación
	rules_invoke_event('invitacion_rechazada', $user, user_load(array('uid' => $equipo->uid)), $equipo);
}
/**
 * Función para administrar las solicitudes del jugador
 */
function administrar_solicitudes() {
	module_load_include( 'php', 'base_datos_externa', 'db_functions' );
	global $user;
	
	//Consulta para obtener todos los equipos donde tiene invitación
		$equipos_result = db_query('SELECT * FROM {eSM_Equipo } AS { e }
								   INNER JOIN {eSM_Integrantes_Equipo } AS { ie } ON ie.{ nid_Equipo } = e.{ nid_Equipo }
								   INNER JOIN {eSM_Jugador } AS { j } ON j.{ id_Jugador } = ie.{ id_Jugador }
								   WHERE ie.{ Estado }	= %d AND j.{ uid } = %d AND e.{ Estado } = %d',
								   SOLICITA, $user->uid, ACTIVO);
	
	$count = 0;			//Contador de invitaciones
	$options = array ();
	
	while( $equipo = db_fetch_object( $equipos_result ) ) {
		$team = node_load ($equipo->nid_Equipo);
		$modalidad = get_modalidad_juego_temporada($team->id_Modalidad_Juego_Temporada);
		$row = array();

		$row[ 'imagen' ] = theme('team_pane', $team, variable_get('team_block_roster_picture_preset', ''));
		$row[ 'nombre' ] = l($team->title, 'node/'. $team->nid);
		$row[ 'modalidad' ] = l($modalidad->Nombre, 'taxonomy/term/'. $modalidad->tid);
		//TODO: Poner un espacio para la plataforma del juego
		++$count;
		$rows[] = $row;
	}
	
	if ($count > 0) {
		$header = array(t(''), t('Team'), t('Arena'));
		$output = theme('table', $header, $rows);
	}
	else {	
		$output = '<div class="form_item">'. t('No has solicitado unirte a ningun equipo.') .'</div>';
	}

	return $output;
}
/**
 * Función para administrar los equipos del jugador
 */
function administrar_equipos() {
	module_load_include( 'php', 'base_datos_externa', 'db_functions' );
	global $user;
	
	//Consulta para obtener todos los equipos donde tiene invitación
	$equipos_result = db_query('SELECT * FROM {eSM_Equipo } AS { e }
							   INNER JOIN {eSM_Integrantes_Equipo } AS { ie } ON ie.{ nid_Equipo } = e.{ nid_Equipo }
							   INNER JOIN {eSM_Jugador } AS { j } ON j.{ id_Jugador } = ie.{ id_Jugador }
							   WHERE ie.{ Estado }	= %d AND j.{ uid } = %d AND e.{ Estado } = %d',
							   ACEPTADO, $user->uid, ACTIVO);
	
	$count = 0;			//Contador de invitaciones
	$options = array ();
	
	while( $equipo = db_fetch_object( $equipos_result ) ) {
		$team = node_load ($equipo->nid_Equipo);
		$modalidad = get_modalidad_juego_temporada($team->id_Modalidad_Juego_Temporada);
		$row = array();

		$row[ 'imagen' ] = theme('team_pane', $team, variable_get('team_block_roster_picture_preset', ''));
		$row[ 'nombre' ] = l($team->title, 'node/'. $team->nid);
		$row[ 'modalidad' ] = l($modalidad->Nombre, 'taxonomy/term/'. $modalidad->tid);
		//TODO: Poner un espacio para la plataforma del juego
		if($user->uid <> $team->uid)
			$row[ 'acciones' ] = l('Abandonar', 'node/'. $team->nid . '/drop', array('attributes'=>array('class' => 'popups')));
		++$count;
		$rows[] = $row;
	}
	
	if ($count > 0) {
		$header = array(t(''), t('Team'), t('Arena'), t('Actions'));
		$output = theme('table', $header, $rows);
	}
	else {	
		$output = '<div class="form_item">'. t('No perteneces a ningun equipo todavia.') .'</div>';
	}

	return $output;
}
/**
 * Formulario para confirmar la salida del jugador del equipo
 */
function team_drop() {
	//Pasando las variables necesarias para procesar después de confirmar la acción
	$form[ 'nid_Equipo' ] = array (
		'#type' => 'value',
		'#value' => arg( 1 ),  //Agrega a la forma el nid del equipo que obtenemos de la barra de dirección
	);

	return confirm_form (
		$form,
		t( '¿Estas seguro que quieres abandonar este equipo?' ), //message title
		( isset( $_GET[ 'destination' ] ) ? $_GET[ 'destination' ] : 'user' ), //redirect if canceled
		t( ' ' ),
		t( 'Si' ), //confirm button text
		t( 'No' ) //cancel button text
	);
}
/**
 * Función para administrar los equipos del jugador
 */
function team_drop_validate($form, $form_state) {
	global $user;
	
	$jugador = db_fetch_object(db_query('SELECT * FROM {eSM_Integrates_Equipo} AS ie
										INNER JOIN {eSM_Equipo} AS e ON ie.nid_Equipo = e.nid_Equpo
										INNER JOIN {eSM_Jugador} AS j on ie.id_Jugador = j.id_Jugador
										WHERE j.uid = %d AND e.nid_Equipo = %d',
										$user->uid, $form_state['values']['nid_Equipo']));
		
	if($jugador->Estado <> ACEPTADO)
		form_set_error('nid_Equipo', t('No perteneces a la plantilla del equipo.'));
}
/**
 * Función para administrar los equipos del jugador
 */
function team_drop_submit($form, $form_state) {
	global $user;
	
		//Obtiene los datos del jugador
		$jugador = db_fetch_object(db_query('SELECT * FROM {eSM_Jugador} AS j
											INNER JOIN {eSM_Integrantes_Equipo} AS ie ON j.id_Jugador = ie.id_Jugador
											INNER JOIN {eSM_Equipo} AS e ON e.nid_Equipo = ie.nid_Equipo
											WHERE e.nid_Equipo = %d AND j.uid = %d',
											$form_state['values']['nid_Equipo'], $user->uid));
		//Actualiza la tabla de integrantes del equipo
		db_query('UPDATE {eSM_Integrantes_Equipos}
						 SET Estado = %d
						 WHERE nid_Equipo = %d AND id_Jugador = %d',
						 ABANDONO, $form_state['values']['nid_Equipo'], $jugador->id_Jugador);
		//Actualiza el estado del jugador
		db_query('UPDATE {eSM_Jugador}
						 SET Estado = %d
						 WHERE id_Jugador = %d',
						 AGENTE_LIBRE, $jugador->id_Jugador);
}