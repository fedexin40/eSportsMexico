<?php

/**
 *	Función para mostrar la puntuación de una modalidad
 */
function mostrar_puntuacion(  ) {
	module_load_include( 'php', 'base_datos_externa', 'db_functions' );
	
	$tid = arg( 1 );
	$modalidad = term_is_active($tid);
	
	db_set_active( 'eSM' );
		$Puntuacion_result = db_query( "SELECT { * } FROM { Puntuacion } AS { p } INNER JOIN { Equipo } AS { e } ON { p.nid_Equipo } = { e.nid_Equipo } WHERE { e.id_Modalidad_Juego_Temporada } = %d ORDER BY {p.Puntuacion} DESC", $modalidad->id_Modalidad_Juego_Temporada );
		//$Puntuacion_result = ranking_get_ranking($modalidad->id_Modalidad_Juego_Temporada);
	db_set_active( 'default' );	
	
	$count = 0;
	//Primero mostraremos todos los equipos
	while( $Puntuacion = db_fetch_object( $Puntuacion_result ) )
	{
		++$count;
		$Equipo = node_load($Puntuacion->nid_Equipo);
		$actividad = get_actividad($Equipo->nid);
		
		if ($Puntuacion->PJ > 0)
			$victorias = $Puntuacion->PG / $Puntuacion->PJ;
		else
			$victorias = 0;

		$form[ $count ][ 'Ranking' ] = array (
			//'#title' => 'Nombre',
			'#type' => 'item',
			'#value' => $count,
		);
		
		$form[ $count ][ 'Title' ] = array (
			//'#title' => 'Nombre',
			'#type' => 'item',
			'#value' => l($Equipo->title, 'node/'.$Equipo->nid),
		);
		
		$form[ $count ][ 'Puntuacion' ] = array (
			//'#title' => 'Puntuacion',
			'#type' => 'item',
			'#value' => $Puntuacion->Puntuacion
		);
		

		$form[ $count ][ 'PJ' ] = array (
			//'#title' => 'Desafios Jugados',
			'#type' => 'item',
			'#value' => $Puntuacion->PJ
		);

		$form[ $count ][ 'PG' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->PG
		);
		
		$form[ $count ][ 'PP' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->PJ - $Puntuacion->PG
		);

		$form[ $count ][ 'TJ' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->TJ
		);
		
		$form[ $count ][ 'Victorias' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $victorias
		);
		
		$form[ $count ][ 'Actividad' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $actividad,
		);
		
		$form[ $count ][ 'Racha' ] = array (
			//'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->Racha
		);
		
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
	
		$time = db_fetch_array( db_query( 'SELECT ( max( Fecha_Inicio ) ) AS { numdays } FROM { Reto } AS { r } INNER JOIN { Inicio_Reto } AS { ir } ON { r.id_Reto = ir.id_Reto } WHERE ( ir.nid_Equipo_Local = %d } OR  { ir.nid_Equipo_Visitante = %d } )', $nid_Equipo, $nid_Equipo ) );
	
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