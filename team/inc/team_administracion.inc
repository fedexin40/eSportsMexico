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


  db_set_active('eSM');
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
  db_set_active('default');  
  
  drupal_set_message( "Jugadores expulsados satisfactoriamente" );
  drupal_goto( 'node/'.$Equipo->nid_Equipo );
  
}

/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_solicitudes( ) {
  $equipo = node_load(arg(1));  //carga el objeto nodo o equipo
  
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
  
  $count = 0;
  $checkboxes = array ();
  
  //Despliega los jugadores 
  while ($jugador = db_fetch_object( $jugadores ) )
  {
    ++ $count;
    $account = user_load( array( 'uid' => $jugador->uid ) );
    $form['Solicitudes'][ $jugador->id_Jugador ][ 'nombre' ] = array (
      '#value' => $account->name,
    );
  
    $form['Solicitudes'][ $jugador->id_Jugador ][ 'mensaje' ] = array (
      '#value' => $jugador->Mensaje,
    );
    
    $checkboxes[$jugador->id_Jugador] = '';
    
    /*$form[ $account->uid ][ 'check' ] = array (
      '#type' => 'checkbox',
    );*/

  }
  
  if ($count == 0) {
    $form['mensaje'] = array (
      '#value' => t('No hay solicitudes nuevas en este momento.'),
    );
  }
  else {
    $form[ 'checkboxes' ] = array (
      '#type' => 'checkboxes',
      '#options' => $checkboxes,
    );
    $form[ 'submit' ] = array (
      '#type' => 'submit',
      '#value' => t( 'Aceptar Jugadores Seleccionados.' ),
    );
  
    $form[ 'nid_equipo' ] = array (
      '#type' => 'hidden',
      '#value' => $equipo->nid,
    );
  }
  
  $form['#theme'] = 'team_solicitudes';
  
  return $form;
  
}



/**
 * Submit para la la forma de jugadores solicitantes
 */
function team_administrar_solicitudes_submit( $form, $form_state )
{
  global $user;
  
  $nid_Equipo = $form_state['values'][ 'nid_equipo' ];
  $count = 0; //Contador para mostrar el número de solicitudes aceptados

  foreach( element_children($form_state['values']['checkboxes']) as $jugador )
  { 
    if ($form_state['values']['checkboxes'][$jugador] != 0) {
      ++$count;
      db_set_active('eSM');
        db_query                                //Actualiza el estado del jugador en la tabla de los integrantes del equipo
        (
          'UPDATE
              { Integrantes_Equipo }
            SET
              Estado = %d
            WHERE 
              nid_Equipo = %d AND id_Jugador = %d',
            ACEPTADO,
            $nid_Equipo,
            $jugador
        );
        
        db_query (
            'UPDATE
                { Jugador }
            SET
                Estado = %d
            WHERE
                id_Jugador = %d',
            TIENE_EQUIPO,
            $jugador
        );
      db_set_active('default');
    }
  }

  if ($count > 0) {
    drupal_set_message( "Has aprobado ". $count ." solicitudes." );
    drupal_goto( 'node/'. $nid_Equipo .'/administrar/activos');
  }
  else {
    drupal_set_message('No has seleccionado ninguna solicitud para aprovar.', 'error');
    //drupal_goto( 'node/'. $nid_Equipo .'/administrar/activos');
  }
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
  }

  $form[ 'nid_equipo' ] = array (
    '#type' => 'hidden',
    '#value' => $equipo->nid,
  );
  
  return $form;

}