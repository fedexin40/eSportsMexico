<?php

/**
 * Función para confirmar que el usuario quiere solicitar su ingreso a un equipo.
 */
function team_solicitud() {
  //Pasando las variables necesarias para procesar después de confirmar la acción
  $form[ 'nid_Equipo' ] = array
  (
    '#type' => 'value',
    '#value' => arg( 1 ),  //Agrega a la forma el nid del equipo que obtenemos de la barra de dirección
  );
  
    $form['Mensaje'] = array (
      '#type' => 'textarea',
      '#title' => t('Escribe el texto de tu solicitud para unirte este equipo.'),
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
function team_solicitud_validate( $form, &$form_state )
{

    module_load_include( 'php', 'base_datos_externa', 'db_functions' );

    //Verifica que el usuario no esté activo en otro equipo
    global $user;

    $team = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );       //Carga el objeto del equipo para poder utilizarlo

    $modalidad = get_modalidad_juego_temporada( $team->id_Modalidad_Juego_Temporada );

    db_set_active( 'eSM' );

    $equipo_jugador = tiene_equipo($user->uid, $team->id_Modalidad_Juego_Temporada);
	
    if ( $equipo_jugador == TIENE_EQUIPO ) {

      //El jugador ya está activo en un equipo.
      form_set_error('nid_Equipo', t('No te puedes unir a este equipo porque ya estás activo en otro equipo.'));

    }
    
    db_set_active( 'eSM' );
	  $solicitud_realizada = db_fetch_object(db_query('SELECT * FROM { Integrantes_Equipo } AS ie INNER JOIN { Jugador } AS j ON ie.{ id_Jugador } = j.{ id_Jugador }
													  WHERE j.{ uid } = %d AND ie.{ Estado } = %d AND ie. { nid_Equipo } = %d',
        $user->uid, SOLICITA, $team->nid));
    db_set_active( 'default' );

  if (is_object($solicitud_realizada)) {
    form_set_error('nid_Equipo', t('Ya has solicitado unirte a este equipo.'));
  }

}


/**
* Submit handler for previus confirm form
*/
function team_solicitud_submit( $form, &$form_state )
{
  
    module_load_include( 'php', 'base_datos_externa', 'db_functions' );
    
    global $user;
    
    //Revisa si el jugador ya está registrado en la tabla de jugadores activos para esta temporada, sino lo agrega
    $team = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );
    
    $modalidad = get_modalidad_juego_temporada( $team->id_Modalidad_Juego_Temporada ); //Obtiene los datos de la modalidad para poder redireccionar la forma

    db_set_active( 'eSM' );
      $jugador = db_fetch_object
      (
        db_query
        (
          'SELECT
              *
            FROM
              { Jugador }
            WHERE
              { id_Modalidad_Juego_Temporada } = %d
            AND
              uid = %d',
          $team->id_Modalidad_Juego_Temporada,
          $user->uid
        )
      );

    db_set_active( 'default' );
  
    if( $jugador->uid == 0 )
    {
      //El jugador no se había registrado antes, crear un id de jugador y cargarlo en el objeto jugador
      db_set_active('eSM');
        db_query
        ( 
            "INSERT INTO { Jugador }
            ( 
                id_Modalidad_Juego_Temporada,
                uid,
                Fecha_Registro,
                Estado
            )			
            VALUES ( %d, %d, %d, %d )",
                $team->id_Modalidad_Juego_Temporada, $user->uid, string_to_timestamp( date( 'Y-m-d' ) ), PENDIENTE
        );

        $jugador = db_fetch_object
        (
          db_query
          (
            'SELECT
                *
              FROM
                { Jugador }
            WHERE
                { id_Modalidad_Juego_Temporada } = %d
              AND uid = %d',
            $team->id_Modalidad_Juego_Temporada,
            $user->uid
          )
        );
      db_set_active( 'default' );
    }
    
    //Insertar el registro de solicitud en la base de datos y actualizar el estado del jugador
    db_set_active( 'eSM' );
      db_query ('INSERT INTO { Integrantes_Equipo } ( nid_Equipo, id_Jugador, Estado, Mensaje ) VALUES ( %d, %d, %d, "%s" )', $team->nid, $jugador->id_Jugador, SOLICITA, $form_state['values']['Mensaje']);
    db_set_active('default');
  
  //Notifica al capitán del equipo utilizando el módulo rules para disparar una acción configurable
  rules_invoke_event('solicitud_usuario', $user, user_load(array('uid' => $team->uid)), $team);
  
  //set a message
  drupal_set_message( '¡Tu solicitud ha sido enviada con éxito!' );
  drupal_goto( 'node/'. $team->nid);


}


