<?php
// $Id$


/**
 *	@file
 *	Form callbacks for Modalidad_Juego_Temporada table
 */
	

	/**
	 * Page callback function for listing the rows from Modalidad_Juego existing in the table
	 * as the name in the file, and inserting information in it too.
	 */
	function modalidad_juego_temporada_admin(  ) {
		// Creating the date/time element starts here
		
		// Provide a default date in the formate YYYY-MM-DD HH:MM:SS.
		$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		db_set_active( 'eSM' );
		$result_set_Temporada = db_query( "SELECT * FROM {Temporada}" );
		$result_set_array_Temporada = db_fetch_array( db_query( "SELECT * FROM {Temporada}" ) );		
		db_set_active( 'default' );


		$Existe_Temporada_Activa = 0;

		while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
			
			if( $Temporada->Estado == 1 ) {
				$Existe_Temporada_Activa = 1;
				break;
			}
		}
		
		$form[ 'Estado_Temporadas' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Estado de las temporadas' ),
			//'#collapsible' => 'TRUE',
			'#collapsed' => 'FALSE',
		
		);
		
		
		if( $Existe_Temporada_Activa == 1 ) {

			
			db_set_active( 'eSM' );
			$result_set_Temporada = db_query( "SELECT * FROM {Temporada} WHERE {Temporada.Estado} = %d", 1 );
			$result_set_array_Temporada = db_fetch_array( db_query( "SELECT * FROM {Temporada}" ) );		

			db_set_active( 'default' );
			
			$Temporada = db_fetch_object( $result_set_Temporada );
			
			//dpm( $Temporada );
						
			$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
			$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );
			
			$form[ 'Estado_Temporadas' ][ 'Listar_Temporada_Activa' ][ 'title' ] = array (
				'#type' => 'item',
				//'label' => t( 'Nombre' ),
				//'#disabled' => 'TRUE',
				//'size' => 17,
				//'maxlength' => 17,
				'#title' => t( 'Temporada activa' ),
				//'#required' => TRUE,
				'#weight' => -8,
				'#value' => $Temporada->Nombre."\n",
			);			
			
			$form[ 'Estado_Temporadas' ][ 'Listar_Temporada_Activa' ][ 'fecha_inicio' ] = array (
				'#type' => 'date_text', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
				'#disabled' => 'TRUE',
				'#title' => 'Fecha de inicio',
				'#default_value' => $fecha_inicio,
				'#date_format' => $format,
				'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
				'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
				'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
				'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
				'#size' => 17,
				'#maxlength' => 17,
			);
			
			$form[ 'Estado_Temporadas' ][ 'Listar_Temporada_Activa' ][ 'fecha_fin' ] = array (
				'#type' => 'date_text', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
				'#disabled' => 'TRUE',
				'#title' => 'Fecha de finalización',
				'#default_value' => $fecha_fin,
				'#date_format' => $format,
				'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
				'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
				'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
				'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
				'#size' => 17,
				'#maxlength' => 17,
			);
						
			$form[ 'Estado_Temporadas' ][ 'Listar_Temporada_Activa' ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/temporadas/'.$Temporada->id_Temporada.'/editar' ),
			);

		}
		else {
			
			
			db_set_active( 'eSM' );		
				$result_set_Temporada = db_query( "SELECT * FROM {Temporada}" );
			db_set_active( 'default' );
			
			
			while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
				$Temporada_Nombres[ $Temporada->id_Temporada ] = $Temporada->Nombre;			
			}

			$form[ 'Estado_Temporadas' ][ 'Establecer_temporada_activa' ][ 'Temporadas_Nombres' ]  = array (
				
				'#type' => 'select',
				'#title' => t( 'No existe una temporada activa. Estas son las temporadas disponibles' ),
				'#options' => $Temporada_Nombres,
			);
			
			
			$form[ 'Estado_Temporadas' ][ 'Establecer_temporada_activa' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Activar' ),
			);
		}	
	
		
		return $form;
		
	}
	

	/**
	 *	Función para editar una modalidad
	 */
	function modalidad_juego_temporada_admin_editar( ) {
		
		
		$id_Modalidad_Juego_Temporada = arg( 3 );
		
		
		db_set_active( 'eSM' );
		$result_set_Modalidad_Juego_Temporada = db_query( "SELECT * FROM { Modalidad_Juego_Temporada } WHERE { id_Modalidad_Juego_Temporada } = %d", $id_Modalidad_Juego_Temporada );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM { Modalidad_Juego }" );
		$result_set_Temporada = db_query( "SELECT * FROM { Temporada }" );
		db_set_active( 'default' );

		$Modalidad_Juego_Temporada = db_fetch_object( $result_set_Modalidad_Juego_Temporada );



		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) )
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = $Modalidad_Juego->Nombre;		
		
		//Primero pondremos un listado de modalidades disponibles
		$form[ 'Modalidad_Juego' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Modalidad de Juego' ),
			'#options' => $Modalidad_Juego_Nombres,
			'#default_value' => $Modalidad_Juego_Temporada->id_Modalidad_Juego,
		);
		
		
		while( $Temporada = db_fetch_object( $result_set_Temporada ) )
			$Temporada_Nombres[ $Temporada->id_Temporada ] = $Temporada->Nombre;
			
		$form[ 'Temporada' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Temporada' ),
			'#options' => $Temporada_Nombres,
			'#default_value' => $Modalidad_Juego_Temporada->id_Temporada,
		);
		
		
		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Insertar' ),
		);
		
		$form[ 'id_Modalidad_Juego_Temporada' ] = array(
			'#type' => 'hidden',
			'#value' => $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada,
		);
		
				
		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Guardar' ),
			
		);
			
			
		
		$form[ '#validate' ] = array( 'modalidad_juego_temporada_admin_validate' );
		
		return $form;
		
		
	}



	/**
	 *	Sumbit para la forma de edición
	 */
	function modalidad_juego_temporada_admin_editar_submit( $form, $form_state ) {		
		
		//dpm( $form_state[ 'values' ][ 'id_Modalidad_Juego_Temporada' ] );
		
		db_set_active( 'eSM' );
		
		$result_set_Modalidad_Juego_Temporada = db_query( "SELECT * FROM {Modalidad_Juego_Temporada} WHERE {id_Modalidad_Juego_Temporada} = %d", $form_state[ 'values' ][ 'id_Modalidad_Juego_Temporada' ] );
		
		
		$Temporada = db_fetch_object( $result_set_Modalidad_Juego_Temporada );
		
		
		db_query
		(
			
			"UPDATE
			{Modalidad_Juego_Temporada}
			SET
			id_Modalidad_Juego =  %d,
			id_Temporada =  %d
			WHERE
			id_Modalidad_Juego_Temporada = %d",
			$form_state[ 'values' ][ 'Modalidad_Juego' ],
			$form_state[ 'values' ][ 'Temporada' ],
			$form_state[ 'values' ][ 'id_Modalidad_Juego_Temporada' ]

		);
				
		db_set_active( 'default' );
		
		drupal_set_message( "La modalidad fué guardada satisfactoriamente en esta temporada" );
		drupal_goto( "admin/base_datos_externa/modalidades_juegos_temporadas" );
				
	}
	
	
	/**
	 *	Función para listar las modalidades de juego por temporada
	 */
	function modalidades_juegos_temporadas_admin_listar ( ) {
		
		
		/*$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		db_set_active( 'eSM' );
		$result_set_Modalidad_Juego_Temporada = db_query( "SELECT * FROM {Modalidad_Juego_Temporada} ORDER BY id_Modalidad_Juego ASC" );
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de modalidades disponibles
		while( $Modalidad_Juego_Temporada = db_fetch_object( $result_set_Modalidad_Juego_Temporada ) ) {

			db_set_active( 'eSM' );
			$result_set_Modalidad_Juego = db_query( "SELECT * from {Modalidad_Juego} WHERE id_Modalidad_Juego = %d ", $Modalidad_Juego_Temporada->id_Modalidad_Juego );
			db_set_active( 'default' );
			
			$Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego );

			$form[ 'listar' ][ $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada ][ 'title' ] = array (
				'#type' => 'item',
				'#title' => t( 'Modalidad de juego' ),
				'#weight' => -8,
				'#value' => $Modalidad_Juego->Nombre."\n",
			);			
			
			db_set_active( 'eSM' );
			$result_set_Temporada = db_query( "SELECT * from {Temporada} WHERE id_Temporada = %d ", $Modalidad_Juego_Temporada->id_Temporada );
			db_set_active( 'default' );
			
			$Temporada = db_fetch_object( $result_set_Temporada );
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'id_plataforma' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Temporada' ),
				'#value' => $Temporada->Nombre,
			);

			
			$form[ 'listar' ][ $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/modalidades_juegos_temporadas/'.$Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada.'/editar' ),
			);
			
			
			$form[ 'listar' ][ $Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada ][ 'eliminar' ] = array (
				'#value' => l( t( 'Eliminar' ), 'admin/base_datos_externa/modalidades_juegos_temporadas/'.$Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada.'/eliminar' ),
			);

		}
		
		return $form;
	}
	
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla modalidad de juego por temporada eliminar
	 */
	function modalidades_juegos_temporadas_admin_eliminar( ) {
		
		$id_Modalidad_Juego_Temporada = arg( 3 );
		
		db_set_active( 'eSM' );
		
		$Modalidad_Juego_Temporada_result = db_query
		(
			"SELECT
					{ * }
				FROM
					{ Modalidad_Juego_Temporada }
			"
		);
		
		$Modalidad_Juego_Temporada = db_fetch_object( $Modalidad_Juego_Temporada_result );
		
		
		db_query
		(
			"DELETE FROM
					{ Modalidad_Juego_Temporada }
				WHERE
					id_Modalidad_Juego_Temporada = %d
				AND
					id_Modalidad_Juego = %d
				AND
					id_Temporada = %d
			",
			$Modalidad_Juego_Temporada->id_Modalidad_Juego_Temporada,
			$Modalidad_Juego_Temporada->id_Modalidad_Juego,
			$Modalidad_Juego_Temporada->id_Temporada
		);
		
		
		db_set_active( 'default' );
		
		drupal_set_message( "La modalidad fué eliminada satisfactoriamente en esta temporada" );
		drupal_goto( "admin/base_datos_externa/modalidades_juegos_temporadas" );
		
	}
	
	
	
	
	/**
	 *	Función para insertar datos en la tabla modalidades de juegos por temporadas
	 */
	//Un nuevo fieldset para insertar nuevas modalidades
	function modalidades_juegos_temporadas_admin_insertar ( ) {
		
		
		/*$form[ 'insertar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Insertar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/



		db_set_active( 'eSM' );		
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM Modalidad_Juego" );
		db_set_active( 'default' );

		
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) )
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = $Modalidad_Juego->Nombre;
		
		
		$form[ 'insertar' ][ 'Modalidad_Juego' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Modalidad de juego' ),
			'#options' => $Modalidad_Juego_Nombres,
		);

		db_set_active( 'eSM' );		
		$result_set_Temporada = db_query( "SELECT * FROM Temporada" );
		db_set_active( 'default' );

		
		while( $Temporada = db_fetch_object( $result_set_Temporada ) )
			$Temporada_Nombres[ $Temporada->id_Temporada ] = $Temporada->Nombre;

		
		$form[ 'insertar' ][ 'Temporada' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Temporada' ),
			'#options' => $Temporada_Nombres,
		);
				
		$form[ 'insertar' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Activar' ),
		);
			
			
		return $form;
	}
	
	
	
	
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla modalidad de juego por temporada
	 */
	function modalidades_juegos_temporadas_admin_insertar_submit( $form, $form_state ) {
		
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Modalidad_Juego_Temporada } 
			( 
				id_Modalidad_Juego,
				id_Temporada			
			) 
				VALUES ( %d, %d )",
				$form_state[ 'values' ][ 'Modalidad_Juego' ],
				$form_state[ 'values' ][ 'Temporada' ]
		);
		
		db_set_active( 'default' );
		
		drupal_set_message( "La modalidad fué activada satisfactoriamente en esta temporada" );
		drupal_goto( "admin/base_datos_externa/modalidades_juegos_temporadas" );
		
	}

	
	
	
	/**
	 *	Introduce en la tabla de temporada la información
	 */
	function modalidad_juego_temporada_admin_submit( $form, $form_state ) {
	
		
		
		$id_Temporada = $form_state[ 'values' ][ 'Temporadas_Nombres' ];
				
		db_set_active( 'eSM' );
			$result_set_Temporada = db_query( "SELECT * FROM {Temporada} ORDER BY Fecha_Inicio ASC" );
		db_set_active( 'default' );
			
		//Primero pondremos un listado de temporadas disponibles
		while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
			if( $Temporada->Estado == 1 ) {
				
				$activaAnterior = $Temporada->id_Temporada;
				db_set_active( 'eSM' );
		
					db_query
					(
						"UPDATE {Temporada}
						SET
						Estado = %d
						WHERE
						id_Temporada = %d",
						0,
						$activaAnterior
					);
		
				db_set_active( 'default' );
			}
		}
				

		db_set_active( 'eSM' );
		db_query
		(
			"UPDATE
			{ Temporada }
			SET
			{ Estado } =  %d
			WHERE
			{ Temporada.id_Temporada} = %d",
			1,
			$id_Temporada
		);
				
		db_set_active( 'default' );
		
		
		
	}