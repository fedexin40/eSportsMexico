<?php

/**
 * Archivo para desplegar las formas que controlan la inscripción a las diferentes modalidades
 * la función de inscripción es la principal que revisa si la modalidad actual es válida en
 * la temporada activa, en caso de que no esté activa despliega el mensaje y regresa a la vista de la modalidad
 * Si la modalidad actual es válida en la temporada activa, revisa si es individual o por equipos
 * y despliega las opciones correspondintes.

/**
 * función que revisa el estado de la modalidad que se está visitando y si es válida en la temporada activa
 * despliega las opciones de acuerdo al máximo de jugadores para los equipos.
 */
function inscripcion( ) {
  
  module_load_include( 'php', 'base_datos_externa', 'db_functions' );
                $tid = arg(1);  //Obtiene el tid de la modalidad de la barra de direcciones
                global $user;
                $tiene_equipo = tiene_equipo($user->uid, $modalidad->id_Modalidad_Juego_Temporada);
                
                if (!($modalidad = term_is_active($tid))) {
                        drupal_set_message( 'Esta modalidad no esta activa en esta temporada.' );				
                        drupal_goto ('taxonomy/term/'.$tid);    //Regresa a la vista de noticias de la modalidad
                }
                else{                        
                        if (($tiene_equipo == AGENTE_LIBRE) || ($tiene_equipo == NO_REGISTRADO)) {
                                if ($modalidad->Maximo_Jugadores == 1){
                                                drupal_goto('node/add/team/'. $modalidad->id_Modalidad_Juego_Temporada);
                                }
                                else {
                                                drupal_goto('inscripcion/multijugador/'. $modalidad->id_Modalidad_Juego_Temporada);
                                }
                        }
                        else {
                                drupal_set_message('Ya perteneces a un equipo de esta modalidad. Para poder inscribirte en otro equipo debes salir primero del equipo al que estás inscrito actualmente.');
                                drupal_goto('taxonomy/term/'. $tid);
                        }
                }
}

/**
* forma para presentar opciones de inscripción a las modalidades por equipos
*/
function team_inscripcion( )
{

  module_load_include( 'php', 'base_datos_externa', 'db_functions' );
                
  $id_Modalidad_Juego_Temporada = arg( 2 );  //Obtiene el id_Modalidad_Juego_Temporada que le pasa la función de inscripción como tercer argumento en la barra de direcciones

  $modalidad = get_modalidad_juego_temporada( $id_Modalidad_Juego_Temporada );
  
  $teams = get_teams( $id_Modalidad_Juego_Temporada );
  
  
  $count = 0;
  //Primero almacenaremos todos los equipos en nodos

  while ( $team = db_fetch_object( $teams ) )
  {
    $team = node_load( array( 'nid' => $team->nid_Equipo) );
    if ( $team->Numero_Integrantes < $modalidad->Maximo_Jugadores )
    {
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
        '#value' => $team->uid,
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


  
  $form[ 'crear_equipo' ] = array (
    '#value' => '<div id="crear">'. l( t( 'Crear equipo  ' ), 'node/add/team/'.$id_Modalidad_Juego_Temporada ) .'</div>',
  );
  
  $form[ 'agente_libre' ] = array (
    '#value' => '<div id = "agente_libre">'. l( t( '  Ser agente libre' ), 'libres/'.$id_Modalidad_Juego_Temporada ) .'</div>',
  );
  
  $form['#theme'] = 'team_inscripcion';
  
  return $form;
}