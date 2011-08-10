<?php

  /**
   * Archivo para desplegar las formas que controlan la inscripción a las diferentes modalidades
   * la función de inscripción es la principal que revisa si la modalidad actual es válida en
   * la temporada activa, en caso de que no esté activa despliega el mensaje y regresa a la vista de la modalidad
   * Si la modalidad actual es válida en la temporada activa, revisa si es individual o por equipos
   * y despliega las opciones correspondintes.

/**  TODO campo imagen columna
* forma para presentar opciones de inscripción a las modalidades por equipos
*/
function equipos_disponibles_listar( ) {

  module_load_include( 'php', 'base_datos_externa', 'db_functions' );
                
  $tid = arg( 1 );  

  $modalidad = term_is_active($tid);
  
  module_load_include( 'php', 'team', 'db_functions' );
  $teams = get_equipos( $modalidad->id_Modalidad_Juego_Temporada );
  
  $count = 0;
  //Primero almacenaremos todos los equipos en nodos
  
  foreach ( $teams AS $index => $team )
  {
    $team = node_load( array( 'nid' => $team['nid_Equipo']) );
    if ( $team->Numero_Integrantes < $modalidad->Maximo_Jugadores )
    {
      $capitan = user_load(array('uid' => $team->uid));
      //Presentaremos una forma para cada equipo disponible
      $form[ 'equipos_disponibles' ][ $team->title ][ 'imagen' ] = array (
        '#value' => $team->imagen,
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'nombre' ] = array (
        '#value' => $team->title,
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'tag' ] = array (
        '#value' => $team->abreviacion,
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'capitan' ] = array (
        '#value' => l($capitan->name, 'user/'. $capitan->uid),
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'plazas' ] = array (
        '#value' => $modalidad->Maximo_Jugadores - $team->Numero_Integrantes,
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'link' ] = array (
        '#value' => l( t( 'Solicitar Unirme' ), 'node/'.$team->nid.'/solicitar' ),
      );
    }
    $count++;
  }
  
  if( $count == 0 )
  {
    $form[ 'equipos_disponibles' ] = array
    (
      '#value' => 'No hay equipos disponibles',
    );
  }
  
  $form['#theme'] = 'team_inscripcion';
  
  return $form;
}