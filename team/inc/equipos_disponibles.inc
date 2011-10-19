<?php
/**
 * Archivo para desplegar las formas que controlan la inscripción a las diferentes modalidades
 * la función de inscripción es la principal que revisa si la modalidad actual es válida en
 * la temporada activa, en caso de que no esté activa despliega el mensaje y regresa a la vista de la modalidad
 * Si la modalidad actual es válida en la temporada activa, revisa si es individual o por equipos
 * y despliega las opciones correspondintes.
 **  TODO campo imagen columna
 * forma para presentar opciones de inscripción a las modalidades por equipos
*/
function equipos_disponibles( ) {
  $tid = arg(2);
  $modalidad = term_is_active($tid);
  drupal_set_title(t('Teams participating in '. $modalidad->Nombre));
  
  module_load_include( 'php', 'team', 'db_functions' );
  $teams = get_equipos( $modalidad->id_Modalidad_Juego_Temporada );
  
  $count = 0;
  foreach ( $teams AS $index => $team ) {
    $team = node_load( array( 'nid' => $team['nid_Equipo']) );
    //Presentaremos una forma para cada equipo disponible
    $form[ 'equipos_disponibles' ][ $team->title ][ 'imagen' ] = array (
      '#value' => theme('team_pane', $team, variable_get('team_block_roster_picture_preset', '')),
    );
    $form[ 'equipos_disponibles' ][ $team->title ][ 'nombre' ] = array (
      '#value' => l($team->title, 'node/'. $team->nid),
    );
    $form[ 'equipos_disponibles' ][ $team->title ][ 'tag' ] = array (
      '#value' => $team->abreviacion,
    );
    if ($team->Numero_Intergrantes < $modalidad->Maximo_Jugadores) {
      $form[ 'equipos_disponibles' ][ $team->title ][ 'recruit' ] = array (
        '#value' => t('Yes'),
      );
    }
    else {
      $form[ 'equipos_disponibles' ][ $team->title ][ 'recruit' ] = array (
        '#value' => t('No'),
      );
    }
    
    //Si está activo el módulo de ránking ponemos los datos de victorias y derrotas
    if(module_exists('ranking')) {
      $puntuacion = _ranking_get_team_puntuacion($team->nid);
      $form[ 'equipos_disponibles' ][ $team->title ][ 'win' ] = array (
        '#value' => $puntuacion->PG,
      );
      $form[ 'equipos_disponibles' ][ $team->title ][ 'lose' ] = array (
        '#value' => ($puntuacion->PJ - $puntuacion->PG),
      );
    }
    
    //Si está activo el módulo de matches ponemos la reputación del equipo
    if(module_exists('match')) {
      $form[ 'equipos_disponibles' ][ $team->title ][ 'reputation' ] = array (
        '#value' => _match_display_reputation($team->nid),
      );
    }
    $count++;
  }
  
  if( $count == 0 ) {
    $form[ 'equipos_disponibles' ] = array (
      '#value' => 'No hay inscritos en esta modalidad todavia',
    );
  }
  
  $form['#theme'] = 'team_inscripcion';
  
  return $form;
}