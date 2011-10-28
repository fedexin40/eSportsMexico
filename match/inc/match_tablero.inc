<?php
/**
 * Despliega la lista de desafíos que se encuentran colgados en el tablero
 */
function tablero_desafios() {
	global $user;
  $tid = arg(2);
  //Obtiene los datos de la modalidad con base en el tid que proporciona la ruta
  $modalidad = term_is_active($tid);
	$is_capitan = is_capitan($user->uid, $modalidad->id_Modalidad_Juego_Temporada);
	
	//Añade el link para colgar un desafio en el tablero
	$output = '<div class="form_item add_match">';
	$output .= l(t('Add Match'), 'taxonomy/term/'. $tid .'/match/defy', array('attributes'=>array('class' => 'popups')));
	$output .= '</div>';
	
	$retos = _match_modalidad_get_match_list($modalidad->id_Modalidad_Juego_Temporada, TABLERO);
	$count = 0;
  while($reto = db_fetch_object($retos)) {
		++$count;
		$home_team = node_load($reto->nid_Equipo_Local);
		
		//Obtiene el ránking del equipo
    $ranking = ranking_get_ranking_equipo($home_team->nid, $modalidad->id_Modalidad_Juego_Temporada);
    //Obtiene la división a la que pertenece un equipo
    $division = ranking_get_division_equipo($ranking);
		
		$row = array();
		$row[ 'Division' ] = $division;
		$row[ 'Fecha' ] = format_date($reto->Fecha_Inicio, 'large');
		$row[ 'Reputacion' ] = _match_display_reputation($home_team->nid);
		$row[ 'Rondas' ] = t($reto->Numero_Rondas. ' mapa');
		$row[ 'Detalles' ] = l('Detalles', 'node/'. $reto->nid_Reto);
		if($is_capitan)
			$row[ 'Aceptar' ] = l('Aceptar', 'node/'. $reto->nid_Reto .'/accept');
			
		$rows[] = $row;
  }
  
  if ($count == 0) {
    $output .= t('No hay desafios programados.');
  }
	else {
		$header = array(t('Division'), t('Fecha'), t('Reputacion Rival'), t('Best of'), '', '');
		$output .= theme('table', $header, $rows);
	}
	
	return $output;
}
/**
 * Despliega el formulario para insertar un desafío en el tablón
 */
function tablero_new_match() {
	global $user;
	$tid = arg(2);
	
	if (!($modalidad = term_is_active($tid)))
    return t('Esta modalidad no esta activa en esta temporada.');
	
	if (!(is_capitan($user->uid, $modalidad->id_Modalidad_Juego_Temporada)))
		return t('Solo los capitanes pueden crear desafios');
		
  //Si el jugador cumple con todos los requisitos se despliega la forma para crear un nuevo equipo.
  if( !function_exists("node_object_prepare")) 
     include_once(drupal_get_path('module', 'node') . '/node.pages.inc');
		 
  $type = 'match';
  // Initialize settings:
  $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '', 'estado' => TABLERO, 'id_Modalidad_Juego_Temporada' => $modalidad->id_Modalidad_Juego_Temporada);
  
  return drupal_get_form($type .'_node_form', $node);
}