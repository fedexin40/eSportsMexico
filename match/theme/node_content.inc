<?php

function theme_match_content($node) {
  global $user;
  //Añade el archivo css donde viene la personalización de la vista del desafio
  drupal_add_css(drupal_get_path('module', 'match') . '/css/match.css');

  //Verifica si el usuario actual es capitán de alguno de los dos equipos
  if (team_user_is_team_capitan($user->uid, $node->nid_Equipo_Local) || $away_capitan = team_user_is_team_capitan($user->uid, $node->nid_Equipo_Visitante))
    $is_capitan = TRUE;

  //Obtiene los datos de las pruebas subidas por los jugadores
  $pruebas = match_get_pruebas($node->nid);
  
  $header = array();
  
  //Primera columa del contenido
  $rows['1'][] = array('data' => 'Competidores', 'class' => 'header', 'width' => '50%'); 
  $rows['1'][] = array('data' => 'Parámetros', 'class' => 'header', 'width' => '50%');

    $home_team = node_load($node->nid_Equipo_Local);
    $away_team = node_load($node->nid_Equipo_Visitante);
    
    
    $field = content_fields('field_logo', 'club');
    //Despliega los logos de los equipos y la palabra vs
    $content['2'][] = array('data' => '<a href="/node/'. $home_team->nid . '"'. theme('imagecache', 'match', (empty($home_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $home_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $home_team->title) . '</a>', 'width' => '45%', 'align' => 'right');
    $content['2'][] = ' VS. ';
    $content['2'][] = '<a href="/node/'. $away_team->nid . '"'. theme('imagecache', 'match', (empty($away_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $away_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $away_team->title) . '</a>';
    //Despliega los nombres de los equipos como link al perfil de los mismos
    $content['3'][] = '<a href="/node/'. $home_team->nid . '">'. $home_team->title . '</a>';
    $content['3'][] = ' ';
    $content['3'][] = '<a href="/node/'. $away_team->nid . '">'. $away_team->title . '</a>';
    //Despliega el ránking de los equipos
    $content['4'][] = 'Ranking';
    $content['4'][] = ' ';
    $content['4'][] = 'Ranking';
    $content['5'][] = ranking_get_ranking_equipo($home_team->nid, $node->id_Modalidad_Juego_Temporada);
    $content['5'][] = ' ';
    $content['5'][] = ranking_get_ranking_equipo($away_team->nid, $node->id_Modalidad_Juego_Temporada);
    //Despliega la puntuación de los equipos
    $content['6'][] = 'Puntos';
    $content['6'][] = ' ';
    $content['6'][] = 'Puntos';
    $content['7'][] = $node->Puntuacion_Inicial_Equipo_Local;
    $content['7'][] = ' ';
    $content['7'][] = $node->Puntuacion_Inicial_Equipo_Visitante;
    //Despliega la racha que tiene el equipo
    $content['8'][] = 'Racha';
    $content['8'][] = ' ';
    $content['8'][] = 'Racha';
    $content['9'][] = $home_team->Racha;
    $content['9'][] = ' ';
    $content['9'][] = $away_team->Racha;
    //Despliega el % de victorias tienen los equipos
    $content['10'][] = '% Victorias';
    $content['10'][] = ' ';
    $content['10'][] = '% Victorias';
    $content['11'][] = $home_team->Victorias;
    $content['11'][] = ' ';
    $content['11'][] = $away_team->Victorias;
    
    if ($node->home_score > $node->away_score )
      $winner = $home_team->title;
    else
      $winner = $away_team->title;

  
  $rows['2'][] = theme('table', $header, $content);
  
  //Segunda columna de contenido
  $content = array();
  $content['2'][] = 'MatchID';
  $content['2'][] = $node->nid;
  $content['3'][] = 'Fecha del partido';
  $content['3'][] = format_date($node->Fecha_Inicio, 'large', '');
  $content['4'][] = 'Número de rondas';
  $content['4'][] = $node->Numero_Rondas;
  
  //Genera un espacio para separar los links
  $content['5'][] = array('data' => 'Acciones', 'class' => 'header');
  $content['5'][] = array('data' => ' ', 'class' => 'header');
  
  //Si el desafio está en estado creado y el usuario es capitán del equipo retado despliega un link para aceptar el reto
  if (($node->Estado == CREADO) && $away_capitan) {
    $content['6'][] = l('Aceptar Desafio', 'node/'. $node->nid . '/aceptar');
  }
  
  //Si el desafio está en estado programado despliega un link para reportar el resultado
  if (($node->Estado == PROGRAMADO) && $is_capitan && ($node->Fecha_Inicio + 60*30 <= time())) {
    $content['6'][] = l('Reportar Resultado', 'node/'. $node->nid . '/reportar');
  }
  //Si el desafio está en estado conflicto despliega un link para subir las pruebas
  if (($node->Estado == CONFLICTO) && $is_capitan) {
    $content['6'][] = l('Enviar Pruebas', 'node/'. $node->nid . '/pruebas');
  }
  
  $content['7'][] = ' . ';
  $content['7'][] = ' . ';
  $content['8'][] = array('data' => 'Pruebas', 'class' => 'header');
  $content['8'][] = array('data' => ' ', 'class' => 'header');
  
  if (is_array($pruebas)) {
    $content['9'][] = 'Prueba del equipo '. $home_team->title .' : ';
    $content['9'][] = l($pruebas['Prueba_Jugador_Local'], $pruebas['Prueba_Jugador_Local']);
    $content['10'][] = 'Prueba del equipo '. $away_team->title .' : ';
    $content['10'][] = l($pruebas['Prueba_Jugador_Visitante'], $pruebas['Prueba_Jugador_Visitante']);
  }
  
  $rows['2'][] = theme('table', $header, $content);
  
  $output = theme('table', $header, $rows, array('id' => 'Match'));
  
  //Añade una tabla nueva para poner los resultados
  $rows = array();
  $rows['3'][] = array('data' => 'Resultado', 'class' => 'header'); 

  $content = array();
  $content['1'][] = array('data' => $match_info['home_score'], 'width' => '45%', 'align' => 'right');
  $content['1'][] = array('data' => ' : ', 'width' => '10%', 'align' => 'center');
  $content['1'][] = $match_info['away_score'];
  
  //Si es un desafio completado pone el nombre del equipo ganador
  if ($node->Estado == COMPLETADO) {
    if ($node->home_team == $node->ganador)
      $winner = $home_team;
    else
      $winner = $away_team;
    
    $content['2'][] = array('data' => '<div align="center" style="margin-top: 10px; margin-bottom: 10px; font-weight:bold;">'. $winner->title .' gana!</div>', 'colspan' => '5');
  }
  
  $rows['4'][] = theme('table', $header, $content);

  $output .= theme('table', $header, $rows, array('id' => 'Match'));
  
  return $output;
}