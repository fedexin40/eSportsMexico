<?php
// $Id: event_manager_match.admin.inc,v 1.12 2010/04/15 18:35:03 alun Exp $

function event_manager_match_admin(){
	$output = '';
  //$output .= drupal_get_form('event_manager_match_filter');
	$output .= drupal_get_form('event_manager_match_list');
	return $output;
}

/**
 * This function is to display a form for entering the result to a match.
 * It used to be on the main node edit page, but the development of more
 * complex results has meant it is going to be added as a separate page.
 */
function event_manager_match_score_edit(){
	$node = node_load(arg(1));
	$form = NULL;
	
	/**
	 * invoke a method for another module here? One for the match results
	 * If there is no result, then the module isn't enabled, or there are no match attributes
	 * Just show the basic form in this case
	 */
	if(module_exists('event_manager_scoring')){
    $form = event_manager_scoring_load_form($node);
	}
	if($form == NULL){
    $form['results']['home_score'] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#required' => FALSE,
      '#maxlength' => 3,
      '#default_value' => $node->home_score,
    );
    
    $form['results']['away_score'] = array(
      '#type' => 'textfield',
      '#required' => FALSE,
      '#maxlength' => 3,
      '#size' => 3,
      '#default_value' => $node->away_score,
    );
	}
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function theme_event_manager_match_score_edit($form){
  $attributes = array();
  
  //check if there are any attributes, or if we just need the whole score
  if(module_exists('event_manager_scoring') && isset($form['results']['attid0'])){
  	$count = 0;
  	while (isset($form['results']['attid'.$count])){
  		$attributes[] = array(
  		  check_plain($form['results']['attname'.$count]['#value']),
        drupal_render($form['results']['homeattval'.$count]),
        drupal_render($form['results']['awayattval'.$count]),
  		);
  		unset($form['results']['attname'.$count]);
  		unset($form['results']['homeattval'.$count]);
  		unset($form['results']['awayattval'.$count]);
  		++$count;
  	}
  }else{
  	//Just the one line for the score then.
  	$attributes[] = array(
      t('Score'),
      drupal_render($form['results']['home_score']),
      drupal_render($form['results']['away_score']),
  	);
  	unset($form['results']['home_score']);
  	unset($form['results']['away_score']);
  }
  $header = array(t(''), t('Home'), t('Away'));
  $output .= theme('table', $header, $attributes);
  $output .= drupal_render($form);
  return $output;
}

function event_manager_match_score_edit_submit($form, $form_state){
  /**
   * If there are additional match attributes for the game, I will have to save them first.
   * I can then use the information to calculate the final score.
   * Get the submitted results, load the node and change the result, then call node_save() 
	 */
  $node = arg(1);
  $node = node_load(intval($node));
  //check for attributes, firstly if the event_manager_scoring module is enabled. If there are any, they will start with a zero.
 	
  if(module_exists('event_manager_scoring') && isset($form_state['values']['attid0'])){
  	//save the attributes
  	$attributes = array();
  	$count = 0;
  	while(isset($form_state['values']['attid'.$count])){
  	  $attributes['home'][$form_state['values']['attid'.$count]] = $form_state['values']['homeattval'.$count];
  	  $attributes['away'][$form_state['values']['attid'.$count]] = $form_state['values']['awayattval'.$count];
  	  ++$count;
  	}
  	$node->match_attributes = $attributes;
  	event_manager_scoring_calculate_score($node);
  }else{
  	$node->home_score = (string)($form_state['values']['home_score']);
  	$node->away_score = (string)($form_state['values']['away_score']);
  }
	node_save($node);
	drupal_goto('/node/'.$node->nid);
}

function event_manager_match_filter(){
	$session = &$_SESSION['event_manager_match_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = user_filters();
  //get seasons
  //get leagues
  //let user choose all, past or future matches

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only matches where'),
    '#theme' => 'event_manager_match_filters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    // Merge an array of arrays into one if necessary.
    $options = $type == 'permission' ? call_user_func_array('array_merge', $filters[$type]['options']) : $filters[$type]['options'];
    $params = array('%property' => $filters[$type]['title'] , '%value' => $options[$value]);
    if ($i++ > 0) {
      $form['filters']['current'][] = array('#value' => t('<em>and</em> where <strong>%property</strong> is <strong>%value</strong>', $params));
    }
    else {
      $form['filters']['current'][] = array('#value' => t('<strong>%property</strong> is <strong>%value</strong>', $params));
    }
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
    );
  }

  $form['filters']['filter'] = array(
    '#type' => 'radios',
    '#options' => $names,
  );
  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => (count($session) ? t('Refine') : t('Filter')),
  );
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }


  return $form;
}

function event_manager_match_list(){
  //add a filter table here?
  $filter = "WHERE {event_manager_match}.match_time < UNIX_TIMESTAMP() ";
  //Here, I should add all the matches which can be edited by the current user. I would like to do this on a filter table though. 
  $form = array();
  //set up form where we can change various match settings
  //get all the matches that have passed the fixture date with no score.
  $result = db_query("SELECT {event_manager_match}.nid FROM {event_manager_match} ".$filter."ORDER BY {event_manager_match}.match_time;");
  //loop through the matches and check permissions to edit the matches. If so, add them to the form.
  /* 
   * Check permissions using node_access('amend', node_load($nid , $revision = NULL, $reset = NULL))) 
   */
  //add a count to see how many matches we have added.
  $count = 0;
  while($match = db_fetch_array($result)){
    $node = node_load($match['nid']);
    if(node_access('update', $node)){
      $form[$count] = array(
        '#type' => 'fieldset',
        '#title' => t($node->title),
      );
      $form[$count][$count] = array(
        '#type' => 'hidden',
        '#value' => $node->title,
      );
      //TODO: Check setting for 24hr time here. Definitely need DateAPI integration here
      $form[$count]['match_time'.$count] = array(
        '#type' => 'hidden',
        '#value' => $node->match_hour.':'.$node->match_minute,
      );
      $form[$count]['match_id'.$count] = array(
        '#type' => 'hidden',
        '#value' => $node->nid,
      );
      $form[$count]['home'.$count] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#required' => FALSE,
        '#default_value' => $node->home_score,
      );
      $form[$count]['away'.$count] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#required' => FALSE,
        '#default_value' => $node->away_score,
      );
      ++$count;
    }
  }
  if($count > 0){
    $form['submit'] = array(
      '#type' => 'submit', '#value' => t('Save Changes'), '#weight' => 10,
    );
  }
  return $form;
}

function event_manager_match_list_validate($form, $form_state){

for($id = 0; isset($form_state['values']['match_id'.$id]); ++$id){
    if(($form_state['values']['home'.$id] != '' && $form_state['values']['away'.$id] == '')||($form_state['values']['home'.$id] == '' && $form_state['values']['away'.$id] != '')){
      //return error - both teams must have scores if any have scores.
      form_set_error('home'.$id, t('Both teams must have a score entered if any have a score entered.'));
    }
  }
}

function event_manager_match_list_submit($form, $form_state){
  for($id = 0; isset($form_state['values']['match_id'.$id]); ++$id){
    //set results
    $node = node_load($form_state['values']['match_id'.$id]);
    //dont resave if the match result hasn't changed.
    if($node->home_score == $form_state['values']['home'.$id] && $node->away_score == $form_state['values']['away'.$id]){
    	continue;
    }
    $node->home_score = $form_state['values']['home'.$id];
    $node->away_score = $form_state['values']['away'.$id];
    node_save($node);
  }
  drupal_set_message($form_state['values'][$id].t('Results saved.'));
}

function event_manager_match_settings(){
  $form = array();
  //set up form where we can change various match settings
  
  //allow user override of match naming
  $form['general_match_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Match name pattern'),
    '#description' => t('This is the pattern used to name a match node. Values you can use are [home_team] and [away_team]'),
    '#default_value' => variable_get('event_manager_match_general_match_name', '[home_team] v [away_team]'),
  );
  
  $form['general_result_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Result name pattern'),
    '#description' => t('This is the pattern used to name a result node values you can use are [home_team], [away_team], [home_score] and [away_score]'),
    '#default_value' => variable_get('event_manager_match_general_result_name', '[home_score] [home_team] - [away_score] [away_team]'),
  );
  
  //update league tables when a user enters a match result
  $form['update_table_insert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update league tables when match result is entered'),
    '#description' => t('If this is checked, when a user enters a match result, the corresponding league table will be updated'),
    '#default_value' => variable_get('event_manager_match_update_table_insert', ''),
  );
  //update league tables when a user changes a match result
  $form['update_table_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update league table when match result is amended'),
    '#default_value' => variable_get('event_manager_match_update_table_update', ''),
  );
  //update league tables when a user deletes a match result
  $form['update_table_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update league table when match result is deleted'),
    '#default_value' => variable_get('event_manager_match_update_table_delete', ''),
  );
    
  //change match teams once a match is created
  $form['change_teams'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change teams'),
    '#description' => t('Allow a user to change the teams, and the league & season for a match once it has been created.'),
    '#default_value' => variable_get('event_manager_match_change_teams', ''),
  );
  
  for($count = 0; $count < 24; ++$count){
    $hours[$count] = sprintf("%02d",$count); ;
  }
  for($count = 0; $count < 60; $count=$count+15){
    $mins[$count] = sprintf("%02d",$count);
  }
  $form['default_start'] = array(
    '#type' => 'fieldset',
    '#title' => t('The default start time for a match'),
  );
  $form['default_start']['hour'] = array(
    '#type' => 'select',
    '#title' => 'hour',
    '#required' => TRUE,
    '#options' => $hours,
    '#default_value' => variable_get('event_manager_match_start_time_hour', '')
  );
  $form['default_start']['minute'] = array(
    '#type' => 'select',
    '#title' => 'minute',
    '#required' => TRUE,
    '#options' => $mins,
    '#default_value' => variable_get('event_manager_match_start_time_minute', '')
  );
  
  //change match dates once a match is created (create a new node, set the original to postponed?)
  
  $form['submit'] = array(
    '#type' => 'submit', '#value' => t('Save')
  );
  return $form;
}

function event_manager_match_settings_submit($form, $form_state){
	variable_set('event_manager_match_general_result_name', $form_state['values']['general_result_name']);
  variable_set('event_manager_match_general_match_name', $form_state['values']['general_match_name']);
  variable_set('event_manager_match_update_table_insert', $form_state['values']['update_table_insert']);
  variable_set('event_manager_match_update_table_update', $form_state['values']['update_table_update']);
  variable_set('event_manager_match_update_table_delete', $form_state['values']['update_table_delete']);
  variable_set('event_manager_match_change_teams', $form_state['values']['change_teams']);
  variable_set('event_manager_match_start_time_hour', $form_state['values']['hour']);
  variable_set('event_manager_match_start_time_minute', $form_state['values']['minute']);
}

/*
 * Theme for the match_admin panel, showing the upcoming games in rows
 */
function theme_event_manager_match_list($form){
	if(module_exists('event_manager_scoring')){
  $output = '<p>Click details if you want to enter the score breakdown of a match</p>';
	}
  if(isset($form['submit'])){
    $matches = array();
    foreach($form as $name => $element){
      if(isset($element['match_id'.$name]) && is_array($element['match_id'.$name])){
        //TODO: Add time and date to the match display
        $matches[] = array(
          check_plain($element['match_time'.$name]['#value']),
          check_plain($element[$name]['#value']),
          drupal_render($element['home'.$name]),
          drupal_render($element['away'.$name]),
          l('details', 'node/'.$element['match_id'.$name]['#value'].'/edit/score'),
        );
        unset($form[$name]);
      }
    }
    $teamsheader = array(t('Time'), t('Match'), t('Home Score'), t('Away Score'), t('Edit'));
    if(count($matches) > 0){
      $output .= theme('table', $teamsheader, $matches);
    }else{
      //no teams found
      $output = '';
    }
    $output .= drupal_render($form);
  }else{
    $output = t('You have no outstanding matches!');
  }
  return $output;
}

/**
 * Form builder; Return form for user administration filters.
 *
 * @ingroup forms
 * @see user_filter_form_submit()
 */
function event_manager_match_filter_form() {
  $session = &$_SESSION['user_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = user_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only users where'),
    '#theme' => 'user_filters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    // Merge an array of arrays into one if necessary.
    $options = $type == 'permission' ? call_user_func_array('array_merge', $filters[$type]['options']) : $filters[$type]['options'];
    $params = array('%property' => $filters[$type]['title'] , '%value' => $options[$value]);
    if ($i++ > 0) {
      $form['filters']['current'][] = array('#value' => t('<em>and</em> where <strong>%property</strong> is <strong>%value</strong>', $params));
    }
    else {
      $form['filters']['current'][] = array('#value' => t('<strong>%property</strong> is <strong>%value</strong>', $params));
    }
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
    );
  }

  $form['filters']['filter'] = array(
    '#type' => 'radios',
    '#options' => $names,
  );
  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => (count($session) ? t('Refine') : t('Filter')),
  );
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array(
      '#type' => 'submit',
      '#value' => t('Undo'),
    );
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  drupal_add_js('misc/form.js', 'core');

  return $form;
}

/**
 * Process result from user administration filter form.
 */
function event_manager_match_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = user_filters();
  switch ($op) {
    case t('Filter'): case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];
        // Merge an array of arrays into one if necessary.
        $options = $filter == 'permission' ? call_user_func_array('array_merge', $filters[$filter]['options']) : $filters[$filter]['options'];
        if (isset($options[$form_state['values'][$filter]])) {
          $_SESSION['user_overview_filter'][] = array($filter, $form_state['values'][$filter]);
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['user_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['user_overview_filter'] = array();
      break;
    case t('Update'):
      return;
  }

  $form_state['redirect'] = 'admin/user/user';
  return;
}

/**
 * Theme match administration filter selector.
 *
 * @ingroup themeable
 */
function theme_event_manager_match_filters($form) {
  $output = '<ul class="clear-block">';
  if (!empty($form['current'])) {
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>'. drupal_render($form['current'][$key]) .'</li>';
    }
  }

  $output .= '<li><dl class="multiselect">'. (!empty($form['current']) ? '<dt><em>'. t('and') .'</em> '. t('where') .'</dt>' : '') .'<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';

  $output .= '<dt>'. t('is') .'</dt><dd class="b">';

  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';

  $output .= '</dl>';
  $output .= '<div class="container-inline" id="user-admin-buttons">'. drupal_render($form['buttons']) .'</div>';
  $output .= '</li></ul>';

  return $output;
}