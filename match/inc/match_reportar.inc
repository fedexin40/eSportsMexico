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
	//Guarda el resultado del desafío reportado por el equipo local
	db_query('INSERT INTO {eSM_Reporte_Reto}
			 (nid_Reto, nid_Equipo, Resultado, Fecha, Comentario)
			 VALUES (%d, %d, %d, %d, "%s")',
			 $node->nid, $form_state['values']['home_team_nid'], $form_state['values']['home_team_resultado'], time(), $form_state['values']['comentario']);
		
	//Crea un reporte de mala conducta para el equipo visitante si se ha seleccionado la opción
	if ($form_state['values']['conducta'] == TRUE)
		_match_crear_reporte_mala_conducta($node->nid, $form_state['values']['away_team_nid'], $form_state['values']['reporte_conducta']);

	//Arma el arreglo $vote para guardar la valoración al equipo contrario
	$vote['content_id'] = $form_state['values']['away_team_nid'];
  }
  elseif (team_user_is_team_capitan($user->uid, $form_state['values']['away_team_nid'])) {
	//Guarda el resultado del desafío reportado por el equipo visitante
	db_query('INSERT INTO {eSM_Reporte_Reto}
			 (nid_Reto, nid_Equipo, Resultado, Fecha, Comentario)
			 VALUES (%d, %d, %d, %d, "%s")',
			 $node->nid, $form_state['values']['away_team_nid'], $form_state['values']['away_team_resultado'], time(), $form_state['values']['comentario']);
		
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