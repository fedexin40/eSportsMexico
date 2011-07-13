<?php
// $Id$


/**
 *	@file
 *	Form callbacks for Mapa table
 */

	

	/**
	 * Page callback function for listing the rows from Mapa existing in the table
	 * as the name in the file, and inserting in it too.
	 */
	function mapa_admin(  ) {
		// Creating the date/time element starts here
		
		
		$form[ 'informacion' ] = array (
			'#type' => 'item',
			'#disabled' => FALSE,
			'#value' => 'Panel de administración de los mapas',
		);

		
		return $form;
	}
	
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla mapa
	 */
	function mapas_admin_insertar_submit( $form, $form_state ) {
				
				
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Mapa } 
			(
				Nombre,
				id_Modalidad_Juego
			) 
				VALUES ( '%s', %d )",
				$form_state[ 'values' ][ 'title' ],
				$form_state[ 'values' ][ 'Modalidad_Juego' ]
		);
		
		db_set_active( 'default' );
		
		drupal_set_message( "El mapa fué insertado satisfactoriamente en esta modalidad" );
		drupal_goto( "admin/base_datos_externa/mapas" );
		
	}
	

	/**
	 *	Función para editar una mapa
	 */
	function mapa_admin_editar( ) {
				
		$id_Mapa = arg( 3 );
	
		
		//dpm( $id_Mapa );
		
		//Hacemos la consulta para las modalidades
		
		db_set_active( 'eSM' );		
		$result_set_Mapa = db_query( "SELECT * FROM {Mapa} WHERE {id_Mapa} = %d", $id_Mapa );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM {Modalidad_Juego}" );
		db_set_active( 'default' );
		
		$Mapa = db_fetch_object( $result_set_Mapa );
		
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = $Modalidad_Juego->Nombre;
			
		}
		
			
		$form[ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $Mapa->Nombre,
		);
					
		$form[ 'Modalidad_Juego' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Modalidad de juego' ),
			'#options' => $Modalidad_Juego_Nombres,
			'#default_value' => $Mapa->id_Modalidad_Juego,
		);

		$form[ 'id_Mapa' ] = array(
			'#type' => 'hidden',
			'#value' => $Mapa->id_Mapa,
		);

		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Guardar' ),
		);
		
		
		return $form;

	}



	/**
	 *	Sumbit para la forma de edición
	 */
	function mapa_admin_editar_submit( $form, $form_state ) {		
		
		db_set_active( 'eSM' );
			
		db_query
		(
			"UPDATE {Mapa}
			SET
			id_Modalidad_Juego = %d,
			Nombre = '%s'
			WHERE
			id_Mapa = %d",
			$form_state[ 'values' ][ 'Modalidad_Juego' ],
			$form_state[ 'values' ][ 'title' ],
			$form_state[ 'values' ][ 'id_Mapa' ]
		);
				
		db_set_active( 'default' );
		
		$form[ '#validate' ] = array( 'mapa_admin_insertar_validate' );
		
		drupal_set_message( "El mapa fué insertado satisfactoriamente en esta modalidad" );
		drupal_goto( "admin/base_datos_externa/mapas" );
		
	}
	
	
	/**
	 * Función para validar:
	 */
	function mapas_admin_insertar_validate( $form, $form_state ) {
		
		$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
		
	}
	
	
	
	
	/**
	 *	Función para listar los mapas
	 */
	function mapas_admin_listar( ) {
		
		/*$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		db_set_active( 'eSM' );
		$result_set_Mapa = db_query( "SELECT {*} FROM {Mapa} ORDER BY {id_Mapa} ASC" );
		db_set_active( 'default' );		
		
		//Primero pondremos un listado de mapas disponibles
		while( $Mapa = db_fetch_object( $result_set_Mapa ) ) {
		
			db_set_active( 'eSM' );		
			$result_set_Modalidad_Juego = db_query( "SELECT {Nombre} FROM {Modalidad_Juego} WHERE {id_Modalidad_Juego} = %d", $Mapa->id_Modalidad_Juego );
			db_set_active( 'default' );

			$Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego );

			$form[ 'listar' ][ $Mapa->id_Mapa ][ 'title' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Nombre' ),
				'#weight' => -8,
				'#value' => $Mapa->Nombre."\n",
			);
						
			$form[ 'listar' ][ $Mapa->id_Mapa ][ 'modalidad_juego' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Modalidad de juego' ),
				//'#weight' => -8,
				'#value' => $Modalidad_Juego->Nombre."\n",
			);
			
			$form[ 'listar' ][ $Mapa->id_Mapa ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/mapas/'.$Mapa->id_Mapa.'/editar' ),
			);

		}
		
		return $form;
	}
	
	
	
	function mapas_admin_insertar( ) {
		
				//Hacemos la consulta para las modalidades
		$Modalidad_Juego = array( );
		
		db_set_active( 'eSM' );		
		$result_set_Modalidad_Juego = db_query( "SELECT {*} FROM {Modalidad_Juego}" );
		
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) )
			//dsm( $row );
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = /*$row->id_Modalidad_Juego . ' : ' . */$Modalidad_Juego->Nombre;
		
		//$modalidades = db_fetch_array( $query_result ) );
		db_set_active( 'default' );

				
		//Un nuevo fieldset para insertar nuevas mapas	
		/*form[ 'insertar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Insertar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		
		$form[ 'insertar' ][ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'Modalidad_Juego' ] = array
		(
			'#type' => 'select',
			'#title' => 'Modalidad de juego',
			'#options' => $Modalidad_Juego_Nombres,
		);

		$form[ 'insertar' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Insertar' ),
		);
				
		return $form;
	
	}