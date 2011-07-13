<?php
function event_manager_match_views_data(){
  $data['event_manager_match']['table']['group'] = t('event_manager');
  $data['event_manager_match']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['event_manager_match']['is_result'] = array(
    'title' => t('Is a result'),
    'help' => t('A boolean representing if a match has a result or not'),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator'
    ),
  );
  $data['event_manager_match']['home_team'] = array(
    'title' => t('Home team'),
    'help' => t('The nid of the home team in the match'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Home Team'),
    ),
  );
  $data['event_manager_match']['away_team'] = array(
    'title' => t('Away team'),
    'help' => t('The nid of the away team in the match'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Away Team'),
    ),
  );
  $data['event_manager_match']['home_score'] = array(
    'title' => t('Home score'),
    'help' => t('The home team score'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['event_manager_match']['away_score'] = array(
    'title' => t('Away score'),
    'help' => t('The away team score'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['event_manager_match']['match_time'] = array(
    'title' => t('Start time'),
    'help' => t('The starting time of the match'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['event_manager_match']['relation_id'] = array(
    'title' => t('League and season id'),
    'help' => t('The league and season the match belongs to'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['event_manager_match']['team_id'] = array(
    'title' => t('Team Id'),
    'help' => t('The id of one of the teams playing.'),
    'argument' => array(
      'handler' => 'event_manager_match_argument_team_id',
      'label' => t('Id for the team'),
    ),
  );
  $data['event_manager_match']['taxonomy_id'] = array(
    'title' => t('Taxonomy Id'),
    'help' => t('The taxonomy match belongs to'),
    'argument' => array(
      'handler' => 'event_manager_match_argument_taxonomy_id',
      'label' => t('Id for the taxonomy'),
    ),
  );
  return $data;
}


function event_manager_match_views_handlers(){
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'event_manager_match') . '/views',
    ),
    'handlers' => array(
      'event_manager_match_argument_team_id' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
      'event_manager_match_argument_taxonomy_id' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
    ),
  );
}