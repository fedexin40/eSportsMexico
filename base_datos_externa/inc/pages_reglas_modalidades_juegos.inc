<?php
/**
 *	@file
 *	Form callbacks for modos_juego table
 */
/**
 *	Función para añadir las reglas de una modalidad
 */
function modalidad_juego_reglas( ) {
	$id_Modalidad_Juego = arg(3);
	$op = arg(5);
	
	db_set_active('eSM');
		$reglas = db_fetch_object(db_query('SELECT * FROM {Reglas} WHERE id_Modalidad_Juego = %d', $id_Modalidad_Juego));
	db_set_active('default');
	
	$form[ 'insertar' ][ 'file' ] = array (
		'#type' => 'textfield',
		'#title' => t( 'Archivo de Reglas' ),
		'#description' => t('Copia la ruta al archivo pdf de las reglas'),
		'#required' => TRUE,
		'#default_value' => $reglas->Archivo,
	);	
	$form[ 'insertar' ][ 'contenido' ] = array (
		'#type' => 'textarea',
		'#title' => t( 'Reglas' ),
		'#required' => TRUE,
		'#default_value' => $reglas->Contenido,
	);
	//Agrega un filtro de formato al elemento
	$form['insertar']['format'] = filter_form($node->format);

	$form[ 'insertar' ][ 'submit' ] = array(
		'#type' => 'submit',
		'#value' => $op,
	);
	
	return $form;
}
/**
 *	Implementación del hook validate
 *	para la tabla modos_juego
 */
function modalidad_juego_reglas_validate( $form, $form_state ) {
	if (!valid_url($form_state['values']['file'], TRUE))
		form_set_error('file', t('Inserta una ruta de archivo válida'));
}
/**
 *	Implementación del hook submit
 *	para la tabla modos_juego
 */
function modalidad_juego_reglas_submit( $form, $form_state ) {
	$id_Modalidad_Juego = arg(3);
	$op = arg(5);
	
	if($op == 'insertar') {
		db_set_active( 'eSM' );
			db_query ("INSERT INTO { Reglas }
						(id_Modalidad_Juego, Contenido, Archivo)
						VALUES (%d, '%s', '%s')",
						$form_state['values']['id_Modalidad_Juego'], $form_state['values']['file'], $id_Modalidad_Juego);
		db_set_active( 'default' );
	}
	
	if($op == 'editar') {
		db_set_active( 'eSM' );
			db_query ("UPDATE { Reglas }
					   SET Contenido = '%s', Archivo = '%s'
					   WHERE id_Modalidad_Juego = %d",
					   $form_state['values']['contenido'], $form_state['values']['file'], $id_Modalidad_Juego);
		db_set_active( 'default' );
	}
	
	drupal_set_message(t("Las reglas se guardaron satisfactoriamente en esta modalidad."));
	drupal_goto('admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/editar');
}