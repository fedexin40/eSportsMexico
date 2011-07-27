<?php
/**
 * Función para mostrar la vista de los participantes y equipos registrados en un torneo
 */
function torneo_signup_pendientes () {
  $torneo = get_settings_torneo(arg(1));                                 //Obtiene las configuraciones del torneo del nid obtenido en la barra de dirección
  $modalidad = get_modalidad($torneo->id_Modalidad_Juego_Temporada);    //Obtiene los datos de la modalidad
  
  if ($modalidad->Minimo_Jugadores > 1) {         //Solo se muestra para las modalidades por equipo de más de 1 jugador
    //Obtiene la lista de equipos inscritos al ránking
    $equipos = get_teams($modalidad->id_Modalidad_Juego_Temporada);
    $equipos_inscritos = 0;
    
    while ($equipo = db_fetch_object($equipos)) {
      
      $plantilla = _team_get_plantilla($equipo->nid_Equipo);    //Obtiene la plantilla de jugadores en un equipo
      
      $inscritos = 0; //Inicializa un contador de jugadores inscritos
      while ($jugador = db_fetch_object($plantilla)) {
        if (_torneo_signup_esta_inscrito($jugador->uid, $torneo->nid_Torneo))
          ++$inscritos;
      }
      
      if ($inscritos < $modalidad->Minimo_Jugadores) {
        $form['pendientes'][$equipo->nid_Equipo]['Nombre'] = array (
          '#value' => l($equipo->Nombre, 'node/'. $equipo->nid_Equipo),
        );
        $form['pendientes'][$equipo->nid_Equipo]['Inscritos'] = array (
          '#value' => $inscritos,
        );
        ++$equipos_inscritos;
      }
    }
    if ($equipos_inscritos == 0 ) {
      $form['mensaje'] = array (
        '#value' => t('No se han inscrito equipos a este torneo todavia o todos los equipos estan completos.'),
      );
    }
  }
  else {
    $form ['mensaje'] = array (
      '#value' => t('Modalidad Individiual'),
    );
  }
  
  return $form;
}