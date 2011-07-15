<?php

/**
 *  Función para administrar a los equipos
 */
function team_administrar(  )
{
  $form[ 'informacion' ] = array (
    '#type' => 'item',
    '#disabled' => FALSE,
    '#value' => 'Panel de administración de los equipos',
  );		

  return $form;
  
}


/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_activos( )
{
  $equipo = node_load( arg( 1 ) );  //carga el objeto nodo o equipo
                
  db_set_active( 'eSM' );  //Carga todos los jugadores activos en el equipo.
    $jugadores = db_query
    (
      'SELECT
          *
        FROM
          { Jugador }
        AS
          j
      INNER JOIN
          { Integrantes_Equipo }
        AS
          ie
        ON
          j.{ id_Jugador } = ie.{ id_Jugador }
      INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d
        AND
          ie.{ Estado } = %d',
      $equipo->nid,
      ACEPTADO
    );

  db_set_active('default');
  
  //Despliega los jugadores 
  while ( $jugador = db_fetch_object( $jugadores ) )
  {
    $account = user_load( array( 'uid' => $jugador->uid ) );

    $form[ $jugador->uid ][ 'nombre' ] = array (
      '#value' => $account->name,
    );

    $form[ $jugador->uid ][ 'check' ] = array (
      '#type' => 'checkbox',
    );

  }

  $form[ 'submit' ] = array (
    '#type' => 'submit',
    '#value' => t( 'Expulsar jugadores seleccionados.' ),
  );

  $form[ 'nid_equipo' ] = array (
    '#type' => 'hidden',
    '#value' => $equipo->nid,
  );

  return $form;

}


/**
 * Submit para la la forma de jugadores activos
 */
function team_administrar_activos_submit( $form, $form_state )
{
  global $user;

  $nid_Equipo = $form_state[ 'nid_equipo' ];
  $Equipo = node_load( $nid_Equipo );

  $Jugadores = db_query
  (
    'SELECT
        *
      FROM
        { Jugador }
      AS
        j
    INNER JOIN
        { Integrantes_Equipo }
      AS
        ie
      ON
        j.{ id_Jugador } = ie.{ id_Jugador }
    INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d',
    $nid_Equipo
  );

  while( $Jugador = db_fetch_object( $Jugadores ) )
  {
    db_query
    (
      'INSERT INTO
          { Integrantes_Equipo }
          ( nid_Equipo, id_Jugador, Estado )
        VALUES
          ( %d, %d, %d )',
        $nid_Equipo,
        $jugador->id_Jugador,
        RECHAZADO
    );
  }

  dsm( "Jugadores expulsados satisfactoriamente" );
  drupal_goto( 'node/'.$nid_Equipo );
  
}

/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_solicitudes( ) {
  $equipo = node_load( arg( 1 ) );  //carga el objeto nodo o equipo
  
  db_set_active('eSM');  //Carga todos los jugadores que solicitaron entrar en el equipo.
    $jugadores = db_query
    ( 'SELECT
          *
        FROM
          { Jugador }
        AS
          j
      INNER JOIN
          { Integrantes_Equipo }
        AS
          ie
        ON
          j.{ id_Jugador } = ie.{ id_Jugador }
      INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d
        AND
          ie.{ Estado } = %d',
      $equipo->nid,
      SOLICITA
    );

  db_set_active('default');
  
  //Despliega los jugadores 
  while ( $jugador = db_fetch_object( $jugadores ) )
  {

    $account = user_load( array( 'uid' => $jugador->uid ) );
    $form[ $jugador->uid ][ 'nombre' ] = array (
      '#value' => $account->name,
    );
    $form[ $jugador->uid ][ 'check' ] = array (
      '#type' => 'checkbox',
    );

  }


  $form[ 'submit' ] = array (
    '#type' => 'submit',
    '#value' => t( 'Aceptar Jugadores Seleccionados.' ),
  );

  $form[ 'nid_equipo' ] = array (
    '#type' => 'hidden',
    '#value' => $equipo->nid,
  );
  
}



/**
 * Submit para la la forma de jugadores solicitantes
 */
function team_administrar_solicitudes_submit( $form, $form_state )
{
  global $user;

  $nid_Equipo = $form_state[ 'nid_equipo' ];
  $Equipo = node_load( $nid_Equipo );

  $Jugadores = db_query
  (
    'SELECT
        *
      FROM
        { Jugador }
      AS
        j
    INNER JOIN
        { Integrantes_Equipo }
      AS
        ie
      ON
        j.{ id_Jugador } = ie.{ id_Jugador }
    INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d',
    $nid_Equipo
  );

  while( $Jugador = db_fetch_object( $Jugadores ) )
  {
    db_query
    (
      'INSERT INTO
          { Integrantes_Equipo }
          ( nid_Equipo, id_Jugador, Estado )
        VALUES
          ( %d, %d, %d )',
        $nid_Equipo,
        $jugador->id_Jugador,
        ACEPTADO
    );
  }

  dsm( "Jugadores aceptados satisfactoriamente" );
  drupal_goto( 'node/'.$nid_Equipo );
  
}



/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_invitados( ) {
  $equipo = node_load( arg( 1 ) );  //carga el objeto nodo o equipo
  
  db_set_active( 'eSM' );  //Carga todos los jugadores invitados en el equipo.
    $jugadores = db_query
    ( 'SELECT
          *
        FROM
          { Jugador }
        AS
          j
      INNER JOIN
          { Integrantes_Equipo }
        AS
          ie
        ON
          j.{ id_Jugador } = ie.{ id_Jugador }
      INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d
        AND
          ie.{ Estado } = %d',
      $equipo->nid,
      INVITADO
    );

  db_set_active( 'default' );
  
  //Despliega los jugadores 
  while ( $jugador = db_fetch_object( $jugadores ) )
  {
    $account = user_load( array( 'uid' => $jugador->uid ) );
    $form[ $jugador->uid ][ 'nombre' ] = array (
      '#value' => $account->name,
    );
    $form[ $jugador->uid ][ 'check' ] = array (
      '#type' => 'checkbox',
    );
  }

  $form[ 'submit' ] = array (
    '#type' => 'submit',
    '#value' => t( 'Rechazar a los jugadores seleccionados.' ),
  );

  $form[ 'nid_equipo' ] = array (
    '#type' => 'hidden',
    '#value' => $equipo->nid,
  );


}


/**
 * Submit para la la forma de jugadores solicitantes
 */
/*function team_administrar_invitados_submit( $form, $form_state )
{
  global $user;

  $nid_Equipo = $form_state[ 'nid_equipo' ];
  $Equipo = node_load( $nid_Equipo );

  $Jugadores = db_query
  (
    'SELECT
        *
      FROM
        { Jugador }
      AS
        j
    INNER JOIN
        { Integrantes_Equipo }
      AS
        ie
      ON
        j.{ id_Jugador } = ie.{ id_Jugador }
    INNER JOIN
        { Equipo }
        AS
          e
        ON
          ie.{ nid_Equipo } = e.{ nid_Equipo }
      WHERE
          e.{ nid_Equipo } = %d',
    $nid_Equipo
  );

  while( $Jugador = db_fetch_object( $Jugadores ) )
  {
    db_query
    (
      'INSERT INTO
          { Integrantes_Equipo }
          ( nid_Equipo, id_Jugador, Estado )
        VALUES
          ( %d, %d, %d )',
        $nid_Equipo,
        $jugador->id_Jugador,
        INVITADO
    );
  }

  dsm( "Jugadores invitados satisfactoriamente" );
  drupal_goto( 'node/'.$nid_Equipo );
  
}*/
