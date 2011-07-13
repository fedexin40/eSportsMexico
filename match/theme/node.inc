<?php

function theme_event_manager_match_node_form($form) {
  /*
   * The code below prepares a nice array for using Javascript to select teams according to the season/league selected 
   */
  $seasons = db_query("SELECT s.min_pp AS min_pp, s.nid AS relation_id FROM {signup} s INNER JOIN {event} e ON s.nid = e.nid INNER JOIN {event_manager_settings} es ON s.nid = es.nid WHERE es.status < 4");
  while($season = db_fetch_array($seasons)){
    if ($season['min_pp'] > 1) 
      $season_teams = db_query("SELECT sl.uid team_id, n.title title FROM {signup_log} sl INNER JOIN {node} n ON sl.uid = n.nid WHERE sl.nid = %d", $season['relation_id']);
    else 
      $season_teams = db_query("SELECT sl.uid team_id, u.name title FROM {signup_log} sl INNER JOIN {users} u ON sl.uid = u.uid WHERE sl.nid = %d", $season['relation_id']);
    
    while($team = db_fetch_array($season_teams)){
      $season_team_list[$season['relation_id']][$team['team_id']] = $team['title'];
    }
  }

  drupal_add_js('var teamlist = '.drupal_to_js($season_team_list), 'inline', 'footer');
  drupal_add_js(drupal_get_path('module', 'event_manager_match') . "/theme/event_manager_match.js");
  return drupal_render($form);
}