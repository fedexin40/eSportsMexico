<?php
/**
 * This function is to display a list of matches
 */
function modalidad_lista_desafios_conflicto () {
  $op = arg(5);
  $tid = arg(2);
  
  //Obtiene los datos de la modalidad con base en el tid que proporciona la ruta
  $modalidad = term_is_active($tid);  
  drupal_set_title(t('Conflict matches from '). $modalidad->Nombre);
  
  //Verifica si el usuario ha pedido saltarse algúnos desafíos
  if ($op == 'skip') {
    $offset = arg(6);
  }
  
  //Obtiene la lista de retos
  $retos = _match_modalidad_get_match_list($modalidad->id_Modalidad_Juego_Temporada, CONFLICTO, $offset);
  if ($reto = db_fetch_object($retos)) {     
      //Obtiene los datos de los equipos participantes
      $home_team = node_load($reto->nid_Equipo_Local);
      $away_team = node_load($reto->nid_Equipo_Visitante);
      
      //Agrega a la forma los valores usados para validar y guardar resultados
      $form['valores']['nid_Reto'] = array (
        '#type' => 'hidden',
        '#value' => $reto->nid_Reto,
      );
      $form['valores']['home_team_nid'] = array (
        '#type' => 'hidden',
        '#value' => $home_team->nid,
      );
      $form['valores']['away_team_nid'] = array (
        '#type' => 'hidden',
        '#value' => $away_team->nid,
      );
      
      //Crea el div de cada equipo
      $form['home_team'] = array (
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#title' => $home_team->title,
        '#attributes' => array ('class' => 'match_details'),
      );
      $form['away_team'] = array (
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#title' => $away_team->title,
        '#attributes' => array ('class' => 'match_details'),
      );
      
      //Agrega la reputación de cada equipo
      $form['home_team']['reputation'] = array (
          '#type' => 'item',
          '#title' => t('Reputation'),
          '#value' => _match_display_reputation($home_team->nid),
      );
      $form['away_team']['reputation'] = array (
          '#type' => 'item',
          '#title' => t('Reputation'),
          '#value' => _match_display_reputation($away_team->nid),
      );
      
      //Agrega el resultado reportado por cada equipo
      $reporte_equipo_local = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Local);
      $reporte_equipo_visitante = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Visitante);
      
      if ($reporte_equipo_local->Resultado == VICTORIA)
        $resultado_local = t('Won');
      elseif ($reporte_equipo_local->Resultado == DERROTA)
        $resultado_local = t('Loss');
      elseif ($reporte_equipo_local->Resultado == PROTESTA)
        $resultado_local = t('Protest');
      else
        $resultado_local = t('No report was maded.');
        
      if ($reporte_equipo_visitante->Resultado == VICTORIA)
        $resultado_visitante = t('Won');
      elseif ($reporte_equipo_visitante->Resultado == DERROTA)
        $resultado_visitante = t('Loss');
      elseif ($reporte_equipo_visitante->Resultado == PROTESTA)
        $resultado_visitante = t('Protest');
      else
        $resultado_visitante = t('No report was maded.');
        
      $form['home_team']['report'] = array(
        '#type' => 'item',
        '#title' => t('Report'),
        '#value' => $resultado_local,
      );
      $form['away_team']['report'] = array(
        '#type' => 'item',
        '#title' => t('Report'),
        '#value' => $resultado_visitante,
      );
      
      //Agrega las pruebas aportadas por cada equipo
      $pruebas = match_get_pruebas($reto->nid_Reto);
      if($pruebas['Prueba_Jugador_Local'] == '')
        $pruebas['Prueba_Jugador_Local'] = t('No proof was provided.');
      else
        $pruebas['Prueba_Jugador_Local'] = l($pruebas['Prueba_Jugador_Local'], $pruebas['Prueba_Jugador_Local']);
        
      if($pruebas['Prueba_Jugador_Visitante'] == '')
        $pruebas['Prueba_Jugador_Visitante'] = t('No proof was provided.');
      else
        $pruebas['Prueba_Jugador_Visitante'] = l($pruebas['Prueba_Jugador_Visitante'], $pruebas['Prueba_Jugador_Visitante']);
        
      $form['home_team']['proof'] = array(
        '#type' => 'item',
        '#title' => t('Pruebas'),
        '#value' => $pruebas['Prueba_Jugador_Local'],
      );
      $form['away_team']['proof'] = array(
        '#type' => 'item',
        '#title' => t('Pruebas'),
        '#value' => $pruebas['Prueba_Jugador_Visitante'],
      );
  
      //Agrega los botones para decidir ganador
      $form['home_team']['button'] = array(
        '#type' => 'submit',
        '#value' => t('Award Win to This Team'),
        '#validate' => array('modalidad_match_conflict_validate'),
        '#submit' => array('modalidad_match_conflict_win_home_team'),
      );
      $form['away_team']['button'] = array(
        '#type' => 'submit',
        '#value' => t('Award Win to This Team'),
        '#validate' => array('modalidad_match_conflict_validate'),
        '#submit' => array('modalidad_match_conflict_win_away_team'),
      );
      
      //Mas opciones
      $form['options'] = array (
        '#type' => 'fieldset',
        '#title' => t('More Options'),
        '#collapsible' => FALSE,
        '#attributes' => array ('class' => 'match_options'),
      );
      $form['options']['cancel'] = array (
        '#type' => 'item',
        '#value' => l('Cancel Match', 'node/'. $reto->nid_Reto .'/cancel'),
      );  
      if($reto = db_fetch_object($retos)) {
        ++$offset;
        
        $form['options']['skip'] = array (
          '#type' => 'item',
          '#value' => l('Skip Match', 'taxonomy/term/'. $tid .'/match/conflict/skip/'. $offset),
        );
      }
  }
  else {
      $form['Mensaje'] = array (
          '#value' => t('No existen desafios '. $op .'.'),
      );
  }
  
  return $form;
}
/**
 * Función para validar que el desafío efectivamente esté todavía en estado de conflicto
 */
function modalidad_match_conflict_validate($form, $form_state) {
  db_set_active('eSM');
	$reto = db_fetch_object(db_query( 'SELECT Estado FROM {Reto} WHERE nid_Reto = %d', $form_state['values']['nid_Reto']));
  db_set_active('default');
  
  if(!($reto->Estado == CONFLICTO))
    form_set_error('nid_Reto', t('Este desafio ya ha sido reportado.'));
}
/**
 * Función para dar el triunfo al equipo local
 */
function modalidad_match_conflict_win_home_team($form, $form_state) {
  _match_end_match($form_state['values']['nid_Reto'], $form_state['values']['home_team_nid']);
}
/**
 * Función para dar el triunfo al equipo visitante
 */
function modalidad_match_conflict_win_away_team($form, $form_state) {
  _match_end_match($form_state['values']['nid_Reto'], $form_state['values']['away_team_nid']);
}