<?php
/**
 * Archivo para administrar las solicitudes de invitación para un usuario
 */


/**
 *	Función para administrar las invitaciones a un jugador
 *	Si no tiene equipo, podrá unirse
 *	Si tiene equipo, no podrá unirse, podrá editar sus equipos
 */
function administrar_invitaciones( ) {
	
	module_load_include( 'php', 'base_datos_externa', 'db_functions' );
	
	global $user;
	
	db_set_active(  'eSM' );

	//Consulta para obtener todos los equipos donde tiene invitación
		$invitaciones_result = db_query
		(
			'SELECT
					*
				FROM
					{ Equipo }
				AS
					{ e }
			INNER JOIN
					{ Integrantes_Equipo }
				AS
					{ ie }
				ON
					ie.{ nid_Equipo } = e.{ nid_Equipo }
			INNER JOIN
					{ Jugador }
				AS
					{ j }
				ON
					j.{ id_Jugador } = ie.{ id_Jugador }
			WHERE		
					ie.{ Estado }	= %d
				AND
					j.{ uid } = %d',
			INVITADO,
			$user->uid
		);

	db_set_active(  'default' );
	
	$count = 0;			//Contador de invitaciones
	$options = array ();
	
	while( $invitacion = db_fetch_object( $invitaciones_result ) ) {
		$team = node_load ($invitacion->nid_Equipo);
		$modalidad = get_modalidad_juego_temporada($team->id_Modalidad_Juego_Temporada);
		$capitan = user_load(array('uid' => $team->uid));
		$options[$team->nid] = '';
		
		$form['equipos_disponibles'][ $team->nid ][ 'imagen' ] = array (
		  '#value' => $team->imagen,
		);
		$form['equipos_disponibles'][ $team->nid ][ 'nombre' ] = array (
		  '#value' => l($team->title, '/node/'. $team->nid)
		);
		$form['equipos_disponibles'][ $team->nid ][ 'tag' ] = array (
		  '#value' => $team->abreviacion,
		);
		$form['equipos_disponibles'][ $team->nid ][ 'capitan' ] = array (
		  '#value' => l($capitan->name, 'user/'. $capitan->uid),
		);
		$form['equipos_disponibles'][ $team->nid ][ 'plazas' ] = array (
		  '#value' => ($modalidad->Maximo_Jugadores - $team->Numero_Integrantes),
		);
		
		$form['Id_Jugador_'. $team->nid] = array (
			'#type' => 'hidden',
			'#value' => $invitacion->id_Jugador,
		);
		
			
		++$count;

	}
	
	if ($count > 0) {

		$form['options'] = array (
		  '#type' => 'radios',
		  '#options' => $options, 
		);

		$form[ 'submit' ] = array (
			'#type' => 'submit',
			'#value' => t( 'Aceptar Invitacion Seleccionada' ),
		);
		
	}
	else {	

		$form[ 'mensaje' ] = array (
			'#value' => t( 'No tienes ninguna invitacion en este momento.' ),
		);

	}
	
	$form['#theme'] = 'user_invitaciones';

	return $form;

}




/**
		Función para el insert anterior
 * Función para crear la forma de invitación
 */
/*function administrar_invitaciones_validate($form, $form_state) {
    $equipo = node_load( $form_state['values']['nid_Equipo'] );

		if


    if ( ($form_state['values']['id_Jugador']) == NULL ) {
        form_set_error('invitado', 'El jugador no está registrado en esta modalidad.');
    }
    else {
        $jugador = get_jugador($form_state['values']['uid_invitado'], $equipo->id_Modalidad_Juegos_Temporadas);
        if ($jugador->Estado == TIENE_EQUIPO) {
            form_set_error('invitado', 'No puedes invitar a esta jugador porque ya pertenece a un equipo.');
        }
        else {
            3('eSM');
                $relaciones = db_query('SELECT * FROM {Integrantes_Equipo} WHERE id_Jugador = %d AND nid_Equipo = %d', $jugador->id_Jugador, $equipo->nid);
            db_set_active('default');
            while ($relacion = db_fetch_array($relaciones)) {
                switch ($relacion['Estado']) {
                    case 1:
                        form_set_error('invitado', t('Ya has invitado a este jugador. Espera a que acepte tu invitación.'));
                        break;
                    case 2:
                        form_set_error('invitado', 'Este jugador ya ha solicitado ser parte del equipo.');
                        break;
                    case 3:
                        form_set_error('invitado', 'Este jugador ya forma parte de tu equipo.');
                        break;
                    default :
                        break;
                }
            }
        }
    }
}
 */


/*
 *	Función que inserta el estado ACEPTADO en la tabla Integrantes_Equipo de acuerdo
 *	a la invitación seleccionada
 */
function administrar_invitaciones_submit ( $form, $form_state ) {
	global $user;
	$equipo = node_load($form_state['values']['options']);	//Obtiene los datos del equipo para saber en que modalidad está inscrito
	
	if ($form_state['values']['options'] != 0 ) {		//Cambia el estado del jugador a ACEPTADO en la tabla de Integrantes Equipo
		db_set_active('eSM');
			db_query('UPDATE {Integrantes_Equipo} SET Estado = %d
					 WHERE nid_Equipo = %d AND id_Jugador = %d',
					 ACEPTADO, $form_state['values']['options'], $form_state['values']['Id_Jugador_'. $form_state['values']['options']]);
														//Cambia el estado del jugador en la tabla de Jugadores
			db_query('UPDATE {Jugador} SET Estado = %d
					 WHERE id_Jugador = %d AND id_Modalidad_Juego_Temporada = %d',
					 TIENE_EQUIPO, $form_state['values']['Id_Jugador_'. $form_state['values']['options']], $equipo->id_Modalidad_Juego_Temporada);
		db_set_active('default');
		
		//Envía mensaje al capitán del equipo notificando que el usuario a aceptado su invitación
		rules_invoke_event('invitacion_aceptada', $user, user_load(array('uid' => $equipo->uid)), $equipo->nid);
		
		/**
		 * Rechaza de manera automática todas las demás invitaciones que haya para la misma modalidad
		 */
		db_set_active('eSM');
			$result = db_query('SELECT * FROM {Integrantes_Equipo} AS ie INNER JOIN {Equipo} AS e ON ie.nid_Equipo = e.nid_Equipo
							   WHERE e.id_Modalidad_Juego_Temporada = %d AND ie.id_Jugador = %d',
							   $equipo->id_Modalidad_Juego_Temporada, $form_state['values']['Id_Jugador_'. $form_state['values']['options']]);
		db_set_active('default');
		
		while ($invitacion = db_fetch_object($result)) {
			if ($invitacion->Estado == INVITADO || $invitacion->Estado == SOLICITA) {
				db_set_active('eSM');
					db_query('UPDATE {Integrantes_Equipo} SET Estado = %d
							 WHERE nid_Equipo = %d AND id_Jugador = %d',
							 RECHAZADO, $invitacion->nid_Equipo, $invitacion->id_Jugador);
				db_set_active('default');
				
				//Si era una invitación, envía un mensaje al capitán del equipo
				if ($invitacion->Estado == INVITADO) {
					$equipo_rechazado = node_load(array('nid' => $invitacion->nid_Equipo));
					rules_invoke_event('invitacion_rechazada', $user, user_load(array('uid' => $equipo_rechazado->uid)), $equipo_rechazado);
				}
			}
		}
		
		drupal_set_message( t( 'Te has unido al equipo.') );
		drupal_goto( '/node/'.$form_state['values']['options'] );
	}
}



/**
 *	Función para que el usuario rechace invitaciones
 *	Sólo es una confirmación con
 *
 */
function rechazar_invitacion(  )
{
		//Pasando las variables necesarias para procesar después de confirmar la acción
	$form[ 'nid_Equipo' ] = array
	(
		'#type' => 'value',
		'#value' => arg( 1 ),  //Agrega a la forma el nid del equipo que obtenemos de la barra de dirección
	);
	//Definiendo a donde se va a redirigir la forma
	//$form['#redirect'] = 'user';
	

	return confirm_form
	(
		$form,
		t( '¿Quieres rechazar la solicitud del equipo?' ), //message title
		( isset( $_GET[ 'destination' ] ) ? $_GET[ 'destination' ] : 'user' ),//redirect if cancelled
		//t( '' ), //message description
		t( '¿Estás seguro?' ),
		t( 'Si' ), //confirm button text
		t( 'No' ) //cancel button text
	);
	
}
	
	/**
	* Submit handler for previus confirm form
	*/
	function rechazar_invitacion_submit( $form, &$form_state )
	{ 
		module_load_include( 'php', 'base_datos_externa', 'db_functions' );
		
	
    $nid_Equipo = $form_state[ 'values' ][ 'nid_Equipo' ];

    $id_Jugador = $form_state[ 'values' ][ 'id_Jugador'];
    
    db_set_active( 'eSM' );
        db_query( 'INSERT INTO { Integrantes_Equipo } ( nid_Equipo, id_Jugador, Estado, Mensaje ) VALUES ( %d, %d, %d, "%s" )', $nid_Equipo, $id_Jugador, RECHAZADO, 'Rechazada la invitación' );
    db_set_active( 'default' );
    
    drupal_set_message( t( 'Tu unión se ha enviado correctamente.' ) );
			$modalidad = get_modalidad_juego_temporada( $equipo->id_Modalidad_Juego_Temporada );
    drupal_goto( '/libres/'.$modalidad->tid.'/lista' );

	
	}
	
		
		