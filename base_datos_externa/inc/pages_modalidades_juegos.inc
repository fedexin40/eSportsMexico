<?php
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
		'#value' => 'Panel de administración de las modalidades de juego',
	);

	if( variable_get( 'vocabulario_modalidades', NULL ) == NULL ) {
		if ( $vocabularios = taxonomy_get_vocabularies( ) ) {
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
				'#value' => t('Save'),
				'#type' => 'submit',
			);
		}
		else {
			$form[ 'mensaje' ] = array (
				'#value' => t('No existe ningun vocabulario definido en la base de datos. Haz click en el boton de abajo para crear uno.</br>'),
			);
			$form[ 'submit' ] = array (
				'#type' => 'submit',
				'#value' => t('Create default vocabulary'),
			);
		}
	}
	else {
		drupal_goto('admin/base_datos_externa/modalidades/listar');
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
	if ($form_state['values']['vocabulary'] == 0) {
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
	else {	
			$vocabulary = taxonomy_vocabulary_load($form_state['values']['vocabulary']);
  }

	variable_set( 'vocabulario_modalidades', $vocabulary->vid );		//Le dice a drupal cual va a ser el vid del vocabulario para las modalidades
	drupal_set_message(t('Se han guardado los cambios correctamente'));
}
	

/**	Function to list Modalidad_Juego's rows out.
 *
 */
function modalidades_juegos_admin_listar ( ) {	
	$result_set_Modalidad_Juego = db_query( "SELECT * FROM {eSM_Modalidad_Juego} ORDER BY id_Modalidad_Juego ASC" );
		
	$output = '<div class="form_item">Lista de modalidades dadas de alta en la base de datos.</div>';
	$header = array (t('Name'), t('Plataforma'), t('Game'), t('Min Jugadores'), t('Max Jugadores'), '');
	
	//Pondremos un listado de modalidads disponibles
	while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {
		$result_set_Plataforma = db_query( "SELECT * from {eSM_Plataforma} WHERE id_Plataforma = %d ", $Modalidad_Juego->id_Plataforma );
		
		$Plataforma = db_fetch_object( $result_set_Plataforma );
		
		$row = array();
		$row[] = $Modalidad_Juego->Nombre;			
		$row[] = $Plataforma->Nombre;
		$row[] = $Modalidad_Juego->Nombre_Juego;
		$row[] = $Modalidad_Juego->Minimo_Jugadores;
		$row[] = $Modalidad_Juego->Maximo_Jugadores;
		$row[] = l( t( 'Editar' ), 'admin/base_datos_externa/modalidades/'.$Modalidad_Juego->id_Modalidad_Juego.'/editar' );
		
		$rows[] = $row;
	}
	$output .= theme('table', $header, $rows);
	
	$output .= l(t('Agregar modalidad'), 'admin/base_datos_externa/modalidades/insertar');
	return $output;
}
/**
 * Función para presentar la forma para insertar una nueva modalidad
 */
function modalidades_juegos_admin_insertar ( )	 {
	$result_set_plataformas = db_query( "SELECT * FROM {eSM_Plataforma}" );
	
	if (isset($result_set_plataformas)) {
		while( $Plataforma = db_fetch_object( $result_set_plataformas ) ) {
			$Plataforma_Nombres[ $Plataforma->id_Plataforma ] = $Plataforma->Nombre;
		}
		
		//Obtiene la lista de categorías en las que se puede ubicar una modalidad
		$terminos = taxonomy_get_tree(variable_get('vocabulario_modalidades', NULL));
		foreach($terminos as $indice => $termino) {
			$options[$termino->tid] = $termino->name;
		}
		$options['0'] = t('Crear categoria');
		
		//Obtiene la lista de campos de identificador que se pueden asociar con una modalidad
		$type_name = 'profile';
		$type = content_types($type_name);
		$fields = $type['fields'];
		foreach($fields as $indice => $field) {
				$options_identificador[$indice] = $indice;
		}
		$options_identificador['crear'] = t('Crear identificador');
		
		$form[ 'insertar' ][ 'title' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
		);
		
		$form[ 'insertar' ][ 'id_plataforma' ] = array (
			'#type' => 'select',
			'#title' => t( 'Plataforma' ),
			'#options' => $Plataforma_Nombres,
		);
		
		$form[ 'insertar' ][ 'nombre_juego' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Nombre del juego' ),
			'#required' => TRUE,
		);
		
		$form[ 'insertar' ][ 'minimo_jugadores' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Mínimo de jugadores' ),
			'#required' => TRUE,
			'#size' => 2,
			'#maxlength' => 2,
		);
		
		$form[ 'insertar' ][ 'maximo_jugadores' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Máximo de jugadores' ),
			'#required' => TRUE,
			'#size' => 2,
			'#maxlength' => 2,
		);
		
		$form[ 'insertar' ][ 'tid' ] = array (
			'#type' => 'select',
			'#title' => t( 'Categoria. Escoge Nueva para crear una categoría con el nombre de la Modaliad.' ),
			'#required' => TRUE,
			'#default_value' => 0,
			'#options' => $options,
		);
		
		$form[ 'insertar' ][ 'identificador' ] = array (
			'#type' => 'select',
			'#title' => t( 'Identificador. Escoge "Nuevo Identificador" para crear un campo de identificador nuevo en el perfil de usuario.' ),
			'#required' => TRUE,
			'#default_value' => 0,
			'#options' => $options_identificador,
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
 * Función para validar:
 * 
 */
function modalidades_juegos_admin_insertar_validate( $form, $form_state ) {
	$form_state[ 'values' ][ 'title' ] = check_plain( $form_state[ 'values' ][ 'title' ] );
	$form_state[ 'values' ][ 'nombre_juego' ] = check_plain( $form_state[ 'values' ][ 'nombre_juego' ] );

	if( !is_numeric( $form_state[ 'values' ][ 'minimo_jugadores' ] ) || $form_state[ 'values' ][ 'minimo_jugadores' ] < 1 )
		form_set_error( 'minimo_jugadores', t( 'Introduce un número válido.' ) );

	if( !is_numeric( $form_state[ 'values' ][ 'maximo_jugadores' ] ) || $form_state[ 'values' ][ 'maximo_jugadores' ] < 1 ) 
		form_set_error( 'maximo_jugadores', t( 'Introduce un número válido.' ) );
		
	if( $form_state[ 'values' ][ 'maximo_jugadores' ] < $form_state[ 'values' ][ 'minimo_jugadores' ] ) 
		form_set_error( 'maximo_jugadores', t( 'El número de jugadores máximo debe ser mayor al mínimo.' ) );
	
	if (( $form_state['values']['id_Modalidad_Juego'] == 0) && ($form_state['values']['tid'] <> 0 )) {
		$result = db_fetch_object(db_query('SELECT {id_Modalidad_Juego} FROM {eSM_Modalidad_Juego} WHERE tid = %d', $form_state['values']['tid']));
		
		if($result->id_Modalidad_Juego <> 0)
			form_set_error('vocabulario', t('No puedes asiganar a dos modaliades la misma categoría'));
	}
	
	if (($form_state['values']['id_Modalidad_Juego'] <> 0) && ($form_state['values'] ['tid'] <> 0)) {
		$result = db_fetch_object(db_query('SELECT {id_Modalidad_Juego} FROM {eSM_Modalidad_Juego} WHERE tid = %d'. $form_state['values']['tid']));
		
		if($result->id_Modalidad_Juego <> $form_state['values']['id_Modalidad_Juego'])
			form_set_error('vocabulario', t('No puedes asignar una misma categoría a dos modalidades diferentes.'));
	}
}

/**
 *	Implementación del hook submit
 *	para la tabla modalidad de juego por temporada al insertar
 */
function modalidades_juegos_admin_insertar_submit( $form, $form_state ) {
	if ($form_state['values']['tid'] == 0) {
		$term = array (
			'vid' => variable_get( 'vocabulario_modalidades', NULL ), // Vocabulary ID
			'name' => $form_state[ 'values' ][ 'title' ], // Term Name 
		);
			
		taxonomy_save_term( $term );
	}
	else {
		$term['tid'] = $form_state['values']['tid'];
	}
	
	//Crea el campo de identificador en caso de ser necesario
	if ($form_state['values']['identificador'] == 'crear') {
		$identificador = modadalidad_juegos_admin_crear_identificador($form_state['values']['title']);
	}
	
	//Graba los datos en la base de datos
	db_query("INSERT INTO {eSM_Modalidad_Juego }
			 (id_Plataforma, Nombre, Nombre_Juego, Minimo_Jugadores, Maximo_Jugadores, tid, reglas)
			 VALUES ( %d, '%s', '%s', %d, %d, %d, %d )",
			 $form_state[ 'values' ][ 'id_plataforma' ], $form_state[ 'values' ][ 'title' ],	 $form_state[ 'values' ][ 'nombre_juego' ],	 $form_state[ 'values' ][ 'minimo_jugadores' ],	$form_state[ 'values' ][ 'maximo_jugadores' ],	 $term[ 'tid' ],	$form_state[ 'values' ][ 'reglas' ]
	);
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades_juegos" );
}
/**
 *	Función para mostrar la forma que permite editar una modalidad
 */
function modalidad_juego_admin_editar( ) {
	$id_Modalidad_Juego = arg( 3 );
	
	$result_set_Modalidad_Juego = db_query( "SELECT * FROM {eSM_Modalidad_Juego } WHERE { id_Modalidad_Juego } = %d", $id_Modalidad_Juego );
	$result_set_Plataforma = db_query( "SELECT * FROM {eSM_Plataforma }" );

	$Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego );

	while( $Plataforma = db_fetch_object( $result_set_Plataforma ) )					//Obtine las posibles plataformas para esta modalidad
		$Plataforma_Nombres[ $Plataforma->id_Plataforma ] = $Plataforma->Nombre;
		
	$terminos = taxonomy_get_tree(variable_get('vocabulario_modalidades', NULL));		//Obtiene las posibles categorías para esta modalidad
	foreach($terminos as $indice => $termino) {
		$options[$termino->tid] = $termino->name;
	}
	$options['0'] = t('Crear categoria');
	
  //Obtiene la lista de campos de identificador que se pueden asociar con una modalidad
  $type_name = 'profile';
  $type = content_types($type_name);
  $fields = $type['fields'];
  foreach($fields as $indice => $field) {
		$options_identificador[$indice] = $indice;
  }
  $options_identificador['crear'] = t('Crear identificador');
	
	$result = db_query("SELECT nid FROM {node} WHERE type = '%s'", variable_get('reglas_modalidades', NULL));	//Obtiene los posibles nodos de reglas para esta modalidad
	while($row = db_fetch_object($result)) {
	  $node = node_load($row->nid);
	  $reglas[$node->nid] = $node->title;
	}
	
		$form['detalles'] = array (
				'#type' => 'fieldset',
				'#title' => 'Detalles',
				'#collapsible' => TRUE,
		);
		
		$form['detalles'][ 'id_Modalidad_Juego' ] = array (
				'#type' => 'hidden',
				'#value' => $Modalidad_Juego->id_Modalidad_Juego,
		);
		
		$form['detalles'][ 'title' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Nombre' ),
			'#required' => TRUE,
			'#weight' => -8,
			'#default_value' => $Modalidad_Juego->Nombre,
		);
			
		$form['detalles'][ 'listar' ][ 'id_Plataforma' ] = array (
			'#type' => 'select',
			'#title' => t( 'Plataforma' ),
			'#options' => $Plataforma_Nombres,
			'#default_value' => $Modalidad_Juego->id_Plataforma,
		);
		
		$form['detalles'][ 'nombre_juego' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Nombre del juego' ),
			'#required' => TRUE,
			'#default_value' => $Modalidad_Juego->Nombre_Juego,
		);
		
		$form['detalles'][ 'minimo_jugadores' ] = array(
			'#type' => 'textfield',
			'#title' => t( 'Mínimo de jugadores' ),
			'#required' => TRUE ,
			'#default_value' => $Modalidad_Juego->Minimo_Jugadores,
			'#size' => 2,
			'#maxlength' => 2,
		);
		
		$form['detalles'][ 'maximo_jugadores' ] = array (
			'#type' => 'textfield',
			'#title' => t( 'Maximo de jugadores' ),
			'#required' => TRUE,
			'#default_value' => $Modalidad_Juego->Maximo_Jugadores,
			'#size' => 2,
			'#maxlength' => 2,
		);
		
		$form['detalles'][ 'tid' ] = array (
			'#type' => 'select',
			'#title' => t( 'Categoria.' ),
			'#default_value' => $Modalidad_Juego->tid,
			'#options' => $options,
		);
		
		$form[ 'detalles' ][ 'identificador' ] = array (
			'#type' => 'select',
			'#title' => t( 'Identificador. Escoge "Nuevo Identificador" para crear un campo de identificador nuevo en el perfil de usuario.' ),
			'#required' => TRUE,
			'#default_value' => $Modalidad_Juego->identificador,
			'#options' => $options_identificador,
		);
		
		$form['mapas'] = array (
				'#type' => 'fieldset',
				'#title' => 'Mapas',
				'#collapsible' => TRUE,
		);
		
		$form['detalles'][ 'submit' ] = array (
			'#type' => 'submit',
			'#value' => t( 'Save' ),
		);
		
		//Ponemos una lista de todos los mapas dados de alta en esta modalidad
		$mapas = db_query('SELECT * FROM {eSM_Mapa} WHERE id_Modalidad_Juego  = %d', $id_Modalidad_Juego);
				
		$count = 0;
		while ($mapa = db_fetch_object($mapas)) {
				++ $count;
				$form['mapas']['detalles'][$mapa->id_Mapa]['nombre'] = array (
						'#value' => $mapa->Nombre,
						'#prefix' => '<tr><td>',
            '#suffix' => '</td>',
				);
				$form['mapas']['detalles'][$mapa->id_Mapa]['editar'] = array (
						'#value' => l(t('Edit'), 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/mapas/'. $mapa->id_Mapa .'/editar'),
						'#prefix' => '<td>',
            '#suffix' => '</td>',
				);
				$form['mapas']['detalles'][$mapa->id_Mapa]['borrar'] = array (
						'#value' => l(t('Delete'), 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/mapas/'. $mapa->id_Mapa .'/eliminar'),
						'#prefix' => '<td>',
            '#suffix' => '</td></tr>',
				);
		}
		
		if ($count == 0) {
				$form['mapas']['mensaje'] = array (
						'#value' => t('No hay mapas agregados a esta modalidad todavia.'),
						'#weight' => -8,
				);
		}
		else {
				$form['mapas']['detalles']['#prefix'] = '<table>';
				$form['mapas']['detalles']['#suffix'] = '</table>';
		}
		
		$form['mapas']['link'] = array (
				'#value' => l(t('Add new map'), 'admin/base_datos_externa/modalidades/' . $id_Modalidad_Juego . '/mapas/insertar'),
				'#weight' => 0,
		);

		$form['modos'] = array (
				'#type' => 'fieldset',
				'#title' => 'Modos',
				'#collapsible' => TRUE,
		);
		
		//Ponemos una lista de todos los modos de juego dados de alta en esta modalidad
		$modos = db_query('SELECT * FROM {eSM_Modo_Juego} WHERE id_Modalidad_Juego  = %d', $id_Modalidad_Juego);
				
		$count = 0;
		while ($modo = db_fetch_object($modos)) {
				++ $count;
				$form['modos']['detalles'][$modo->id_Modo_Juego]['nombre'] = array (
						'#value' => $modo->Nombre,
						'#prefix' => '<tr><td>',
            '#suffix' => '</td>',
				);
				$form['modos']['detalles'][$modo->id_Modo_Juego]['editar'] = array (
						'#value' => l(t('Edit'), 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/modos/'. $modo->id_Modo_Juego .'/editar'),
						'#prefix' => '<td>',
            '#suffix' => '</td>',
				);
				$form['modos']['detalles'][$modo->id_Modo_Juego]['borrar'] = array (
						'#value' => l(t('Delete'), 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/modos/'. $modo->id_Modo_Juego .'/eliminar'),
						'#prefix' => '<td>',
            '#suffix' => '</td></tr>',
				);
		}
		
		if ($count == 0) {
				$form['modos']['mensaje'] = array (
						'#value' => t('No hay modos de juego agregados a esta modalidad todavia.'),
						'#weight' => -8,
				);
		}
		else {
				$form['modos']['detalles']['#prefix'] = '<table>';
				$form['modos']['detalles']['#suffix'] = '</table>';
		}
		
		$form['modos']['link'] = array (
				'#value' => l(t('Add new mod'), 'admin/base_datos_externa/modalidades/' . $id_Modalidad_Juego . '/modos/insertar'),
				'#weight' => 8,
		);
		
		//Ponemos las reglas de la modalidad y un link para editarlas
		$reglas = db_fetch_object(db_query('SELECT * FROM {eSM_Reglas} WHERE id_Modalidad_Juego = %d', $id_Modalidad_Juego));
		
		$form['rules'] = array (
			'#type' => 'fieldset',
			'#title' => t('Rules'),
			'#collapsible' => TRUE,
			'#collapsed' => TRUE,
		);
		
		if (is_object($reglas)) {
			$form['rules']['file'] = array (
				'#type' => 'item',
				'#value' => l($reglas->Archivo, $reglas->Archivo),
				'#title' => t('File'),
			);
			$form['rules']['contenido'] = array (
				'#type' => 'textarea',
				'#disabled' => TRUE,
				'#value' => $reglas->Contenido,
				'#title' => t('Reglas'),
			);
			$form['rules']['editar'] = array (
				'#value' => l('Editar Reglas', 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/reglas/editar'),
			);
		}
		else {
			$form['reglas']['#collapsed'] = FALSE;
			$form['reglas']['agregar'] = array (
				'#type' => 'item',
				'#value' => l('Agregar Reglas', 'admin/base_datos_externa/modalidades/'. $id_Modalidad_Juego .'/reglas/insertar'),
			);
		}
		$form[ '#validate' ] = array( 'modalidad_admin_insertar_validate' );
	
	return $form;	
}



/**
 *	Sumbit para la forma de edición
 */
function modalidad_juego_admin_editar_submit( $form, $form_state ) {
	if ($form_state['values']['tid'] == 0) {
		$term = array (
			'vid' => variable_get( 'vocabulario_modalidades', NULL ), // Vocabulary ID
			'name' => $form_state[ 'values' ][ 'title' ], // Term Name 
		);
			
		taxonomy_save_term( $term );
	}
	else {
		$term['tid'] = $form_state['values']['tid'];
	}
	
	//Crea el campo de identificador en caso de ser necesario
	if ($form_state['values']['identificador'] == 'crear') {
		$identificador = modadalidad_juegos_admin_crear_identificador($form_state['values']['title']);
	}
		
	db_query("UPDATE {eSM_Modalidad_Juego}
			 SET id_Plataforma = %d, Nombre = '%s', Nombre_Juego = '%s', Minimo_Jugadores = %d, Maximo_Jugadores = %d, tid = %d, identificador = '%s', reglas = %d
			 WHERE id_Modalidad_Juego = %d",
			 $form_state[ 'values' ][ 'id_Plataforma' ], $form_state[ 'values' ][ 'title' ], $form_state[ 'values' ][ 'nombre_juego' ], $form_state[ 'values' ][ 'minimo_jugadores' ], $form_state[ 'values' ][ 'maximo_jugadores' ], $term[ 'tid' ], $form_state['values']['identificador'], $form_state[ 'values' ][ 'reglas' ], $form_state[ 'values' ][ 'id_Modalidad_Juego' ]
		);		
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades" );
}
/**
 * Función para crear el campo de identificador
 */
function modadalidad_juegos_admin_crear_identificador ($name) {

  module_load_include('inc', 'content', 'includes/content.crud');

  $field = array (
		'field_name' => 'field_'. str_replace( " ", "_", $name),
		'type_name' => 'profile',
		'display_settings' => array (
			'label' => array (
				'format' => 'above',
				'exclude' => 0,
				),
			'teaser' => array (
				'format' => 'default',
				'exclude' => 0,
				),
			'full' => array (
				'format' => 'default',
				'exclude' => 0,
				),
			4 => array (
				'format' => 'default',
				'exclude' => 0,
				),
			'token' => array (
				'format' => 'default',
				'exclude' => 0,
				),
		),
		'widget_active' => '1',
		'type' => 'text',
		'required' => '0',
		'multiple' => '0',
		'db_storage' => '1',
		'module' => 'text',
		'active' => '1',
		'locked' => '0',
		'columns' => array (
			'value' => array (
				'type' => 'varchar',
				'length' => '45',
				'not null' => false,
				'sortable' => true,
				'views' => true,
			),
		),
		'text_processing' => '0',
		'max_length' => '45',
		'allowed_values' => '',
		'allowed_values_php' => '',
		'widget' => array (
			'rows' => 5,
			'size' => '45',
			'default_value' => array (
				0 => 
				array ('value' => '',
					'_error_element' => 'default_value_widget][field_'. str_replace( " ", "_", $name) .'][0][value',
				),
			),
			'default_value_php' => NULL,
			'label' => 'identificador_'. str_replace( " ", "_", $name),
			'weight' => '31',
			'description' => 'Escribe tu identificador online de'. $name,
			'type' => 'text_textfield',
			'module' => 'text',
		),
	);

  content_field_instance_create($field);
	drupal_set_message(t('Se ha creado el identificador onlie'));
	
	return $field['field_name'];
}