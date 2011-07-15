<?php
	// $Id$


/**
 *	@file
 *	Form callbacks for Modalidad_Juego table
 */
	

/**
 * Page callback function for listing the rows from Modalidad_Juego existing in the table
 * as the name in the file, and inserting in it too.
 */
function modalidad_juego_admin(  ) {
	$form[ 'informacion' ] = array (
		'#type' => 'item',
		'#disabled' => FALSE,
		'#value' => 'Panel de administración de las modalidades de juego',
	);
	
	if( variable_get( 'vocabulario_modalidades', NULL ) == NULL ) {
		if ($vocabularios = taxonomy_get_vocabularies()) {
			foreach($vocabularios as $indice => $vocabulario) {
				$options[$vocabulario->vid] = $vocabulario->name;
			}
			//Agrega una opción extra para crear el vocabulario definido por dafault en el módulo
			$options['0'] = 'Crear Vocabulario por defecto';
			
			$form[ 'vocabulary' ] = array (
				'#title' => t('Select the option for the Modalidades vocabulary.'),
				'#type' => 'select',
				'#default_value' => '0',
				'#options' => $options,
			);
			$form[ 'submit' ] = array (
				'#title' => t('Save'),
				'#type' => 'submit',
			);
		}
		else {
			$form[ 'submit' ] = array (
				'#title' => t('Create default vocabulary.'),
				'#type' => 'submit',
			);
		}
		
	}
	else {
			$form[ 'message' ] = array (
				'#title' => t('Modalidades module is configured properly.'),
			);
	}
	
	return $form;

}
	
	
/**
 *	Implementación del hook submit para la forma
 *	de administración de las modalidades.
 *	Inserta el vid del vocabulario seleccionado en la
 *	tabla de variables de drupa y crea un vocabulario
 *	en caso de ser la opción del usiario.
 */
function modalidad_juego_admin_submit( $form, $form_state ) {
	if ($form_state['values']['vocabulary'] <> 0) {			//Si se escogió un vocabulario ya existente carga los datos del mismo
		$vocabulary = taxonomy_vocabulary_load($form_state['values']['vocabulary']);
	}
	else {
		$vocabulary = array (			//Define los datos para crear un vocabulario nuevo
			'name' => t( 'Modalidades de juegos' ),
			'description' => t('Modalidades de juego'),
			'help' => t('Modalidades de juego'),
			'nodes' => array ( 'story' => 1, 'page' => 1 ),
			'hierarchy' => 1,
			'relations' => 0,
			'tags' => 0,
			'multiple' => 0,
			'required' => 0,
			'module' => 'base_datos_externa',
			'weight' => -10,
		);
		
		taxonomy_save_vocabulary($vocabulary);		//Crear el vocabulario nuevo
		
		drupal_set_message( "El vocabulario por defecto se ha creado correctamente." );
	}
		
	variable_set( 'vocabulario_modalidades', $vocabulary->vid );		//Le dice a drupal cual va a ser el vid del vocabulario para las modalidades
}
	

/**
 *	Función para mostrar la forma que permite editar una modalidad
 */
function modalidad_juego_admin_editar( ) {
	
	$id_Modalidad_Juego = arg( 3 );
	
	db_set_active( 'eSM' );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM { Modalidad_Juego } WHERE { id_Modalidad_Juego } = %d", $id_Modalidad_Juego );
		$result_set_Plataforma = db_query( "SELECT * FROM { Plataforma }" );
	db_set_active( 'default' );

	$Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego );

	while( $Plataforma = db_fetch_object( $result_set_Plataforma ) )
		$Plataforma_Nombres[ $Plataforma->id_Plataforma ] = $Plataforma->Nombre;
		
	$terminos = taxonomy_get_tree(variable_get('vocabulario_modalidades', NULL));
	foreach($terminos as $indice => $termino) {
		$options[$termino->tid] = $termino->name;
	}
	$options['0'] = t('Crear categoria');
	
	$form[ 'id_Modalidad_Juego' ] = array
	(
		'#type' => 'hidden',
		'#value' => $Modalidad_Juego->id_Modalidad_Juego,
	);
	
	$form[ 'title' ] = array
	(
		'#type' => 'textfield',
		'#title' => t( 'Nombre' ),
		'#required' => TRUE,
		'#weight' => -8,
		'#default_value' => $Modalidad_Juego->Nombre,
	);
		
	$form[ 'listar' ][ 'id_Plataforma' ] = array
	(
		'#type' => 'select',
		'#title' => t( 'Plataforma' ),
		'#options' => $Plataforma_Nombres,
		'#default_value' => $Modalidad_Juego->id_Plataforma,
	);
	
	
	$form[ 'nombre_juego' ] = array
	(
		'#type' => 'textfield',
		'#title' => t( 'Nombre del juego' ),
		'#required' => TRUE,
		//'#weight' => -8,
		'#default_value' => $Modalidad_Juego->Nombre_Juego,
	);
	
	$form[ 'minimo_jugadores' ] = array
	(
		'#type' => 'textfield',
		'#title' => t( 'Mínimo de jugadores' ),
		'#required' => TRUE,
		//'#weight' => -8,
		'#default_value' => $Modalidad_Juego->Minimo_Jugadores,
	);
	
	
	$form[ 'maximo_jugadores' ] = array
	(
		'#type' => 'textfield',
		'#title' => t( 'Maximo de jugadores' ),
		'#required' => TRUE,
		//'#weight' => -8,
		'#default_value' => $Modalidad_Juego->Maximo_Jugadores,
	);
	
	$form[ 'tid' ] = array(
		'#type' => 'select',
		'#default_value' => $Modalidad_Juego->tid,
		'#options' => $options,
	);
	
	$form[ 'submit' ] = array(
		'#type' => 'submit',
		'#value' => t( 'Insertar' ),
	);
			
			
	$form[ '#validate' ] = array( 'modalidad_admin_insertar_validate' );
	
	return $form;	
}



/**
 *	Sumbit para la forma de edición
 */
function modalidad_juego_admin_editar_submit( $form, $form_state ) {
	if ($form_state['values']['tid'] == 0) {
		$term = array(
			'vid' => variable_get( 'vocabulario_modalidades', NULL ), // Vocabulary ID
			'name' => $form_state[ 'values' ][ 'title' ], // Term Name 
		);
			
		taxonomy_save_term( $term );
	}
	else {
		$term['tid'] = $form_state['values']['tid'];
	}
	
	db_set_active( 'eSM' );
		
	db_query
	(
		"UPDATE {Modalidad_Juego}
		SET
		id_Plataforma = %d,
		Nombre = '%s',
		Nombre_Juego = '%s',
		Minimo_Jugadores = %d,
		Maximo_Jugadores = %d,
		tid = %d
		WHERE
		id_Modalidad_Juego = %d",
		$form_state[ 'values' ][ 'id_Plataforma' ],
		$form_state[ 'values' ][ 'title' ],
		$form_state[ 'values' ][ 'nombre_juego' ],
		$form_state[ 'values' ][ 'minimo_jugadores' ],
		$form_state[ 'values' ][ 'maximo_jugadores' ],
		$term[ 'tid' ],
		$form_state[ 'values' ][ 'id_Modalidad_Juego' ]
	);
			
	db_set_active( 'default' );
	
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades_juegos" );

	
}
	
/**
 * Función para validar:
 * 
 */
function modalidades_juegos_admin_insertar_validate( $form, $form_state ) {
	
	
	$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
	$form_state[ 'values' ][ 'nombre_juego' ] = check_plain( $form_state[ 'values' ][ 'nombre_juego' ] );

	
	if( !is_numeric( $form_state[ 'values' ][ 'minimo_jugadores' ] ) || $form_state[ 'values' ][ 'minimo_jugadores' ] < 1 ) {
		form_set_error( 'minimo_jugadores', t( 'Introduce un número válido.' ) );
	}


	if( !is_numeric( $form_state[ 'values' ][ 'maximo_jugadores' ] ) || $form_state[ 'values' ][ 'maximo_jugadores' ] < 1 ) {
		form_set_error( 'maximo_jugadores', t( 'Introduce un número válido.' ) );
	}
		
	if( $form_state[ 'values' ][ 'maximo_jugadores' ] < $form_state[ 'values' ][ 'minimo_jugadores' ] ) {
		form_set_error( 'maximo_jugadores', t( 'El número de jugadores máximo debe ser mayor al mínimo.' ) );
	}
	
	if (( $form_state['values']['id_Modalidad_Juego'] == 0) && ($form_state['values']['tid'] <> 0 )) {
		db_set_active('eSM');
			$result = db_fetch_object(db_query('SELECT {id_Modalidad_Juego} FROM {Modalidad_Juego} WHERE tid = %d', $form_state['values']['tid']));
		db_set_active('default');
		
		if($result->id_Modalidad_Juego <> 0)
			form_set_error('vocabulario', t('No puedes asiganar a dos modaliades la misma categoría'));
	}
	
	if (($form_state['values']['id_Modalidad_Juego'] <> 0) && ($form_state['values'] ['tid'] <> 0)) {
		db_set_active('eSM');
			$result = db_fetch_object(db_query('SELECT {id_Modalidad_Juego} FROM {Modalidad_Juego} WHERE tid = %d'. $form_state['values']['tid']));
		db_set_active('default');
		
		if($result->id_Modalidad_Juego <> $form_state['values']['id_Modalidad_Juego'])
			form_set_error('vocabulario', t('No puedes asignar una misma categoría a dos modalidades diferentes.'));
	}
}
	
	/**	Function to list Modalidad_Juego's rows out.
	 *
	 */
	function modalidades_juegos_admin_listar ( ) {
		
		// Creating the date/time element starts here
		
		
		/*$form[ 'listar' ] = array(
			'#type' => 'fieldset',
			'#title' => t( 'Listar' ),
			'#collapsible' => 'TRUE',
			'#collapsed' => 'TRUE',
		);*/
		
		db_set_active( 'eSM' );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM {Modalidad_Juego} ORDER BY id_Modalidad_Juego ASC" );
		//$result_set_array = db_fetch_array( db_query( "SELECT * FROM {Modalidad_Juego}" ) );		
		db_set_active( 'default' );
		
		
		$terms_array = array(  );
		
		$term = array( );
		
		//Primero pondremos un listado de modalidads disponibles
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {

			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'title' ] = array (
				'#type' => 'item',
				'#title' => t( 'Nombre' ),
				'#weight' => -8,
				'#value' => $Modalidad_Juego->Nombre."\n",
			);			
			
			db_set_active( 'eSM' );
			$result_set_Plataforma = db_query( "SELECT * from {Plataforma} WHERE id_Plataforma = %d ", $Modalidad_Juego->id_Plataforma );
			db_set_active( 'default' );
			
			$Plataforma = db_fetch_object( $result_set_Plataforma );
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'id_plataforma' ] = array
			(
				'#type' => 'item',
				'#title' => t( 'Plataforma' ),
				'#value' => $Plataforma->Nombre,
			);
			
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'nombre_juego' ] = array (
				'#type' => 'item',
				'#title' => t( 'Nombre de juego' ),
				//'#weight' => -8,
				'#value' => $Modalidad_Juego->Nombre_Juego."\n",
			);
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'minimo_jugadores' ] = array (
				'#type' => 'item',
				'#title' => t( 'Mínimo de jugadores' ),
				//'#weight' => -8,
				'#value' => $Modalidad_Juego->Minimo_Jugadores."\n",
			);
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'maximo_jugadores' ] = array (
				'#type' => 'item',
				'#title' => t( 'Maximo de jugadores' ),
				//'#weight' => -8,
				'#value' => $Modalidad_Juego->Maximo_Jugadores."\n",
			);
			
			$form[ 'listar' ][ $Modalidad_Juego->id_Modalidad_Juego ][ 'editar' ] = array (
				'#value' => l( t( 'Editar' ), 'admin/base_datos_externa/modalidades_juegos/'.$Modalidad_Juego->id_Modalidad_Juego.'/editar' ),
			);

			
		}
		
		return $form;
		
	}
	
	
	
	
	
	
	
/**
 * Función para presentar la forma para insertar una nueva modalidad
 */
function modalidades_juegos_admin_insertar ( )	 {
	
	db_set_active( 'eSM' );		
		$result_set_plataformas = db_query( "SELECT * FROM Plataforma" );
	db_set_active( 'default' );

	if (isset($result_set_plataformas)) {
		while( $Plataforma = db_fetch_object( $result_set_plataformas ) ) {
			$Plataforma_Nombres[ $Plataforma->id_Plataforma ] = $Plataforma->Nombre;
		}
		
	$terminos = taxonomy_get_tree(variable_get('vocabulario_modalidades', NULL));
	foreach($terminos as $indice => $termino) {
		$options[$termino->tid] = $termino->name;
	}
	$options['0'] = t('Crear categoria');	
	
		$form[ 'insertar' ][ 'title' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'id_plataforma' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Plataforma' ),
			'#options' => $Plataforma_Nombres,
		);
		
		$form[ 'insertar' ][ 'nombre_juego' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Nombre del juego' ),
			'#required' => TRUE,
			//'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'minimo_jugadores' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Mínimo de jugadores' ),
			'#required' => TRUE,
			//'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'maximo_jugadores' ] = array
		(
			'#type' => 'textfield',
			'#title' => t( 'Máximo de jugadores' ),
			'#required' => TRUE,
			//'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'tid' ] = array
		(
			'#type' => 'select',
			'#title' => t( 'Categoria. Escoge Nueva para crear una categoría con el nombre de la Modaliad.' ),
			'#required' => TRUE,
			'#default_value' => 0,
			'#options' => $options,
			//'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'submit' ] = array(
			'#type' => 'submit',
			'#value' => t( 'Insertar' ),
		);
	}
	else {
		$form['insertar']['error'] = array (
			'#value' => t('You need to setup the Plataforma section first. Please.'),
		);
	}
	return $form;
	
}



/**
 *	Implementación del hook submit
 *	para la tabla modalidad de juego por temporada al insertar
 */
function modalidades_juegos_admin_insertar_submit( $form, $form_state ) {
	if ($form_state['values']['tid'] == 0) {
		$term = array(
			'vid' => variable_get( 'vocabulario_modalidades', NULL ), // Vocabulary ID
			'name' => $form_state[ 'values' ][ 'title' ], // Term Name 
		);
			
		taxonomy_save_term( $term );
	}
	else {
		$term['tid'] = $form_state['values']['tid'];
	}
	
	db_set_active( 'eSM' );
	
	db_query
	( 
		"INSERT INTO { Modalidad_Juego } 
		( 
			id_Plataforma,
			Nombre,
			Nombre_Juego,
			Minimo_Jugadores,
			Maximo_Jugadores,
			tid
		) 
			VALUES ( %d, '%s', '%s', %d, %d, %d )",
			$form_state[ 'values' ][ 'id_plataforma' ],
			$form_state[ 'values' ][ 'title' ],
			$form_state[ 'values' ][ 'nombre_juego' ],
			$form_state[ 'values' ][ 'minimo_jugadores' ],
			$form_state[ 'values' ][ 'maximo_jugadores' ],
			$term[ 'tid' ]
			
	);

	db_set_active( 'default' );
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades_juegos" );
	
}