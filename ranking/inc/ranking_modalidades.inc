<?php

/**
 *	Función para mostrar la puntuación de una modalidad
 */
function mostrar_puntuacion(  ) {
	$tid = arg( 2 );
	$modalidad = term_is_active($tid);
	
	drupal_set_title(t('Ranking de '. $modalidad->Nombre));
	
	$Puntuacion_result = ranking_get_ranking($modalidad->id_Modalidad_Juego_Temporada);
	
	$count = 0;
	//Primero mostraremos todos los equipos
	while( $Puntuacion = db_fetch_object( $Puntuacion_result ) ) {
	  $Equipo = node_load($Puntuacion->nid_Equipo);
	  
	  if($Equipo->Numero_Integrantes >= $modalidad->Minimo_Jugadores) {
		++$count;
		
		$actividad = get_actividad($Equipo->nid);
		
		if ($Puntuacion->PJ > 0)
			$victorias = $Puntuacion->PG / $Puntuacion->PJ;
		else
			$victorias = 0;

		$form[ $count ][ 'Ranking' ] = array (
			'#type' => 'item',
			'#value' => $count,
		);
		
		$form[ $count ][ 'Image' ] = array (
			'#type' => 'item',
			'#value' => theme('team_pane', $Equipo, variable_get('team_lists_picture_preset', '')),
		);
		
		$form[ $count ][ 'Title' ] = array (
			'#type' => 'item',
			'#value' => l($Equipo->title, 'node/'.$Equipo->nid),
		);
		
		$form[ $count ][ 'Puntuacion' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->Puntuacion
		);

		$form[ $count ][ 'PJ' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->PJ
		);

		$form[ $count ][ 'PG' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->PG
		);
		
		$form[ $count ][ 'PP' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->PJ - $Puntuacion->PG
		);

		$form[ $count ][ 'TJ' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->TJ
		);
		
		$form[ $count ][ 'Victorias' ] = array (
			'#type' => 'item',
			'#value' => $victorias
		);
		
		$form[ $count ][ 'Actividad' ] = array (
			'#type' => 'item',
			'#value' => $actividad,
		);
		
		$form[ $count ][ 'Racha' ] = array (
			'#type' => 'item',
			'#value' => $Puntuacion->Racha
		);
	  }
	}
	
	if ($count == 0) {
		$form['Mensaje'] = array (
			'#value' => t('No hay equipos participando en este ranking todavia.'),
		);
	}
	
	$form['#theme'] = 'ranking_puntuacion';
	return $form;
}
/**
 * Función para saber la actividad reciente de un equipo
 */
function get_actividad($nid_Equipo) {
    db_set_active('eSM');
	$time = db_fetch_array(db_query('SELECT (max( Fecha_Inicio )) AS numdays FROM { Reto } AS r
					INNER JOIN {Inicio_Reto} AS ir ON r.nid_Reto = ir.nid_Reto
					WHERE ((ir.nid_Equipo_Local = %d OR ir.nid_Equipo_Visitante = %d) && r.Estado = %d)',
					$nid_Equipo, $nid_Equipo, COMPLETADO));
    db_set_active( 'default' );
	
    $matches = time( ) - $time[ 'numdays' ];
    $image_path = drupal_get_path('module', 'ranking') . '/images/';
	
    if ($matches=="" || $matches > 2592000 ) 
      $daysGraph = "<img alt='No matches played in last 30 days' src='/" . $image_path . "graph1.gif'>";
    elseif ($matches <= 172800) 
      $daysGraph = "<img alt='They have played a match within the last 2 days' src='/" . $image_path . "graph7.gif'>";
    elseif ($matches <= 604800) 
      $daysGraph = "<img alt='They have played a match within the last 7 days' src='/" . $image_path . "graph6.gif'>";
    elseif ($matches <= 864000) 
      $daysGraph = "<img alt='They have played a match within the last 10 days' src='/" . $image_path . "graph5.gif'>";
    elseif ($matches <= 1209600) 
      $daysGraph = "<img alt='They have played a match within the last days' src='/" . $image_path . "graph4.gif'>";
    elseif ($matches <= 1814400) 
      $daysGraph = "<img alt='They have played a match within the last 21 days' src='/" . $image_path . "graph3.gif'>";
    else 
      $daysGraph = "<img alt='They have played a match within the last 30 days' src='/" . $image_path . "graph2.gif'>";
	  
	return $daysGraph;
}