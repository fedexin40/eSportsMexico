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
	db_set_active( 'eSM' );
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM {Modalidad_Juego} ORDER BY id_Modalidad_Juego ASC" );
	db_set_active( 'default' );
	
	$output = '<div class="form_item">Lista de modalidades dadas de alta en la base de datos.</div>';
	$header = array (t('Name'), t('Plataforma'), t('Game'), t('Minimo Jugadores'), t('Maximo Jugadores'), '');
	
	//Pondremos un listado de modalidads disponibles
	while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {
		db_set_active( 'eSM' );
				$result_set_Plataforma = db_query( "SELECT * from {Plataforma} WHERE id_Plataforma = %d ", $Modalidad_Juego->id_Plataforma );
		db_set_active( 'default' );
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
	
	db_set_active( 'eSM' );
	
		db_query("INSERT INTO { Modalidad_Juego }
												(id_Plataforma, Nombre, Nombre_Juego, Minimo_Jugadores, Maximo_Jugadores, tid, reglas) 
												VALUES ( %d, '%s', '%s', %d, %d, %d, %d )",
												$form_state[ 'values' ][ 'id_plataforma' ], $form_state[ 'values' ][ 'title' ],	 $form_state[ 'values' ][ 'nombre_juego' ],	 $form_state[ 'values' ][ 'minimo_jugadores' ],	$form_state[ 'values' ][ 'maximo_jugadores' ],	 $term[ 'tid' ],	$form_state[ 'values' ][ 'reglas' ]
		);
	db_set_active( 'default' );
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades_juegos" );
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

	while( $Plataforma = db_fetch_object( $result_set_Plataforma ) )					//Obtine las posibles plataformas para esta modalidad
		$Plataforma_Nombres[ $Plataforma->id_Plataforma ] = $Plataforma->Nombre;
		
	$terminos = taxonomy_get_tree(variable_get('vocabulario_modalidades', NULL));		//Obtiene las posibles categorías para esta modalidad
	foreach($terminos as $indice => $termino) {
		$options[$termino->tid] = $termino->name;
	}
	$options['0'] = t('Crear categoria');
	
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
		db_set_active('eSM');
				$mapas = db_query('SELECT * FROM {Mapa} WHERE id_Modalidad_Juego  = %d', $id_Modalidad_Juego);
		db_set_active('default');
		
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
		db_set_active('eSM');
				$modos = db_query('SELECT * FROM {Modo_Juego} WHERE id_Modalidad_Juego  = %d', $id_Modalidad_Juego);
		db_set_active('default');
		
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
	
	db_set_active( 'eSM' );
		
	db_query("UPDATE {Modalidad_Juego}
						SET id_Plataforma = %d, Nombre = '%s', Nombre_Juego = '%s', Minimo_Jugadores = %d, Maximo_Jugadores = %d, tid = %d, reglas = %d
						WHERE id_Modalidad_Juego = %d",
						$form_state[ 'values' ][ 'id_Plataforma' ], $form_state[ 'values' ][ 'title' ], $form_state[ 'values' ][ 'nombre_juego' ], $form_state[ 'values' ][ 'minimo_jugadores' ], $form_state[ 'values' ][ 'maximo_jugadores' ], $term[ 'tid' ], $form_state[ 'values' ][ 'reglas' ], $form_state[ 'values' ][ 'id_Modalidad_Juego' ]
		);		
	db_set_active( 'default' );
	
	drupal_set_message( "La modalidad de juego fué guardada satisfactoriamente" );
	drupal_goto( "admin/base_datos_externa/modalidades" );
}
