<?php
/**
 * Función para actualizar los equipos registrados para un torneo
 */ 
function _torneo_signup_update_equipos () {
  //Selecciona todos los torneos para los que todavía no se han creado brackets
  $result = db_query('SELECT * FROM {eSM_Torneos} WHERE Estado = %d', FUTURO);
    
  while ($torneo = db_fetch_object($result)) {
    
    $modalidad = get_modalidad($torneo->id_Modalidad_Juego_Temporada);    //Obtiene los datos de la modalidad

    //Obtiene la lista de equipos inscritos al ránking
    $equipos = get_teams($modalidad->id_Modalidad_Juego_Temporada);    
      
    while ($equipo = db_fetch_object($equipos)) {      
      $plantilla = _team_get_plantilla($equipo->nid_Equipo);    //Obtiene la plantilla de jugadores en un equipo
      
      $inscritos = 0; //Inicializa un contador de jugadores inscritos
      
      while ($jugador = db_fetch_object($plantilla)) {
        if (_torneo_signup_esta_inscrito($jugador->uid, $torneo->nid_Torneo))
          ++$inscritos;
      }
      
      if ($inscritos >= $modalidad->Minimo_Jugadores) {
        if (!(_torneo_signup_check_team_signup($equipo->nid_Equipo, $torneo->nid_Torneo))) {
          _torneo_signup_register_team($equipo->nid_Equipo, $torneo->nid_Torneo);
        }
      }
    }
  }
}

