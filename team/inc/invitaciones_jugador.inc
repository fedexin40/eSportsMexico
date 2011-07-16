<?php


/**
 * Archivo para administrar las solicitudes de invitación para un usuario
 */


	/**
	 *	Función para administrar las invitaciones a un jugador
	 *	Si no tiene equipo, podrá unirse
	 *	Si tiene equipo, no podrá unirse, podrá editar sus equipos
	 */
	function administrar_invitaciones( )
	{
    global $user;

    $usuario_administrador = user_load( arg( 1 ) );  //El primer argumento en la ruta define el uid del jugador al que le pertenecen las invitaciones

		$equipo = node_load(arg(3)); 

    $jugador = get_jugador( $usuario_administrador->uid, $equipo->id_Modalidad_Juego_Temporada );


		
		db_set_active( 'eSM' );

			$result_set_Jugador = db_query( "SELECT * FROM { Jugador }" );

		db_set_active( 'default' );


		$Tiene_Equipo = 0;

		while( $Jugador = db_fetch_object( $result_set_Jugador ) ) {
			
			if( $Jugador->Estado == TIENE_EQUIPO ) {
				$Tiene_Equipo = 1;
				break;
			}
		}

		$form[ 'invitaciones' ] = array (
			'#title' => t( 'Invitaciones.' ),
			'#type' => 'fieldset',
			//'#default_value' => $Equipos->Nombre,
			//'#options' => $options,
			'#collapsible' => 0,
		);


		db_set_active(  'eSM' );

		//Consulta para obtener todos los equipos donde tiene invitación
		$Equipo_result = db_query
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
					j.{ id_Jugador } = %d',
			INVITADO,
			$jugador->id_Jugador
		);


		
		
		if( $Tiene_Equipo == 1 )
		{

			while( $Equipos = db_fetch_object( $Equipo_result ) )
			{
				
				$form[ 'invitaciones' ][ 'abreviacion' ] = array (
					'#type' => 'item',
					'#value' => $Equipos->abreviacion,
				);
	
			}

		}
		else
		{	
			
			$form[ 'invitaciones' ][ 'nid_Equipo' ] = array (
				'#type' => 'checkbox',
				'#title' => t( 'Invitaciones.' ),
				'#options' => $Invitaciones_Equipos,
			);
	
			$form[ 'invitaciones' ][ 'submit' ] = array (
				'#type' => 'submit',
				'#value' => t( 'Aceptar' ),
			);
	
			$form[ 'invitaciones' ][ 'id_Jugador' ] = array (
				'#type' => 'hidden',
				'#value' => $jugador
			);
	
	
			$form[ 'invitaciones' ][ 'rechazar' ] = array (
				'#value' => l( t( 'Rechazar' ), 'user/'.arg( 1 ).'rechazar_invitacion' ),
			);
	


		}

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
            db_set_active('eSM');
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


    $nid_Equipo = $form_state['values'][ 'nid_Equipo' ];

    $id_Jugador = $form_state['values']['id_Jugador'];
    
    db_set_active('eSM');
        db_query('INSERT INTO {Integrantes_Equipo} (nid_Equipo, id_Jugador, Estado, Mensaje) VALUES (%d, %d, %d, "%s")', $nid_Equipo, $id_Jugador, ACEPTADO, 'Acepada la invitación' );
    db_set_active('default');
    
    dsm( t( 'Tu unión se ha enviado correctamente.') );
			$modalidad = get_modalidad( $equipo->id_Modalidad_Juego_Temporada );
    drupal_goto( '/libres/'.$modalidad->tid.'/lista' );
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
	
    $nid_Equipo = $form_state[ 'values' ][ 'nid_Equipo' ];

    $id_Jugador = $form_state[ 'values' ][ 'id_Jugador'];
    
    db_set_active( 'eSM' );
        db_query( 'INSERT INTO { Integrantes_Equipo } ( nid_Equipo, id_Jugador, Estado, Mensaje ) VALUES ( %d, %d, %d, "%s" )', $nid_Equipo, $id_Jugador, RECHAZADO, 'Rechazada la invitación' );
    db_set_active( 'default' );
    
    dsm( t( 'Tu unión se ha enviado correctamente.' ) );
			$modalidad = get_modalidad( $equipo->id_Modalidad_Juego_Temporada );
    drupal_goto( '/libres/'.$modalidad->tid.'/lista' );

	
	}
	
		
		