<?php

/**
 *	Función para mostrar la puntuación de una modalidad
 */
function mostrar(  )
{
	
	dsm( "Hola" );
	
	$id_Modalidad_Juego_Temporada = arg( 1 );
	
	
	drupal_set_message(  "Aqui" );
	
	$form[ 'puntuacion' ] = array
	(
		'#title' => t( 'Puntuacion.' ),
		'#type' => 'fieldset',
		//'#default_value' => $Equipos->Nombre,
		//'#options' => $options,
		'#collapsible' => 0,
	);
	
	db_set_active( 'eSM' );
	
		$Puntuacion_result = db_query( "SELECT { * } FROM { Puntuacion } AS { p } INNER JOIN { Equipo } AS { e } ON { p.nid_Equipo } = { e.nid_Equipo } INNER JOIN { Modalidad_Juego_Temporada } AS { m } ON { e.id_Modalidad_Juego_Temporada } = { m.id_Modalidad_Juego_Temporada } WHERE { p.nid_Equipo } = %d ", arg( 2 ) );
		
	db_set_active( 'default' );	
	
	//Primero mostraremos todos los equipos
	while( $Puntuacion = db_fetch_object( $Puntuacion_result ) )
	{
		
		
		$form[ 'puntuacion' ][ 'equipos' ] = array
		(
			'#title' => t( 'Equipos' ),
			'#type' => 'fieldset',
			'collapsible' => 1,
		);
		
		db_set_active( 'eSM' );
		
			$Equipo_result = db_query( "SELECT { * } FROM Equipo WHERE { Equipo.nid_Equipo } = %d", $Puntuacion->nid_Equipo );
		
		db_set_active( 'default' );
		
		
		$Equipo = db_fetch_object( $Equipo_result );
		
		$form[ 'puntuacion' ][ 'equipos' ][ 'Abreviacion' ] = array (
			'#title' => 'Abreviacion',
			'#type' => 'item',
			'#value' => $Equipo->Abreviacion
			
		);
		
		$form[ 'puntuacion' ][ 'equipos' ][ 'Puntuacion' ] = array (
			'#title' => 'Puntuacion',
			'#type' => 'item',
			'#value' => $Puntuacion->Puntuacion
			
		);
		

		$form[ 'puntuacion' ][ 'equipos' ][ 'PJ' ] = array (
			'#title' => 'Desafios Jugados',
			'#type' => 'item',
			'#value' => $Puntuacion->PJ
			
		);

		$form[ 'puntuacion' ][ 'equipos' ][ 'PG' ] = array (
			'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->PG
			
		);

		$form[ 'puntuacion' ][ 'equipos' ][ 'Racha' ] = array (
			'#title' => 'Desafíos Ganados',
			'#type' => 'item',
			'#value' => $Puntuacion->Racha
			
		);
		
		drupal_set_message(  "Aqui" );
		
	}
	
	drupal_set_message(  "Aqui" );
	return $form;
}