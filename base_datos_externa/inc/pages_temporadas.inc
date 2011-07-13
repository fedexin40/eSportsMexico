<?php
// $Id$

/**
 *	@file
 *	Form callbacks for Temporada table
 */
	

	/**
	 * Page callback function for listing the rows from Temporada existing in the table
	 * as the name in the file, and inserting in it too.
	 */
	function temporada_admin(  ) {
		// Creating the date/time element starts here
		
		// Provide a default date in the format YYYY-MM-DD HH:MM:SS.
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
			
			$form[ 'Estado_Temporadas' ][ 'Listar_Temporada_Activa' ][ 'desactivar' ] = array (
				'#value' => l( t( 'Desactivar' ), 'admin/base_datos_externa/temporadas/'.$Temporada->id_Temporada.'/desactivar' ),
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
	 *	Implementación del hook submit
	 *	para la tabla temporada
	 */
	function temporadas_admin_insertar_submit( $form, $form_state ) {
		
		
		
		//dpm( "temporadasinsertar" );
		//$temporada_inicio = string_to_timestamp( $form_state[ 'values' ][ 'insertar' ][ 'fecha_inicio' ] );
		//$temporada_fin = string_to_timestamp( $form_state[ 'values' ][ 'insertar' ][ 'fecha_fin' ] );
		
		$temporada_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );
		$temporada_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
		
		

		
		
		db_set_active( 'eSM' );
		
		db_query
		( 
			"INSERT INTO { Temporada } 
			( 
				Nombre, 
				Fecha_Inicio, 
				Fecha_Fin
			) 
				VALUES ( '%s', %d, %d )",

				$form_state[ 'values' ][ 'title' ],
				$temporada_inicio, 
				$temporada_fin
		);

		
		db_set_active( 'default' );
		
		drupal_set_message( "La temporada fué insertada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/temporadas" );
		
	}
	

	/**
	 *	Función para editar una temporada
	 */
	function temporada_admin_editar( ) {
		
		
		
		
		$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		
		$id_Temporada = arg( 3 );
		
		
		
		db_set_active( 'eSM' );
		$result_set_Temporada = db_query( "SELECT * FROM {Temporada} WHERE {Temporada.id_Temporada} = %d", $id_Temporada );
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de temporadas disponibles
		$Temporada = db_fetch_object( $result_set_Temporada );
		
		$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
		$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );
		
		
		$form[ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t('Nombre'),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $Temporada->Nombre,
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
		
		
		$form[ 'id_Temporada' ] = array(
			'#type' => 'hidden',
			'#value' => $Temporada->id_Temporada,
		);
			
		/*$form[ 'id_Form' ] = array(
			'#type' => 'hidden',
			'#value' => $form->form_id,
		);*/
				
		$form[ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Guardar' ),
			
		);
				
				
				
		$form[ '#validate' ] = array( 'temporada_admin_validate' );
		
		
		return $form;
		
		
	}
	

	/**
	 *	Sumbit para la forma de edición
	 */
	function temporada_admin_editar_submit( $form, $form_state ) {
		
		$temporada_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );

		$temporada_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
		
		//dpm( "Hola" );
		
		db_set_active( 'eSM' );
			
		db_query
		(
			"UPDATE {Temporada}
			SET
			Nombre = '%s',
			Fecha_Inicio = %d,
			Fecha_Fin = %d
			WHERE
			id_Temporada = %d",
			$form_state[ 'values' ][ 'title' ],
			$temporada_inicio,
			$temporada_fin,
			$form_state[ 'values' ][ 'id_Temporada' ]
		);
				
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué guardada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/temporadas" );
		
	}
	
	
	/**
	 * Función para validar:
	 * La fecha de fin sea mayor que la fecha de inicio
	 * La fecha de inicio sea mayor o igual que la fecha actual
	 * La fechas de temporadas no abarquen periodos compartidos de tiempo
	 */
	function temporadas_admin_insertar_validate( $form, $form_state ) {
		
		//Guardamos la fecha actual como timestamp
		$fecha_actual = string_to_timestamp ( date( "Y-m-d" ) );

		
		//Asignamos fechas de inicio y final de acuerdo a la forma
		/*if ( $form[ 'form_id' ][ '#id' ] == "edit-temporada-admin-list" ) {
			$fecha_inicio_forma = string_to_timestamp ( $form_state[ 'values' ][ 'insertar' ][ 'fecha_inicio' ] );
			$fecha_fin_forma = string_to_timestamp ( $form_state[ 'values' ][ 'insertar' ][ 'fecha_fin' ] );			
		}
		else if ($form[ 'form_id' ][ '#id' ] == "edit-temporada-admin-list-edit" ) {*/
			$fecha_inicio_forma = string_to_timestamp ( $form_state[ 'values' ][ 'fecha_inicio' ] );
			$fecha_fin_forma = string_to_timestamp ( $form_state[ 'values' ][ 'fecha_fin' ] );			
		//}
		
		
		//Validamos
		if( intval( $fecha_fin_forma ) < intval( $fecha_inicio_forma ) ) {
			form_set_error( 'fecha_fin', t( 'La fecha de fin debe ser mayor o igual que la fecha de inicio' ) );
		}
		else if( (integer)$fecha_inicio_forma < (integer)$fecha_actual ) {
			form_set_error( 'fecha_inicio', t( 'La fecha de inicio debe ser mayor o igual que la fecha actual' ) );
		}
		
		else
		{
		
			db_set_active( 'eSM' );
			$result_set_Temporada = db_query( "SELECT * FROM {Temporada} ORDER BY Fecha_Inicio ASC" );
			db_set_active( 'default' );
			
			//Primero pondremos un listado de temporadas disponibles
			while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
				

					if( $Temporada->Estado == 2 )  								//Si las temporadas son futuras, no las valida
						break;
					$fecha_inicio = $Temporada->Fecha_Inicio;
					$fecha_fin = $Temporada->Fecha_Inicio;
					
					/*dpm( 'Fecha de inicio: '.$fecha_inicio.' para el id: '.$Temporada->id_Temporada );
					dpm( 'Fecha de inicio en la forma: '.$fecha_inicio_forma );
					dpm( 'Fecha de fin: '.$fecha_fin.' para el id: '.$Temporada->id_Temporada );
					dpm( 'Fecha de fin en la forma: '.$fecha_fin_forma );*/
					
					
					if( $fecha_inicio_forma <= $fecha_fin ) {
						form_set_error( array( 'fecha_inicio', 'fecha_fin'  ), t( 'Las fechas entre temporadas no pueden solaparse' ) );
						drupal_goto( "admin/base_datos_externa/temporadas" );
					}
				if( $Temporada->Estado == 0 ) {																							//Si la temporada es pasada
					form_set_error( array( 'fecha_inicio', 'fecha_fin'  ), t( 'Las fechas pasadas no se puden editar' ) );
						drupal_goto( "admin/base_datos_externa/temporadas" );
				}
				
			}			
		}
	}
	
	
	
	
	/**
	 *	Refresca el estado de las tablas
	 */
	function refresh_status( ) {
		$fecha_actual = string_to_timestamp ( date( "Y-m-d" ) );
		if( $fecha_actual >= $temporada_inicio && $fecha_actual <= $temporada_fin )
		{
			$estado = 1;
		}
		else {
			$estado = 2;
		}
	}
	
	
	
	/**
	 *	Introduce en la tabla de temporada la información
	 */
	function temporada_admin_submit( $form, $form_state ) {
	
		
		
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
		
		
		drupal_set_message( "La plataforma fué activada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/temporadas" );
		
	}
	
	
	
	
	/**
	 *	Confirma la operación de activación de temporada
	 */
	function temporada_activa_admin_confirmacion_operacion( ) {
				
		//dpm( "Hola" );
		$options = array( "Si", "No" );
			

			
		$form[ 'Confirmar_Activa' ][ 'title' ]  = array (
			'#type' => 'title',
			//'label' => t( 'Nombre' ),
			//'#disabled' => 'TRUE',
			//'size' => 17,
			//'maxlength' => 17,
			'#title' => t( '¿Estás seguro?' ),
			//'#required' => TRUE,
			'#weight' => -8,
		);

		$form[ 'Confirmar_Activa' ][ 'Si' ] = array
		(
			'#value' => l( t( 'Si' ), 'admin/base_datos_externa/temporadas/confirmar_activa' ),
		);

		$form[ 'Confirmar_Activa' ][ 'No' ] = array
		(
			'#value' => l( t( 'No' ), 'admin/base_datos_externa/temporadas/' ),
		);
		
		
	}
	
	
	/**
	 *	Página de confirmación de la operación de activación
	 */
	function temporada_admin_confirmar_activa( ) {
		drupal_set_message( "La temporada fué insertada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/temporadas" );
	}
	
	
	
	
	
	/**
	 *	Lista las temporadas disponibles en la base de datos
	 */
	function temporadas_admin_listar( ) {
		
		// Creating the date/time element starts here
		
		// Provide a default date in the format YYYY-MM-DD HH:MM:SS.
		$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		
		/*$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#disabled' => TRUE,
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'FALSE',
		);*/
		
		
		db_set_active( 'eSM' );
			$result_set_Temporada = db_query( "SELECT * FROM {Temporada}" );
			$result_set_array_Temporada = db_fetch_array( db_query( "SELECT * FROM {Temporada}" ) );		
		db_set_active( 'default' );
		
		
		//Primero pondremos un listado de temporadas disponibles
		while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
		
			$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
			$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );
			

			$form[ 'listar' ][ $Temporada->id_Temporada ][ 'title' ] = array (
				'#type' => 'item',
				//'label' => t( 'Nombre' ),
				'#disabled' => 'TRUE',
				//'size' => 17,
				//'maxlength' => 17,
				'#title' => t( 'Nombre' ),
				//'#required' => TRUE,
				'#weight' => -8,
				'#value' => $Temporada->Nombre."\n",
			);			
			
			$form[ 'listar' ][ $Temporada->id_Temporada ][ 'fecha_inicio' ] = array (
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
			
			$form[ 'listar' ][ $Temporada->id_Temporada ][ 'fecha_fin' ] = array (
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
			
			
			$form[ 'listar' ][ $Temporada->id_Temporada ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/temporadas/'.$Temporada->id_Temporada.'/editar' ),
			);
			
		}
		
		return $form;
	}
	
	
	/**
	 *	Inserta una temporada en la base de datos
	 */
	function temporadas_admin_insertar ( ) {
		// Creating the date/time element starts here
		
		// Provide a default date in the format YYYY-MM-DD HH:MM:SS.
		$date = '2011-12-31';

		// Provide a format using regular PHP format parts (see documentation on php.net).
		// If you're using a date_select, the format will control the order of the date parts in the selector,
		// rearrange them any way you like. Parts left out of the format will not be displayed to the user.
		$format = 'Y-m-d';
		
		
		
		
		//Un nuevo fieldset para insertar nuevas temporadas	
		/*$form[ 'insertar' ] = array(
			'#type' => 'fieldset',
			'#disabled' => TRUE,
			'#title' => t( 'Insertar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		
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
		
		return $form;
	}
	
	
	/**
	 *	Introduce en la tabla de temporada la información
	 */
	function temporada_admin_desactivar( ) {
	
		$id_Temporada = arg( 3 );

		db_set_active( 'eSM' );
		db_query
		(
			"UPDATE
			{ Temporada }
			SET
			{ Estado } =  %d
			WHERE
			{ Temporada.id_Temporada} = %d",
			0,
			$id_Temporada
		);
				
		db_set_active( 'default' );
		
		drupal_set_message( "La plataforma fué desactivada satisfactoriamente" );
		drupal_goto( "admin/base_datos_externa/temporadas" );		
		
	}
	
	
	/**
	 *	Confirmación de la operación de inserción
	 *	con la sobreescritura de confirm
	 */
	function temporadas_admin_editar_confirm(&$form_state, $doctor) {
		$form['Temporada_Activa'] = array(
			'#type' => 'value',
			'#value' => $doctor,
		);
	 
		return confirm_form
		(
			$form,
			t('¿Estás seguro?'),
			isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/temporadas",
			t('Esta acción es muy importante'),
			t('Seguir'),
			t('Cancelar')
		);
		
	}
