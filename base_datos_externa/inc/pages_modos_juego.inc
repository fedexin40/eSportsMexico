<?php
// $Id$


/**
 *	@file
 *	Form callbacks for modos_juego table
 */

	

	/**
	 * Page callback function for listing the rows from modos_juego existing in the table
	 * as the name in the file, and inserting in it too.
	 */
	function modos_juego_admin(  ) {
		// Creating the date/time element starts here
		
		
		$form[ 'informacion' ] = array (
			'#type' => 'item',
			'#disabled' => FALSE,
			'#value' => 'Panel de administración de los modos de juego',
		);

		
		return $form;
	}
	
	
	function modos_juego_admin_insertar( ) {
		
				//Hacemos la consulta para las modalidades
		$Modalidad_Juego = array( );
		
		db_set_active( 'eSM' );		
		$result_set_Modalidad_Juego = db_query( "SELECT { * } FROM { Modalidad_Juego }" );
		
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) )
			//dsm( $row );
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = /*$row->id_Modalidad_Juego . ' : ' . */$Modalidad_Juego->Nombre;
		
		//$modalidades = db_fetch_array( $query_result ) );
		db_set_active( 'default' );

				
		//Un nuevo fieldset para insertar nuevas modos_juegos	
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
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla modos_juego
	 */
	function modos_juego_admin_insertar_submit( $form, $form_state ) {
				
				
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Modo_Juego } 
			(
				Nombre,
				id_Modalidad_Juego
			) 
				VALUES ( '%s', %d )",
				$form_state[ 'values' ][ 'title' ],
				$form_state[ 'values' ][ 'Modalidad_Juego' ]
		);
		
		db_set_active( 'default' );
		
		drupal_set_message( "El modo de juego fué insertado satisfactoriamente en esta modalidad" );
		drupal_goto( "admin/base_datos_externa/modos_juego" );
		
	}
	

	/**
	 *	Función para editar una modos_juego
	 */
	function modos_juego_admin_editar( ) {
				
		$id_Modo_Juego = arg( 3 );
	
		
		//dpm( $id_modos_juego );
		
		//Hacemos la consulta para las modalidades
		
		db_set_active( 'eSM' );		
		$result_set_modos_juego = db_query( "SELECT * FROM {Modo_Juego} WHERE {id_Modo_Juego} = %d", $id_Modo_Juego );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM {Modalidad_Juego}" );
		db_set_active( 'default' );
		
		$modos_juego = db_fetch_object( $result_set_modos_juego );
		
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {
			$Modalidad_Juego_Nombres[ $Modalidad_Juego->id_Modalidad_Juego ] = $Modalidad_Juego->Nombre;
			
		}
		
			
		$form[ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $modos_juego->Nombre,
		);
					
		$form[ 'Modalidad_Juego' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Modalidad de juego' ),
			'#options' => $Modalidad_Juego_Nombres,
			'#default_value' => $modos_juego->id_Modalidad_Juego,
		);

		$form[ 'id_Modo_juego' ] = array(
			'#type' => 'hidden',
			'#value' => $modos_juego->id_Modo_Juego,
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
	function modos_juego_admin_editar_submit( $form, $form_state ) {		
		
		db_set_active( 'eSM' );
			
		db_query
		(
			"UPDATE {Modo_juego}
			SET
			id_Modalidad_Juego = %d,
			Nombre = '%s'
			WHERE
			id_Modo_juego = %d",
			$form_state[ 'values' ][ 'Modalidad_Juego' ],
			$form_state[ 'values' ][ 'title' ],
			$form_state[ 'values' ][ 'id_Modo_Juego' ]
		);
				
		db_set_active( 'default' );
		
		$form[ '#validate' ] = array( 'modos_juego_admin_insertar_validate' );
		
		drupal_set_message( "El modo de juego fué insertado satisfactoriamente en esta modalidad" );
		drupal_goto( "admin/base_datos_externa/modos_juego" );
		
	}
	
	
	/**
	 * Función para validar:
	 */
	function modos_juego_admin_insertar_validate( $form, $form_state ) {
		
		$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
		
	}
	
	
	
	
	/**
	 *	Función para listar los modos_juegos
	 */
	function modos_juego_admin_listar( ) {
		
		/*$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		db_set_active( 'eSM' );
		$result_set_modos_juego = db_query( "SELECT {*} FROM {Modo_Juego} ORDER BY {id_Modo_Juego} ASC" );
		db_set_active( 'default' );		
		
		//Primero pondremos un listado de modos_juegos disponibles
		while( $modos_juego = db_fetch_object( $result_set_modos_juego ) ) {
		
			db_set_active( 'eSM' );		
			$result_set_Modalidad_Juego = db_query( "SELECT {Nombre} FROM {Modalidad_Juego} WHERE {id_Modalidad_Juego} = %d", $modos_juego->id_Modalidad_Juego );
			db_set_active( 'default' );

			$Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego );

			$form[ 'listar' ][ $modos_juego->id_Modo_Juego ][ 'title' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Nombre' ),
				'#weight' => -8,
				'#value' => $modos_juego->Nombre."\n",
			);
						
			$form[ 'listar' ][ $modos_juego->id_Modo_Juego ][ 'modalidad_juego' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Modalidad de juego' ),
				//'#weight' => -8,
				'#value' => $Modalidad_Juego->Nombre."\n",
			);
			
			$form[ 'listar' ][ $modos_juego->id_Modo_Juego ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/modos_juego/'.$modos_juego->id_Modo_Juego.'/editar' ),
			);

		}
		
		return $form;
	}