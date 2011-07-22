<?php

/**
 * Función para confirmar que el usuario quiere solicitar su ingreso a un equipo.
 */
function team_solicitud( )//&$form_state )
{

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
  
  /*
  return confirm_form
  (
    $form,
    t( '¿Quieres solicitar inscribirte a este equipo?' ), //message title
    ( isset( $_GET[ 'destination' ] ) ? $_GET[ 'destination' ] : 'user' ),//redirect if cancelled
    //t( '' ), //message description
    t( '¿Estás seguro?' ),
    t( 'Si' ), //confirm button text
    t( 'No' ) //cancel button text
  );*/


}


/**
  * Validate handler for previus confirm form
  */
function team_solicitud_validate( $form, &$form_state )
{

    //Verifica que el usuario no esté activo en otro equipo
    global $user;



    $team = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );       //Carga el objeto del equipo para poder utilizarlo

    $modalidad = get_modalidad( $team->id_Modalidad_Juego_Temporada );

    db_set_active( 'eSM' );

    $equipo_jugador = db_fetch_object
    (
      db_query
      (
        'SELECT
            *
          FROM
            { Integrantes_Equipo }
          AS
            ie
        INNER JOIN
            { Jugador }
          AS
            j
          ON
            ie.{ id_Jugador } = j.{ id_Jugador }
         WHERE
            j.{ uid } = %d
          AND
            ie.{ Estado } = %d',
        $user->uid,
        ACEPTADO
      )
    );
    
    db_set_active( 'default' );
  
  
    if ( $equipo_jugador != NULL )
    {

      //El jugador ya está activo en un equipo. Salir
      drupal_set_message( 'No te puedes unir a este equipo porque ya estás activo en otro equipo.', 'error' );
      drupal_goto( 'taxonomy/term/'.$modalidad->tid );

    }


}


/**
* Submit handler for previus confirm form
*/
function team_solicitud_submit( $form, &$form_state )
{ 
    global $user;
    
    //Revisa si el jugador ya está registrado en la tabla de jugadores activos para esta temporada, sino lo agrega
    $team = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );
    
    $modalidad = get_modalidad( $team->id_Modalidad_Juego_Temporada ); //Obtiene los datos de la modalidad para poder redireccionar la forma

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
    
    print_r($jugador);
  
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
      db_query ('INSERT INTO { Integrantes_Equipo } ( nid_Equipo, id_Jugador, Estado, Mensaje ) VALUES ( %d, %d, %d, "%s" )', $team->nid, $jugador->id_Jugador, SOLICITA, $form_state['values']['mensaje']);
    db_set_active('default');
    
  
  //set a message
  drupal_set_message( '¡Tu solicitud ha sido enviada con éxito!' );
  drupal_goto( '/taxonomy/term/'.$modalidad->tid );


}


