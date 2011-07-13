<?php
// $Id$


/**
 *	@file
 *	Form callbacks for Plataforma table
 */

	

	/**
	 * Page callback function for listing the rows from Plataforma existing in the table
	 * as the name in the file, and inserting in it too.
	 */
	function plataforma_admin(  ) {

		$form[ 'informacion' ] = array (
			'#type' => 'item',
			'#disabled' => FALSE,
			'#value' => 'Panel de administración de las plataformas',
		);

		
		return $form;
	}
	
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla plataforma
	 */
	function plataforma_admin_insertar_submit( $form, $form_state ) {
		
		
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Plataforma } 
			( 
				Nombre
			) 
				VALUES ( '%s' )",
				$form_state[ 'values' ][ 'title' ]
		);
		
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué insertada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/plataformas" );
		
	}
	

	/**
	 *	Función para editar una plataforma
	 */
	function plataforma_admin_editar( ) {
		
		$id_Plataforma = arg( 3 );
		
		
		
		db_set_active( 'eSM' );
		$result_set_Plataforma = db_query( "SELECT * FROM {Plataforma} WHERE {Plataforma.id_Plataforma} = %d", $id_Plataforma );
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de plataformas disponibles
		$Plataforma = db_fetch_object( $result_set_Plataforma );

		
		$form[ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t('Nombre'),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $Plataforma->Nombre,
		);
		
		
		$form[ 'id_Plataforma' ] = array(
			'#type' => 'hidden',
			'#value' => $Plataforma->id_Plataforma,
		);
			

		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Guardar' ),
			
		);
		
								
		$form[ '#validate' ] = array( 'plataforma_admin_list_validate' );
				
		return $form;
			
	}




	/**
	 *	Sumbit para la forma de edición
	 */
	function plataforma_admin_editar_submit( $form, $form_state ) {
		
		db_set_active( 'eSM' );
			
		db_query
		(
			"UPDATE {Plataforma}
			SET
			Nombre = '%s'
			WHERE
			id_Plataforma = %d",
			$form_state[ 'values' ][ 'title' ],
			$form_state[ 'values' ][ 'id_Plataforma' ]
		);
				
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué guardada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/plataformas" );
		
		
	}
	
	
	/**
	 * Función para validar:
	 * La fecha de fin sea mayor que la fecha de inicio
	 * La fecha de inicio sea mayor o igual que la fecha actual
	 * La fechas de plataformas no abarquen periodos compartidos de tiempo
	 */
	function plataforma_admin_list_validate( $form, $form_state ) {
		
		$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
			
	}
	
	
	
	
	/**
	 *	Función para insertar en la plataforma
	 */
	function plataforma_admin_listar( ) {
		
		
		/*$form[ 'listar' ] = array
		(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		
		db_set_active( 'eSM' );
		$result_set_Plataforma = db_query( "SELECT * FROM {Plataforma}" );	
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de plataformas disponibles
		while( $Plataforma = db_fetch_object( $result_set_Plataforma ) ) {

			$form[ 'listar' ][ $Plataforma->id_Plataforma ][ 'title' ] = array (
				
				'#type' => 'item',
				'#title' => t( 'Nombre' ),
				'#weight' => -8,
				'#value' => $Plataforma->Nombre."\n",
				
			);			
			
			$form[ 'listar' ][ $Plataforma->id_Plataforma ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/plataformas/'.$Plataforma->id_Plataforma.'/editar' ),
			);
		}
		
		return $form;
	}
	
	
	function plataforma_admin_insertar ( ) {	
		
			/*$form[ 'insertar' ] = array
		(
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
			
		$form[ 'insertar' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Insertar' ),
		);
		
		
		
		return $form;
	}
	
