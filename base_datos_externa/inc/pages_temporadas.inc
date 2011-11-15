<?php
/**
 *	@file
 *	Form callbacks for Temporada table
 */
/**
 * Page callback function for listing the rows from Temporada existing in the table
 * as the name in the file, and inserting in it too.
 */
function temporada_admin(  ) {
	$result_set_Temporada = db_query( "SELECT * FROM {eSM_Temporada}" );	
	
	$Existe_Temporada_Activa = 0;

	while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
		if( $Temporada->Estado == TEMPORADA_ACTIVA ) {
			$Existe_Temporada_Activa = 1;
			break;
		}
	}
	
	if( $Existe_Temporada_Activa == 1 ) {
		drupal_goto('admin/base_datos_externa/temporada/listar');
	}
	else {
		//No hay temporadas activas en la base de datos
		$result_set_Temporada = db_query( "SELECT * FROM {eSM_Temporada} WHERE Estado <> %d", TEMPORADA_CERRADA );
				
		$output = '<p>No existe ninguna temporada activa actualemente.</p><p>Activa una temporada o crea una temporada nueva utilizando los enlaces de esta página</p>';
		$output .= '<fieldset class="collapsible"><legend>Temporadas Disponibles</legend>';
		
		$count = 0;
		while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
			$count++;
			$row = array();
			$row[] = $Temporada->Nombre;
			$row[] = timestamp_to_string( $Temporada->Fecha_Inicio );
			$row[] = timestamp_to_string( $Temporada->Fecha_Fin );
			$row[] = l( t( 'Activar' ), 'admin/base_datos_externa/temporada/activar/'.$Temporada->id_Temporada);
			$rows[] = $row;
		}

		if ($count > 0) {
			$header = array ('Nombre', 'Fecha de Inicio', 'Fecha Final', ' ');
			$output .= theme('table', $header, $rows);
		}
		else {
			$output .= '<p>No hay temporadas disponibles.</p>';
		}
		
		$output .= l( t( 'Añadir Temporada' ), 'admin/base_datos_externa/temporada/insertar');
		$output .= '</fieldset>';
	}	
	return $output;
}
/**
 * Despliega la información de la temporada activa
 */
function temporada_activa_listar () {
	$result_set_Temporada = db_query( "SELECT * FROM {eSM_Temporada} WHERE {Temporada.Estado} = %d", TEMPORADA_ACTIVA );
			
		$Temporada = db_fetch_object( $result_set_Temporada );
					
		$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
		$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );
		
		$output = '<fieldset class="collapsible"><legend>Detalles</legend>';
		$content[] = '<strong>Nombre:</strong> ' . $Temporada->Nombre;
		$content[] = '<strong>Fecha de inicio:</strong> ' . $fecha_inicio;
		$content[] = '<strong>Fecha de fin:</strong> ' . $fecha_fin;
		
		$output .= theme('item_list', $content);
					
		$output .= l( t( 'Editar Detalles' ), 'admin/base_datos_externa/temporada/editar/'.$Temporada->id_Temporada);
		$output .=  '<span> | </span>';
		$output .= l( t( 'Cerrar Temporada' ), 'admin/base_datos_externa/temporada/desactivar/'.$Temporada->id_Temporada);
		$output .=  '<span> | </span>';
		$output .= l( t( 'Añadir Temporada' ), 'admin/base_datos_externa/temporada/insertar');
		$output .= '</fieldset>';
		
		$modalidades = db_query( "SELECT * FROM {eSM_Modalidad_Juego_Temporada} AS mjt
								INNER JOIN {eSM_Modalidad_Juego} AS mj
								ON mjt.id_Modalidad_Juego = mj.id_Modalidad_Juego
								WHERE id_Temporada = %d",
								$Temporada->id_Temporada);
		$count = 0;
		//Primero pondremos un listado de modalidades disponibles
		while( $modalidad = db_fetch_object( $modalidades) ) {
			$count++;
			$row = array();
			$row[] = $modalidad->Nombre;
			$row[] = l( t( 'Desactivar Modalidad' ), 'admin/base_datos_externa/temporada/'. $Temporada->id_Temporada .'/modalidades/'. $modalidad->id_Modalidad_Juego .'/desactivar');
			$rows[] = $row;
		}
		
		$output .= '<fieldset class = "collapsible"><legend>Modalidades Activas</legend>';
		if ($count > 0) {
			$header = array ('Nombre', ' ');
			$output .= theme('table', $header, $rows);
		}
		else {
			$output .= '<p>No hay Modalidades Activas en la temporada.</p>';
		}
		$output .= l( t( 'Añadir Modalidad' ), 'admin/base_datos_externa/temporada/'. $Temporada->id_Temporada .'/modalidades'). '</fieldset>';
	
	return $output;
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
	
	$id_Temporada = arg( 4 );
	
	$result_set_Temporada = db_query( "SELECT * FROM {eSM_Temporada} WHERE {Temporada.id_Temporada} = %d", $id_Temporada );

	$Temporada = db_fetch_object( $result_set_Temporada );
	
	$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
	$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );
	
	$form[ 'title' ] = array (
		'#type' => 'textfield',
		'#title' => t('Nombre'),
		'#required' => TRUE,
		'#weight' => -8,
		'#default_value' => $Temporada->Nombre,
	);
	
	$form[ 'fecha_inicio' ] = array (
		'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
		'#title' => 'Selecciona la fecha de inicio',
		'#default_value' => $fecha_inicio,
		'#date_format' => $format,
		'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
		'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
		'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
		'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
		);
	
	$form[ 'fecha_fin' ] = array (
		'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
		'#title' => 'Selecciona la fecha de finalización',
		'#default_value' => $fecha_fin,
		'#date_format' => $format,
		'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
		'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
		'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
		'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
	);
	
	$form[ 'id_Temporada' ] = array (
		'#type' => 'hidden',
		'#value' => $Temporada->id_Temporada,
	);
		
	$form[ 'submit' ] = array (
		'#type' => 'submit',
		'#value' => t( 'Guardar' ),
	);
				
	$form[ '#validate' ] = array( 'temporada_admin_insertar_validate' );
	
	return $form;
}
/**
 *	Sumbit para la forma de edición
 */
function temporada_admin_editar_submit( $form, $form_state ) {
	$temporada_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );
	$temporada_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
	
	db_query("UPDATE {eSM_Temporada} SET
			 Nombre = '%s', Fecha_Inicio = %d, Fecha_Fin = %d
			 WHERE id_Temporada = %d",
			 $form_state[ 'values' ][ 'title' ], $temporada_inicio, $temporada_fin, $form_state[ 'values' ][ 'id_Temporada' ]);
	
	drupal_set_message('Se han guardado los cambios');
	drupal_goto( "admin/base_datos_externa/temporada" );
}
/**
 *	Lista las temporadas disponibles en la base de datos
 */
function temporadas_admin_listar( ) {
	$output = '';
	$temporada = db_fetch_object(db_query("SELECT * FROM {eSM_Temporada} WHERE Estado = %d LIMIT 1", TEMPORADA_ACTIVA));
		
	if(is_object($temporada)) {	//Si hay una temporada activa desplegamos su información
		$header = array(t('Nombre'), t('Fecha Inicio'), t('Fecha Fin'));
		$rows = array(array($temporada->Nombre, timestamp_to_string( $temporada->Fecha_Inicio ), timestamp_to_string( $temporada->Fecha_Inicio )));
		$output .= '<fieldset class = "collapsible"><legend>Temporada Activa</legend>' . theme('table', $header, $rows) . '</fieldset>';
	}
	
	//Primero pondremos un listado de las temporadas inactivas
	$result_set_Temporada = db_query("SELECT * FROM {eSM_Temporada} WHERE Estado = %d ORDER BY Fecha_Inicio ASC", TEMPORADA_INACTIVA);
	
	$rows = array();
	while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
		$row = array();
		$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
		$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );

		$row[] = $Temporada->Nombre;			
		$row[] = $fecha_inicio;
		$row[] = $fecha_fin;
		$row[] = l( t( 'Editar' ), 'admin/base_datos_externa/temporada/editar/'.$Temporada->id_Temporada);
		$rows[] = $row;
	}
	
	if(count($rows) > 0) {	//Si hay temporadas inactivas
		$header = array(t('Nombre'), t('Fecha Inicio'), t('Fecha Fin'), ' ');
		$output .= '<fieldset class = "collapsible"><legend>Temporada Activa</legend>' . theme('table', $header, $rows) . '</fieldset>';
	}
	
	//Finalizamos con un listado de todas las temporadas pasadas
	$result_set_Temporada = db_query("SELECT * FROM {eSM_Temporada} WHERE Estado = %d ORDER BY Fecha_Inicio DESC", TEMPORADA_CERRADA);
	
	$rows = array();
	while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
		$row = array();
		$fecha_inicio = timestamp_to_string( $Temporada->Fecha_Inicio );
		$fecha_fin = timestamp_to_string( $Temporada->Fecha_Fin );

		$row[] = $Temporada->Nombre;			
		$row[] = $fecha_inicio;
		$row[] = $fecha_fin;
		$row[] = l( t( 'Estadisticas' ), 'admin/base_datos_externa/temporada/stats/'.$Temporada->id_Temporada);
		$rows[] = $row;
	}
	
	if(count($rows) > 0) {	//Si hay temporadas inactivas
		$header = array(t('Nombre'), t('Fecha Inicio'), t('Fecha Fin'), ' ');
		$output .= '<fieldset class = "collapsible"><legend>Temporada Activa</legend>' . theme('table', $header, $rows) . '</fieldset>';
	}
	
	return $output;
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
	
	$form[ 'insertar' ][ 'title' ] = array 	(
			'#type' => 'textfield',
			'#title' => t('Nombre'),
			'#required' => TRUE,
			'#weight' => -8,
	);
	
	$form[ 'insertar' ][ 'fecha_inicio' ] = array (
			'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
			'#title' => 'Selecciona la fecha de inicio',
			'#default_value' => date( 'Y-m-d' ), 
			'#date_format' => $format,
			'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
			'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
			'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
			'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
		);
	
	$form[ 'insertar' ][ 'fecha_fin' ] = array (
			'#type' => 'date_select', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
			'#title' => 'Selecciona la fecha de finalización',
			'#date_format' => $format,
			'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
			'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
			'#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
			'#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
	);
				
		$form[ 'insertar' ][ 'submit' ] = array (
		'#type' => 'submit',
		'#value' => t( 'Insertar' ),
		);
	
	return $form;
}
/**
 *	Implementación del hook submit
 *	para la tabla temporada
 */
function temporadas_admin_insertar_submit( $form, $form_state ) {
	$temporada_inicio = string_to_timestamp( $form_state[ 'values' ][ 'fecha_inicio' ] );
	$temporada_fin = string_to_timestamp( $form_state[ 'values' ][ 'fecha_fin' ] );
	
	db_query("INSERT INTO {eSM_Temporada }
			 (Nombre,  Fecha_Inicio,  Fecha_Fin)
			 VALUES ( '%s', %d, %d )",
			 $form_state[ 'values' ][ 'title' ], $temporada_inicio, $temporada_fin);
	
	drupal_set_message( "La temporada fué insertada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/temporada" );
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
	else {
		$result_set_Temporada = db_query( "SELECT * FROM {eSM_Temporada} ORDER BY Fecha_Inicio ASC" );
		
		//Primero pondremos un listado de temporadas disponibles
		while( $Temporada = db_fetch_object( $result_set_Temporada ) ) {
				if( $Temporada->Estado == 2 )  								//Si las temporadas son futuras, no las valida
					break;
				$fecha_inicio = $Temporada->Fecha_Inicio;
				$fecha_fin = $Temporada->Fecha_Inicio;
				
				if( $fecha_inicio_forma <= $fecha_fin ) {
					form_set_error( array( 'fecha_inicio', 'fecha_fin'  ), t( 'Las fechas entre temporadas no pueden solaparse' ) );
				}
			if( $Temporada->Estado == TEMPORADA_CERRADA ) {																							//Si la temporada es pasada
				form_set_error( array( 'fecha_inicio', 'fecha_fin'  ), t( 'Las temporadas pasadas no se puden editar' ) );
			}
		}			
	}
}
/**
 *	Confirmación de la operación de cierre de la temporada
 */
function temporada_admin_desactivar( ) {
	return confirm_form (
		$form,
		t('¿Realmente quieres cerrar la temporada actual?'),
		isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/temporada",
		t('Los cambios no se pueden deshacer'),
		t('Si'),
		t('Cancelar')
	);
}
/**
 *	Introduce en la tabla de temporada la información
 */
function temporada_admin_desactivar_submit( ) {
	$id_Temporada = arg( 4 );
	
	db_query ("UPDATE {eSM_Temporada}
			  SET { Estado } =  %d
			  WHERE { Temporada.id_Temporada} = %d",
			  TEMPORADA_CERRADA, $id_Temporada);			

	//TODO: Establecer una serie de acciones a realizar cuando se cierra la temporada
	drupal_set_message( "Has cerrado la temporada." );
	drupal_goto( "admin/base_datos_externa/temporada" );		
}
/**
 *	Formulario para confirmar la activación de una temporada
 */
function temporada_admin_activar( ) {
	return confirm_form (
		$form,
		t('¿Realmente quieres activar la temporada seleccionada?'),
		isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/temporada",
		t(' '),
		t('Si'),
		t('Cancelar')
	);
}
/**
 *	Activa una temporada
 */
function temporada_admin_activar_submit( ) {
	$id_Temporada = arg( 4 );
	
	db_query ("UPDATE {eSM_Temporada}
			  SET { Estado } =  %d
			  WHERE { Temporada.id_Temporada} = %d",
			  TEMPORADA_ACTIVA, $id_Temporada);			
	
	drupal_set_message( "La temporada ha sido activada corrrectamente." );
	drupal_goto( "admin/base_datos_externa/temporada" );
}
/**
 *	Página de confirmación de la operación de activación
 */
function temporada_admin_activar_validate( ) {
	$id_Temporada = arg( 4 );
	
	$result = db_result(db_query ("SELECT count(*) FROM {eSM_Temporada}
								  WHERE { Estado } = %d",
								  TEMPORADA_ACTIVA));			
	
	if ($result <> 0)
		form_set_error(NULL, t('Ya existe una temporada activa.'));
}