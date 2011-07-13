<?php

function theme_event_manager_match_block($to_show) {
  /*
   * The code below prepares a nice block to show the upcoming and las matches whit pictures or logos from participants 
   */
  
  while ($match = db_fetch_array($to_show)) {
    if ($match['min_pp'] > 1) {
      $home_team = node_load($match['home_team']);
      $away_team = node_load($match['away_team']);
      $block_content .= '<a href="/node/'. $match['nid'] . '"';
      $block_content .= theme('imagecache', 'profile', (empty($home_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $home_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $home_team->name);
      $block_content .= '<img src="'. drupal_get_path('module', 'event_manager_match') .'/images/vs.jpg">';
      $block_content .= theme('imagecache', 'profile', (empty($away_team->field_logo[0]['filepath']) ? $field['widget']['default_image']['filepath'] : $away_team->field_logo[0]['filepath']), 'Match', 'Imágen de '. $away_team->name);
      $block_content .= '</a><br />';
      $block_content .= '<bold>'. $match['title'] . '</bold><br />';
    }
    else {
      $home_team = user_load($match['home_team']);
      $away_team = user_load($match['away_team']);
      $block_content .= '<a href="/node/'. $match['nid'] . '"';
      $block_content .= theme('imagecache', 'profile', (!file_exists($home_team->picture) ? variable_get('user_picture_default', '') : $home_team->picture), 'Match', 'Imágen de '. $home_team->name);
      $block_content .= '<img src="'. drupal_get_path('module', 'event_manager_match') .'/images/vs.jpg">';
      $block_content .= theme('imagecache', 'profile', (!file_exists($away_team->picture) ? variable_get('user_picture_default', '') : $away_team->picture), 'Match', 'Imágen de '. $away_team->name);
      $block_content .= '</a><br />';
      $block_content .= '<bold>'. $match['title'] . '</bold><br />';
    }
    //$block_content .= l($links['title'], 'node/'.$links['nid']) . '<br />';
  }
  return $block_content;
}