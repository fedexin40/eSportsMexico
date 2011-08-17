<?php
/**
 * Función para ver que torneos están próximos a empezar y crear los brackets para los equipos inscritos
 */
function _torneo_init_brackets () {
  //Selecciona todos los torneos para los que todavía no se han creado brackets
  db_set_active ('eSM');
    $result = db_query('SELECT * FROM {Torneos} WHERE Estado = %d', FUTURO);
  db_set_active ('default');
  
  $fecha_actual = strtotime( date( "Y-m-d H:i")) + 86400; //86400 segundos equivalen a 24 horas antes del inicio del torneo se crean los brackets
  
  while ($torneo = db_fetch_object($result)) {
    if ($fecha_actual >= $torneo->Fecha_Inicio) {
      _torneo_create_brackets($torneo->nid_Torneo);
      _torneo_update_estado($torneo->nid_Torneo, PRESENTE);
    }
  }
}
/**
 * Fuction to generate the brackets
 *
 * @param 
 *   nid_Torneo
 */
function _torneo_create_brackets($nid_Torneo) {
  //Obtiene las características del torneo
  $settings = get_settings_torneo($nid_Torneo);
  
  //Obtiene el total de participantes en el torneo.
  $numTeams = torneo_get_total_participantes($nid_Torneo);
  
  if ($numTeams < 2 ) {     //El número de equipos participantes es menor a dos por lo tanto no se puede llevar a cabo el torneo
    watchdog('error', t('Se ha intentado crear los brackets para un torneo con solo un equipo inscrito.')); //muestra un mensaje de error en el watchdog
  }
  else {
    //Obtiene la lista de los equipos participantes en el torneo
    $teams = torneo_get_participantes($nid_Torneo);
    while ($team = db_fetch_array($teams)) {
      $participantes[$team['nid_Equipo']] = $team;
    }

    //Obtiene la tabla de posiciones de los equipos en el ranking
    $ranking = ranking_get_ranking($settings->id_Modalidad_Juego_Temporada);

    $ids[] = array();
    $n=0;
    while ($team =  db_fetch_object($ranking)) {
      if (is_array($participantes[$team->nid_Equipo])) {   //Verifica que el equipo esté inscrito al torneo
        $ids[$n] = $team->nid_Equipo;
        $n++;
      }
    }

    // find next power of 2 ($bracketSize)
    $bracketSize = 2;
    $numrounds = 2;
    while($numTeams > $bracketSize) {
      $numrounds++;
      $bracketSize = $bracketSize * 2;
    }
    $numByes = $bracketSize - $numTeams;
    // insert byes into pull
    for($i=0; $i<$numByes ; $i++ ) {
      $ids[$n] = 1;
      $n++;
    }
    // Ordena los equipos en el arreglo de acuerdo al criterio de emparejamiento ponderado que se usa en la liga
    $bracket_list = $ids;
    $slice = 1;

    while ($slice < (count($bracket_list) / 2)) {

      $temp = $bracket_list;
      $bracket_list = array ();

      $count = count($temp);

      for ($i = 0; $i < ($count / $slice / 2); $i++) {
        $bracket_list = array_merge($bracket_list, array_slice ($temp, ($i * $slice), $slice));
        $bracket_list = array_merge($bracket_list, array_slice ($temp, -($slice + $i * $slice), $slice));
      }

      $slice = $slice * 2;
    }
    $ids = array ();
    $ids = $bracket_list;
    //Inserta los brackets ordenados en la base de datos
    for ($n=0; $n<count($ids); $n++) {
      db_set_active('eSM');
        db_query("INSERT INTO {Torneos_Brackets} (nid_Torneo, nid_Equipo, Ronda, Posicion)
                 VALUES (%d, %d, %d, %d)",
                 $nid_Torneo, $ids[$n], 1, ($n+1));
      db_set_active('deafult');
    }
    // promote byes through bracket
    for ($round=1; $round < $numrounds; $round++) {
      for ($y=0 ; $y<=$bracketSize ; $y++) {
        $team1 = _torneo_get_bracket_info($nid_Torneo, $round, $y);
        
        if($team1['nid_Equipo'] == 1) {
          // get winning team info
          if($y&1) { //is odd
            $team2 = _torneo_get_bracket_info($nid_Torneo, $round, ($y + 1));
            $y++;
          }
          else {//is even
            $team2 = _torneo_get_bracket_info($nid_Torneo, $round, ($y - 1));
          }			
          // update database with winner
          db_set_active('eSM');
            db_query("INSERT INTO {Torneos_Brackets} (nid_Torneo, nid_Equipo, Ronda, Posicion)
                     VALUES (%d, %d, %d, %d)",
                     $nid_Torneo, $team2['nid_Equip'], ($round+1), ceil( (float)$y / 2.0 ));
          db_set_active('default');
        }
      }
    }
    
    //Crea los partidos correspondientes si está habilitado el módulo match
    if (module_exists('match')) {
      _torneo_create_matches($nid_Torneo);
    }
  }
}
/**
 * Función para finalizar los torneos
 */
function _torneo_ending_event() {
  
}