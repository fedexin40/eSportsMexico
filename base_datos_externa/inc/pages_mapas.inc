<?php
/**
 *	@file
 *	Form callbacks for Mapa table
 */
/**
 *	Función para añadir un nuevo mapa
 */
function mapas_admin_insertar( ) {
	$form[ 'insertar' ][ 'title' ] = array (
		'#type' => 'textfield',
		'#title' => t( 'Nombre' ),
		'#required' => TRUE,
		'#weight' => -8,
	);

	$form[ 'insertar' ][ 'submit' ] = array (
		'#type' => 'submit',
		'#value' => t( 'Insertar' ),
	);
			
	return $form;
}
/**
 * Función para validar:
 */
function mapas_admin_insertar_validate( $form, $form_state ) {
	$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
}
/**
 *	Implementación del hook submit
 *	para la tabla mapa
 */
function mapas_admin_insertar_submit( $form, $form_state ) {
	$id_Modalidad_Juego = arg(3);
	
	db_set_active( 'eSM' );
		db_query("INSERT INTO { Mapa }
						 (Nombre, id_Modalidad_Juego)
						 VALUES ( '%s', %d )",
						 $form_state[ 'values' ][ 'title' ], $id_Modalidad_Juego);
	db_set_active( 'default' );
	
	drupal_set_message( "El mapa fué insertado satisfactoriamente en esta modalidad" );
	drupal_goto('admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/editar');
}
/**
 *	Función para editar una mapa
 */
function mapa_admin_editar( ) {
	$id_Mapa = arg( 5 );
	$id_Modalidad = arg(3);
	
	//Hacemos la consulta para saber el nombre del mapa
	db_set_active( 'eSM' );		
		$result_set_Mapa = db_query( "SELECT * FROM {Mapa} WHERE {id_Mapa} = %d", $id_Mapa );
	db_set_active( 'default' );
	
	$Mapa = db_fetch_object( $result_set_Mapa );
		
	$form[ 'title' ] = array (
		'#type' => 'textfield',
		'#title' => t( 'Nombre' ),
		'#required' => TRUE,
		'#weight' => -8,
		'#default_value' => $Mapa->Nombre,
	);

	$form[ 'id_Mapa' ] = array (
		'#type' => 'hidden',
		'#value' => $Mapa->id_Mapa,
	);
	
	$form[ 'id_Modalidad' ] = array (
		'#type' => 'hidden',
		'#value' => $id_Modalidad,
	);

	$form[ 'submit' ] = array (
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
		db_query("UPDATE {Mapa} SET Nombre = '%s'
						 WHERE id_Mapa = %d",
						 $form_state[ 'values' ][ 'title' ], $form_state[ 'values' ][ 'id_Mapa' ]);
	db_set_active( 'default' );
	
	drupal_set_message( "Las modificaciones se guardaron correctamente" );
	drupal_goto( "admin/base_datos_externa/modalidades/". $form_state['values']['id_Modalidad'] ."/editar" );
}
/**
 * Función para despleagar el formulario de confirmación del borrado de un mapa
 */
function mapa_admin_eliminar() {
	$id_Mapa = arg( 5 );
	$id_Modalidad = arg(3);
	
	//Hacemos la consulta para saber el nombre del mapa
	db_set_active( 'eSM' );		
		$mapa = db_fetch_object(db_query( "SELECT * FROM {Mapa} WHERE {id_Mapa} = %d", $id_Mapa ));
	db_set_active( 'default' );
	
	return confirm_form (
		$form,
		t('¿Realmente quieres borrar el mapa '. $mapa->Nombre .'?'),
		isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/modalidades/". $id_Modalidad . "/editar",
		t(' '),
		t('Si'),
		t('Cancelar')
	);
}
/**
 * Función para despleagar el formulario de confirmación del borrado de un mapa
 */
function mapa_admin_eliminar_submit() {
	$id_Mapa = arg( 5 );
	$id_Modalidad = arg(3);
	
	//Hacemos la consulta para saber el nombre del mapa
	db_set_active( 'eSM' );		
		db_query( "DELETE FROM {Mapa} WHERE {id_Mapa} = %d", $id_Mapa );
	db_set_active( 'default' );
	
	drupal_set_message( "El mapa se ha borrado correctamente" );
	drupal_goto( "admin/base_datos_externa/modalidades/". $id_Modalidad ."/editar" );
}