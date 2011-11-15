<?php
/**
 *	@file
 *	Form callbacks for modos_juego table
 */
/**
 *	Función para añadir un nuevo modo de juego
 */
function modos_juego_admin_insertar( ) {
	$form[ 'insertar' ][ 'title' ] = array (
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
/**
 * Función para validar:
 */
function modos_juego_admin_insertar_validate( $form, $form_state ) {
	$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
}
/**
 *	Implementación del hook submit
 *	para la tabla modos_juego
 */
function modos_juego_admin_insertar_submit( $form, $form_state ) {
	$id_Modalidad_Juego = arg(3);
	
	db_query ("INSERT INTO {eSM_Modo_Juego }
			  (Nombre, id_Modalidad_Juego)VALUES ( '%s', %d )",
			  $form_state[ 'values' ][ 'title' ], $id_Modalidad_Juego);
	
	drupal_set_message( "El modo de juego fué insertado satisfactoriamente en esta modalidad" );
	drupal_goto('admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/editar');
}
/**
 *	Función para editar una modos_juego
 */
function modos_juego_admin_editar( ) {
	$id_Modalidad = arg(3);
	$id_Modo_Juego = arg( 5 );

	//Hacemos la consulta para saber el nombre del modo de juego
	$result_set_modos_juego = db_query( "SELECT * FROM {eSM_Modo_Juego} WHERE {id_Modo_Juego} = %d", $id_Modo_Juego );
		
	$modos_juego = db_fetch_object( $result_set_modos_juego );
	
	$form[ 'title' ] = array (
		'#type' => 'textfield',
		'#title' => t( 'Nombre' ),
		'#required' => TRUE,
		'#weight' => -8,
		'#default_value' => $modos_juego->Nombre,
	);
				
	$form[ 'id_Modo_Juego' ] = array(
		'#type' => 'hidden',
		'#value' => $modos_juego->id_Modo_Juego,
	);

	$form[ 'id_Modalidad' ] = array (
		'#type' => 'hidden',
		'#value' => $id_Modalidad,
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
	db_query("UPDATE {eSM_Modo_Juego}
			 SET Nombre = '%s'
			 WHERE id_Modo_juego = %d",
			 $form_state[ 'values' ][ 'title' ], $form_state[ 'values' ][ 'id_Modo_Juego' ]);
	
	drupal_set_message( "El modo de juego fué insertado satisfactoriamente en esta modalidad" );
	drupal_goto( "admin/base_datos_externa/modalidades/". $form_state['values']['id_Modalidad'] ."/editar" );
}
/**
 * Función para despleagar el formulario de confirmación del borrado de un mapa
 */
function modos_juego_admin_eliminar() {
	$id_Modo_Juego = arg( 5 );
	$id_Modalidad = arg(3);
	
	//Hacemos la consulta para saber el nombre del mapa
	$modo = db_fetch_object(db_query( "SELECT * FROM {eSM_Modo_Juego} WHERE {id_Modo_Juego} = %d", $id_Modo_Juego ));
		
	return confirm_form (
		$form,
		t('¿Realmente quieres borrar el modo de juego '. $modo->Nombre .'?'),
		isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/modalidades/". $id_Modalidad . "/editar",
		t(' '),
		t('Si'),
		t('Cancelar')
	);
}
/**
 * Función para despleagar el formulario de confirmación del borrado de un mapa
 */
function modos_juego_admin_eliminar_submit() {
	$id_Modo_Juego = arg( 5 );
	$id_Modalidad = arg(3);
	
	//Hacemos la consulta para saber el nombre del mapa
	db_query( "DELETE FROM {eSM_Modo_Juego} WHERE {id_Modo_Juego} = %d", $id_Modo_Juego );
		
	drupal_set_message( "El mapa se ha borrado correctamente" );
	drupal_goto( "admin/base_datos_externa/modalidades/". $id_Modalidad ."/editar" );
}