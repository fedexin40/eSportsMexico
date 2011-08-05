<?php

function theme_match_content($node) {
  global $user;
  //Añade el archivo css donde viene la personalización de la vista del desafio
  drupal_add_css(drupal_get_path('module', 'match') . '/css/match.css');
  
  //Verifica si el usuario actual es capitán de alguno de los dos equipos
  if (team_user_is_team_capitan($user->uid, $node->nid_Equipo_Local) || team_user_is_team_capitan($user->uid, $node->nid_Equipo_Visitante))
    $is_capitan = TRUE;
  
  $header = array();
  
  //Primera columa del contenido
  $rows['1'][] = array('data' => 'Competidores', 'class' => 'header', 'width' => '50%'); 
  $rows['1'][] = array('data' => 'Parámetros', 'class' => 'header', 'width' => '50%');

    $home_team = node_load($node->nid_Equipo_Local);
    $away_team = node_load($node->nid_Equipo_Visitante);
    //drupal_set_title($home_team->title. " vs " . $away_team->title);
    $field = content_fields('field_logo', 'club');
    
    $content['2'][] = array('data' => '<a href="/node/'. $home_team->nid . '"'. theme('imagecache', 'match', (empty($home_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $home_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $home_team->title) . '</a>', 'width' => '45%', 'align' => 'right');
    $content['2'][] = ' VS. ';
    $content['2'][] = '<a href="/node/'. $away_team->nid . '"'. theme('imagecache', 'match', (empty($away_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $away_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $away_team->title) . '</a>';
    $content['3'][] = '<a href="/node/'. $home_team->nid . '">'. $home_team->title . '</a>';
    $content['3'][] = ' ';
    $content['3'][] = '<a href="/node/'. $away_team->nid . '">'. $away_team->title . '</a>';
    
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
  
  //Si el desafio está en estado programado despliega un link para reportar el resultado
  if ($node->Estado == PROGRAMADO && $is_capitan) {
    $content['4'][] = l('Reportar Resultado', 'node/'. $node->nid . '/reportar');
  }
  
  $rows['2'][] = theme('table', $header, $content);
  
  $rows['3'][] = array('data' => 'Resultados', 'class' => 'header'); 
  $rows['3'][] = array('data' => 'Media', 'class' => 'header');

  $content = array();
  $content['1'][] = array('data' => $match_info['home_score'], 'width' => '45%', 'align' => 'right');
  $content['1'][] = array('data' => ' : ', 'width' => '10%', 'align' => 'center');
  $content['1'][] = $match_info['away_score'];
  
  //Si es un desafio completado pone el nombre del equipo ganador
  if ($node->Estado == COMPLETADO) {
    $winner = node_load(array('nid' => $node->ganador));
    $content['2'][] = array('data' => '<div align="center" style="margin-top: 10px; margin-bottom: 10px; font-weight:bold;">'. $winner->title .' gana!</div>', 'colspan' => '5');
  }
  
  $rows['4'][] = theme('table', $header, $content);

  $output = theme('table', $header, $rows, array('id' => 'Match'));
  
  return $output;
}