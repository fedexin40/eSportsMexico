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
  $tid = arg( 2 );  //Obtiene el tid de la modalidad de la barra de direcciones
  global $user;
  
  $Modalidad_Juego_Temporada = get_modalidad_juego_temporada_tid_id_temporada( $tid, $Temporada_Activa->id_Temporada );
  
  $tiene_equipo = tiene_equipo( $user->uid, $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada );
  
  if ( !( $Modalidad_Juego = term_is_active( $tid ) ) ) {
    drupal_set_message( 'Esta modalidad no esta activa en esta temporada.' );
    drupal_goto ( 'taxonomy/term/'.$tid );    //Regresa a la vista de noticias de la modalidad
  }
  else {
    if ( ( $tiene_equipo == AGENTE_LIBRE ) || ( $tiene_equipo == NO_REGISTRADO ) ) {
        drupal_goto('node/add/team/'. $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada);
    }
    else {
      drupal_set_message( 'Ya perteneces a un equipo de esta modalidad. Para poder inscribirte en otro equipo debes salir primero del equipo al que estás inscrito actualmente.' );
      drupal_goto( 'taxonomy/term/'. $tid );
    }
  }
}