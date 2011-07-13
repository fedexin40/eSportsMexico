<?php

function theme_event_manager_match_content($node) {
  drupal_add_css(drupal_get_path('module', 'event_manager_match') . '/css/event_manager_match.css');
  
  //get the extra information from the database
  $signup = db_fetch_array(db_query("SELECT min_pp FROM {signup} WHERE nid = %d", $node->season_league));
  $match_info = db_fetch_array(db_query("SELECT * FROM {event_manager_match} WHERE nid = %d LIMIT 1", $node->nid));
  
  $header = array();
  
  //Primera columa del contenido
  $rows['1'][] = array('data' => 'Competidores', 'class' => 'header', 'width' => '50%'); 
  $rows['1'][] = array('data' => 'Parámetros', 'class' => 'header', 'width' => '50%');

  if ($signup['min_pp'] > 1) {
    $home_team = node_load($node->home_team);
    $away_team = node_load($node->away_team);
    drupal_set_title($home_team->title. " vs " . $away_team->title);
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
  }
  else {
    $home_team = user_load($node->home_team);
    $away_team = user_load($node->away_team);
    drupal_set_title($home_team->name. " vs " . $away_team->name);
    
    $content['2'][] = array('data' => '<a href="/user/'. $home_team->uid . '"'. theme('imagecache', 'match', (!file_exists($home_team->picture) ? variable_get('user_picture_default', '') : $home_team->picture), 'Match', 'Imágen de '. $home_team->name) . '</a>', 'width' => '45%', 'align' => 'right');
    $content['2'][] = array('data' =>' VS. ', 'width' => '10%', 'align' => 'center');
    $content['2'][] = '<a href="/user/'. $away_team->uid . '"'. theme('imagecache', 'match', (!file_exists($away_team->picture) ? variable_get('user_picture_default', '') : $away_team->picture), 'Match', 'Imágen de '. $away_team->name) . '</a>';
    $content['3'][] = array('data' => '<a href="/user/'. $home_team->uid . '">'. $home_team->name . '</a>', 'align' => 'right');
    $content['3'][] = ' ';
    $content['3'][] = '<a href="/user/'. $away_team->uid . '">'. $away_team->name . '</a>';
    
    if ($node->home_score > $node->away_score )
      $winner = $home_team->name;
    else
      $winner = $away_team->name;
  }
  
  $rows['2'][] = theme('table', $header, $content);
  
  //Segunda columna de contenido
  $content = array();
  $content['2'][] = 'MatchID';
  $content['2'][] = $node->nid;
  $content['3'][] = 'Fecha del partido';
  $content['3'][] = format_date($match_info['match_time'], 'large', '');
  $content['4'][] = l('Reportar Resultado', 'node/'. $node->nid . '/edit/score');
  
  $rows['2'][] = theme('table', $header, $content);
  
  $rows['3'][] = array('data' => 'Resultados', 'class' => 'header'); 
  $rows['3'][] = array('data' => 'Media', 'class' => 'header');

  $content = array();
  $content['1'][] = array('data' => $match_info['home_score'], 'width' => '45%', 'align' => 'right');
  $content['1'][] = array('data' => ' : ', 'width' => '10%', 'align' => 'center');
  $content['1'][] = $match_info['away_score'];
  if ($node->is_result == 1)
    $content['2'][] = array('data' => '<div align="center" style="margin-top: 10px; margin-bottom: 10px; font-weight:bold;">'. $winner .' gana!</div>', 'colspan' => '5');
  
  $rows['4'][] = theme('table', $header, $content);

  $output = theme('table', $header, $rows, array('id' => 'Match'));
  
  return $output;
}

/**
 * Función para formatear el teaser de los nodos de tipo match
 */
function theme_event_manager_match_teaser($node) {
}