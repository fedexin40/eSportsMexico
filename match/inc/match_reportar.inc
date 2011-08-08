<?php
/**
 * This function is to display a form for entering the result to a match.
 * It used to be on the main node edit page, but the development of more
 * complex results has meant it is going to be added as a separate page.
 */
function match_report(){
	//Obtiene el objeto que contien el nodo
	$node = node_load(arg(1));
	//Deffine la constante de usuario global
	global $user;
	//Obtiene los objetos de los equipos participantes en el reto
	$home_team = node_load(array('nid' => $node->nid_Equipo_Local));
	$away_team = node_load(array('nid' => $node->nid_Equipo_Visitante));
	//Pone un titulo para la pagina
	drupal_set_title('Introduce el resultado y la valoracion de tu rival');
	
	$options = array (
		VICTORIA => 'Victoria',
		DERROTA => 'Derrota',
		PROTESTA => 'Protesta',
	);
	
	$form['Detalles'] = array(
		'#value' => t('Dispones de 180 minutos a partir de la hora en que esta programado el desafio para introducir el resultado'),
	);
	$form['Resultado'] = array(
		'#title' => t('Resultado'),
		'#type' => 'fieldset',
	);
	$form['Resultado']['home_team_nid'] = array(
		'#value' => $home_team->nid,
		'#type' => 'hidden',
	);
	$form['Resultado']['home_team_resultado']= array(
		'#title' => $home_team->title,
		'#type' => 'select',
		'#options' => $options,
	);
	$form['Resultado']['away_team_nid'] = array(
		'#value' => $away_team->nid,
		'#type' => 'hidden',
	);
	$form['Resultado']['away_team_resultado'] = array(
		'#title' => $away_team->title,
		'#type' => 'select',
		'#options' => $options,
	);	
		
	if(team_user_is_team_capitan($user->uid, $home_team->nid)){
		$form['Resultado']['away_team_resultado']['#disabled'] = TRUE;
	}
	else {
		$form['Resultado']['home_team_resultado']['#disabled'] = TRUE;
	}
	$form['valoracion'] = array(
		'#title' => t('Valoracion de tu rival'),
		'#type' => 'fieldset',
	);	
    $form['valoracion']['puntuacion'] = array(
	  '#title' => 'Puntuacion',
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => array (
						20 => '1. Mala Conducta',
						40 => '2. Mal Jugador/Equipo',
						60 => '3. Normal',
						80 => '4. Buen Jugador/Equipo',
						100 => '5. Jugador/Equipo Excelente',
					),
	  '#default_value' => 60,
    );
    $form['valoracion']['comentario'] = array(
	  '#title' => 'Comentario del partido',
      '#type' => 'textarea',
      '#required' => FALSE,
	  '#resizable' => FALSE,
    );
    $form['valoracion']['conducta'] = array(
	  '#title' => 'Conducta antideportiva',
      '#type' => 'checkbox',
      '#required' => FALSE,
    );
    $form['valoracion']['reporte_conducta'] = array(
      '#type' => 'select',
      '#required' => TRUE,
      '#options' => array (
						0 => 'Seleccione una',
						1 => 'Insultos o amenazas',
						2 => 'Comentarios ofensivos',
						3 => 'Juego sucio o trampas',
					),
    );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enviar Resultado'),
  );
  return $form;
}

/**
 * Función para validar que no se haga dos veces el reporte de resultados
 * 
 */
function match_report_validate($form, $form_state){
  global $user;
  $node = node_load(arg(1));

  //Revisa que el desafío esté en estado programado
  if ($node->Estado != PROGRAMADO) 
	form_set_error('home_team_nid', t('El desafio todavia no ha sido aceptado por el equipo contrario.'));
  
  //Revisa que esté en tiempo para reportar el resultado de un desafío
  if (($node->Fecha_Inicio + 60*60*3) < time() ) 
	//form_set_error('home_team_nid', t('Ya ha pasado el tiempo para reportar el resultado de este desafio'));

  //Si se marcó la opción de reporte de mala conducta, revisa que se haya seleccionado un tipo de conducta
  if ($form_state['values']['conducta'] == TRUE && $form_state['values']['reporte_conducta'] == 0)
	form_set_error('reporte_conducta', t('Selecciona una opcion de mala conducta a reportar'));
 
  if (team_user_is_team_capitan($user->uid, $form_state['values']['home_team_nid'])) 
	$reporte = _match_get_team_resultado($node->nid, $form_state['values']['home_team_nid']);
	
  elseif (team_user_is_team_capitan($user->uid, $form_state['values']['away_team_nid'])) 
	$reporte = _match_get_team_resultado($node->nid, $form_state['values']['away_team_nid']);
	
  else
	form_set_error('home_team_nid', t('Solo los capitanes de equipo pueden reportar resultados'));
  
	if (is_object($reporte)) 
		form_set_error('home_team_nid', 'Ya has reportado el resultado para este desafio');

}
/**
 * Función para guardar los resultados reportados en la base de datos
 */
function match_report_submit($form, $form_state){
  global $user;
  //Carga los datos del nodo
  $node = arg(1);
  $node = node_load(intval($node));
 	
  //Guarda los datos en las tablas correspondientes
  if (team_user_is_team_capitan($user->uid, $form_state['values']['home_team_nid'])) {
	db_set_active('eSM');
		//Guarda el resultado del desafío reportado por el equipo local
		db_query('INSERT INTO {Reporte_Reto}
				 (nid_Reto, nid_Equipo, Resultado, Fecha, Comentario)
				 VALUES (%d, %d, %d, %d, "%s")',
				 $node->nid, $form_state['values']['home_team_nid'], $form_state['values']['home_team_resultado'], time(), $form_state['values']['comentario']);
	db_set_active('default');		
		
	//Crea un reporte de mala conducta para el equipo visitante si se ha seleccionado la opción
	if ($form_state['values']['conducta'] == TRUE)
		_match_crear_reporte_mala_conducta($node->nid, $form_state['values']['away_team_nid'], $form_state['values']['reporte_conducta']);

	//Arma el arreglo $vote para guardar la valoración al equipo contrario
	$vote['content_id'] = $form_state['values']['away_team_nid'];
  }
  elseif (team_user_is_team_capitan($user->uid, $form_state['values']['away_team_nid'])) {
	db_set_active('eSM');
		//Guarda el resultado del desafío reportado por el equipo visitante
		db_query('INSERT INTO {Reporte_Reto}
				 (nid_Reto, nid_Equipo, Resultado, Fecha, Comentario)
				 VALUES (%d, %d, %d, %d, "%s")',
				 $node->nid, $form_state['values']['away_team_nid'], $form_state['values']['away_team_resultado'], time(), $form_state['values']['comentario']);
	db_set_active('default');		
		
	//Crea un reporte de mala conducta para el equipo visitante si se ha seleccionado la opción
	if ($form_state['values']['conducta'] == TRUE)
		_match_crear_reporte_mala_conducta($node->nid, $form_state['values']['home_team_nid'], $form_state['values']['reporte_conducta']);
		
	//Arma el arreglo $vote para guardar la valoración al equipo contrario
	$vote['content_id'] = $form_state['values']['home_team_nid'];
  }
  
  //Arma el resto del arreglo $vote para guardar la puntuación al equipo contrario
	$vote['content_type'] = 'team';
	$vote['value'] = $form_state['values']['puntuacion'];
	$vote['uid'] = $user->uid;
  
  //Guarda la puntuación otorgada al equipo contrario
  votingapi_set_votes($vote);
  
  //Llama a la rutina que checa si ya se han reportado los dos resultados y si son coherentes
  _match_check_match($node->nid); 
  
  //Redirige al usuario y despliega el mensaje de confirmación
  drupal_set_message('Has reportado correctamente el resultado de tu partido.');
  drupal_goto('node/'.$node->nid);
}

/* Funciones que están descartadas momentaneamente
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
   *
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

/function event_manager_match_list_validate($form, $form_state){

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

/*
 * Theme for the match_admin panel, showing the upcoming games in rows
 *
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
 *
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
 *
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
 *
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
}*/