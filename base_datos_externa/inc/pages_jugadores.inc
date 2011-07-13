<?php
// $Id$

/**
 *	@file
 *	Form callbacks for Jugador table
 */
	

	/**
	 * Page callback function for listing the rows from Jugador existing in the table
	 * as the name in the file, and inserting in it too.
	 */
	function jugadores_admin_list(  ) {
		// Creating the date/time element starts here
		
		
		
		// Provide a default date in the format YYYY-MM-DD HH:MM:SS.
			$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		
		$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);
		
		db_set_active( 'eSM' );
		$result_set_Jugador = db_query( "SELECT * FROM {Jugador}" );
		db_set_active( 'default' );
		
		
		//$form_id = jugador_admin_list;
		
		
		//Primero pondremos un listado de jugadors disponibles
		while( $Jugador = db_fetch_object( $result_set_Jugador ) ) {
		
			$fecha_registro = timestamp_to_string( $Jugador->Fecha_Inicio );
			

			$form[ 'listar' ][ $Jugador->id_Jugador ][ 'title' ] = array (
				'#type' => 'item',
				//'label' => t( 'Nombre' ),
				//'#disabled' => 'TRUE',
				//'size' => 17,
				//'maxlength' => 17,
				'#title' => t( 'Nombre' ),
				//'#required' => TRUE,
				'#weight' => -8,
				'#value' => $Jugador->Nombre."\n",
			);			
			
			$form[ 'listar' ][ $Jugador->id_Jugador ][ 'fecha_registro' ] = array (
				'#type' => 'date_text', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
				'#disabled' => 'TRUE',
				'#title' => 'Fecha de inicio',
				'#default_value' => $fecha_registro,
				'#date_format' => $format,
				'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
				'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
				'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
				'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
				'#size' => 17,
				'#maxlength' => 17,
			);
						
			$form[ 'listar' ][ $Jugador->id_Jugador ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/jugadores/'.$row->id_Jugador.'/edit' ),
			);

		}		
		
		//Un nuevo fieldset para insertar nuevas jugadors	
		$form[ 'insertar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Insertar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);
		
		
		$form[ 'insertar' ][ 'title' ] = array
		(
				'#type' => 'textfield',
				'#title' => t('Nombre'),
				'#required' => TRUE,
				'#weight' => -8,
				//'#default_value' => $node->title,
		);
		
		$form[ 'insertar' ][ 'fecha_inicio' ] = array
			(
				'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
				'#title' => 'Selecciona la fecha de inicio',
				'#default_value' => date( 'Y-m-d' ), 
				'#date_format' => $format,
				'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
				'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
				'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
				'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
			);
		
		$form[ 'insertar' ][ 'fecha_fin' ] = array
		(
				'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
				'#title' => 'Selecciona la fecha de finalización',
				//'#default_value' => $node->fecha_fin, 
				'#date_format' => $format,
				'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
				'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
				'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
				'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
		);
				
		
			
		
		$form[ 'insertar' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Insertar' ),
		);
		
		
		$form[ '#validate' ] = array( 'jugador_admin_list_validate' );
		
		return $form;
	}
	
	
	/**
	 *	Implementación del hook submit
	 *	para la tabla jugador
	 */
	function jugador_admin_list_submit( $form, $form_state ) {
		
		//$jugador_inicio = string_to_timestamp( $form_state[ 'values' ][ 'insertar' ][ 'fecha_inicio' ] );
		//$jugador_fin = string_to_timestamp( $form_state[ 'values' ][ 'insertar' ][ 'fecha_fin' ] );
		
		$jugador_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );
		$jugador_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
		
		
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Jugador } 
			( 
				Nombre, 
				Fecha_Inicio, 
				Fecha_Fin 
			) 
				VALUES ( '%s', %d, %d )",
				$form_state[ 'values' ][ 'title' ],
				$jugador_inicio, 
				$jugador_fin 
		);
		
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué insertada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/jugadores" );
		
	}
	

	/**
	 *	Función para editar una jugador
	 */
	function jugador_admin_list_edit( ) {
		
		
		
		
		$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		
		$id_Jugador = arg( 3 );
		
		
		
		db_set_active( 'eSM' );
		$result_set = db_query( "SELECT * FROM {Jugador} WHERE {Jugador.id_Jugador} = %d", $id_Jugador );
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de jugadors disponibles
		$row = db_fetch_object( $result_set );
		
		$fecha_inicio = timestamp_to_string( $row->Fecha_Inicio );
		$fecha_fin = timestamp_to_string( $row->Fecha_Fin );
		
		
		$form[ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t('Nombre'),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $row->Nombre,
		);
		
		$form[ 'fecha_inicio' ] = array
			(
			'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
			'#title' => 'Selecciona la fecha de inicio',
			'#default_value' => $fecha_inicio,
			'#date_format' => $format,
			'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
			'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
			'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
			'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
			);
		
		$form[ 'fecha_fin' ] = array
		(
			'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
			'#title' => 'Selecciona la fecha de finalización',
			'#default_value' => $fecha_fin,
			'#date_format' => $format,
			'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
			'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
			'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
			'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
		);
		
		
		$form[ 'id_Jugador' ] = array(
			'#type' => 'hidden',
			'#value' => $row->id_Jugador,
		);
			
		/*$form[ 'id_Form' ] = array(
			'#type' => 'hidden',
			'#value' => $form->form_id,
		);*/
				
		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Guardar' ),
			
		);
				
				
				
		$form[ '#validate' ] = array( 'jugador_admin_list_validate' );
		
		
		return $form;
		
		
	}




	/**
	 *	Sumbit para la forma de edición
	 */
	function jugador_admin_list_edit_submit( $form, $form_state ) {
		
		$jugador_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );

		$jugador_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
		
		
		db_set_active( 'eSM' );
			
		db_query
		(
			"UPDATE {Jugador}
			SET
			Nombre = '%s',
			Fecha_Inicio = %d,
			Fecha_Fin = %d
			WHERE
			id_Jugador = %d",
			$form_state[ 'values' ][ 'title' ],
			$jugador_inicio,
			$jugador_fin,
			$form_state[ 'values' ][ 'id_Jugador' ]
		);
				
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué guardada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/jugadores" );
		
	}
	
	
	/**
	 * Función para validar:
	 * La fecha de fin sea mayor que la fecha de inicio
	 * La fecha de inicio sea mayor o igual que la fecha actual
	 * La fechas de jugadors no abarquen periodos compartidos de tiempo
	 */
	function jugador_admin_list_validate( $form, $form_state ) {
		
		//Guardamos la fecha actual como timestamp
		$fecha_actual = string_to_timestamp ( date( "Y-m-d" ) );

		
		//Asignamos fechas de inicio y final de acuerdo a la forma
		/*if ( $form[ 'form_id' ][ '#id' ] == "edit-jugador-admin-list" ) {
			$fecha_inicio_forma = string_to_timestamp ( $form_state[ 'values' ][ 'insertar' ][ 'fecha_inicio' ] );
			$fecha_fin_forma = string_to_timestamp ( $form_state[ 'values' ][ 'insertar' ][ 'fecha_fin' ] );			
		}
		else if ($form[ 'form_id' ][ '#id' ] == "edit-jugador-admin-list-edit" ) {*/
			$fecha_inicio_forma = string_to_timestamp ( $form_state[ 'values' ][ 'fecha_inicio' ] );
			$fecha_fin_forma = string_to_timestamp ( $form_state[ 'values' ][ 'fecha_fin' ] );			
		//}
		
		
		//Validamos
		if( intval( $fecha_fin_forma ) < intval( $fecha_inicio_forma ) ) {
			form_set_error( 'fecha_fin', t( 'La fecha de fin debe ser mayor o igual que la fecha de inicio' ) );
		}
		if( (integer)$fecha_inicio_forma < (integer)$fecha_actual ) {
			form_set_error( 'fecha_inicio', t( 'La fecha de inicio debe ser mayor o igual que la fecha actual' ) );
		}
		
		
		db_set_active( 'eSM' );
		$result_set = db_query( "SELECT * FROM {Jugador} ORDER BY Fecha_Inicio ASC" );
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de jugadors disponibles
		while( $row = db_fetch_object( $result_set ) ) {
			
			$fecha_inicio = $row->Fecha_Inicio;
			$fecha_fin = $row->Fecha_Inicio;
			
			/*dpm( 'Fecha de inicio: '.$fecha_inicio.' para el id: '.$row->id_Jugador );
			dpm( 'Fecha de inicio en la forma: '.$fecha_inicio_forma );
			dpm( 'Fecha de fin: '.$fecha_fin.' para el id: '.$row->id_Jugador );
			dpm( 'Fecha de fin en la forma: '.$fecha_fin_forma );*/
			
			
			if( ( $fecha_inicio_forma <= $fecha_fin ) && ( $fecha_fin_forma <= $fecha_fin ) ) {
				form_set_error( array( 'fecha_inicio', 'fecha_fin'  ), t( 'Las fechas entre jugadors no pueden solaparse' ) );
			}
			
		}
	}
	