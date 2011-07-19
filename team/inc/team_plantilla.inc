<?php
/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_plantilla( )
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

    $form['jugadores_activos'][ $jugador->uid ][ 'nombre' ] = array (
      '#value' => l($account->name, 'user/'. $account->uid),
    );
    
    if ($account->uid == $equipo->uid) {
      $form['jugadores_activos'][ $jugador->uid ][ 'rol' ] = array (
        '#value' => t('Capitan'),
      );
    }
    else {
      $form['jugadores_activos'][ $jugador->uid ][ 'rol' ] = array (
        '#value' => t('Jugador'),
      );
    }

  }

  return $form;

}