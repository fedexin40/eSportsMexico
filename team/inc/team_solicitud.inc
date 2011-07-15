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
  //Definiendo a donde se va a redirigir la forma
  //$form['#redirect'] = 'user';

  dsm( $form );
 

  dsm( "Hi" );
  
  return confirm_form
  (
    $form,
    t( '¿Quieres solicitar inscribirte a este equipo?' ), //message title
    ( isset( $_GET[ 'destination' ] ) ? $_GET[ 'destination' ] : 'user' )//redirect if cancelled
    //t( 'Debes estar seguro' ) //message description
    //t(  )
    //t( 'Si' ), //confirm button text
    //t( 'No' ) //cancel button text
  );


}

/**
* Submit handler for previus confirm form
*/
function team_solicitud_submit( $form, &$form_state )
{

  dsm( "Hi2" );

  dsm( $form_state );

  //verify that the form was confirmed
  if ( $form_state[ 'values' ][ 'confirm' ] )
  {

    //Do something here
    global $user;
    
    //Revisa si el jugador ya está registrado en la tabla de jugadores activos para esta temporada, sino lo agrega
    $team = node_load( $form_state[ 'values' ][ 'nid_Equipo' ] );
    
    $modalidad = get_modalidad( $team->$id_Modalidad_Juego_Equipo ); //Obtiene los datos de la modalidad para poder redireccionar la forma
    
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
    
    if( is_null( $jugador ) )
    {
      //El jugador no se había registrado antes, crear un id de jugador y cargarlo en el objeto jugador
      db_set_active('eSM');
        db_query
        (
          'INSERT INTO
              { Jugador }
              ( id_Modalidad_Juego_Temporada, uid, Fecha_Registro )
            VALUES
              ( %d, %d, %d )',
          $team->id_Modalidad_Juego_Temporada,
          $user->uid,
          convierte_a_timestamp( date( Y-m-d ) )
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
      db_query
      (
        'INSERT INTO
            { Integrantes_Equipos }
            ( nid_Equipo, id_Jugador, Estado )
          VALUES ( %d, %d, %d )',
        $team->nid,
        $jugador->id_Jugador,
        SOLICITA
      );
    db_set_active('default');
    
    //set a message
    drupal_set_message( '¡Tu solicitud ha sido enviada con éxito!' );
    drupal_goto( '/taxonomy/term/'. $modalidad->tid );
  }

}


/**
* Validate handler for previus confirm form
*/
function team_solicitud_validate( $form, &$form_state )
{
  dsm( "Hi3" );

  //verify that the form was confirmed
  if ( $form_state['values']['confirm'] )
  {
    //Verifica que el usuario no esté activo en otro equipo
    global $user;
    $team = node_load( $form_state['values']['nid_Equipo'] );       //Carga el objeto del equipo para poder utilizarlo
    $modalidad = get_modalidad( $team->id_Modalidad_Juego_Temporada );
    
    db_set_active( 'eSM' );

      $equipo_jugador = db_fetch_object
      ( db_query
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
           WHERE j.{ uid } = %d
            AND ie.{ Estado } = %d',
          $user->uid,
          ACEPTADO
        )
      );

    db_set_active( 'default' );
    
    if ( !is_null( $equipo_jugador ) )
    {
      //El jugador ya está activo en un equipo. Salir
      dpm( 'No te puedes unir a este equipo porque ya estás activo en otro euipo.', 'error' );
      drupal_goto( 'taxonomy/term/'. $modalidad->tid );
    }

  }

}