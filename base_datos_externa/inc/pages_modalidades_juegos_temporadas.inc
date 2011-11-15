<?php
/**
 *	@file
 *	Form callbacks for Modalidad_Juego_Temporada table
 */
/**
 *	Función para insertar datos en la tabla modalidades de juegos por temporadas
 */
function modalidades_temporada ( ) {
	$id_Temporada = arg(3);
	$result = db_result(db_query('SELECT Estado FROM {eSM_Temporada} WHERE id_Temporada = %d', $id_Temporada));
		
	if ($result <> TEMPORADA_ACTIVA) {
		drupal_set_message(t('No se pueden agregar modalidades a una temporada que no está activa'), 'error');
		drupal_goto	('admin/base_datos_externa/temporada');
	}
	else {
		$result_set_Modalidad_Juego = db_query( "SELECT * FROM {eSM_Modalidad_Juego}" );
				
		$count = 0;
		while( $Modalidad_Juego = db_fetch_object( $result_set_Modalidad_Juego ) ) {
			$result = db_fetch_array(db_query('SELECT * FROM {eSM_Modalidad_Juego_Temporada }
											  WHERE id_Modalidad_Juego = %d AND id_Temporada = %d',
											  $Modalidad_Juego->id_Modalidad_Juego, $id_Temporada));
			
			if (!is_array($result)) {
				++ $count;
				$form['insertar'][$Modalidad_Juego->id_Modalidad_Juego][ 'nombre' ] = array (
					'#value' => $Modalidad_Juego->Nombre,
				);
				
				$checkboxes[$Modalidad_Juego->id_Modalidad_Juego] = '';
			}
		}
		
		if ($count == 0) {
			$form['mensaje'] = array (
				'#value' => t('<p>Todas las modalidades que existen actualmente están activas en esta temporada.</p>'),
			);
		}
		else {
			$form[ 'checkboxes' ] = array (
				'#type' => 'checkboxes',
				'#options' => $checkboxes,
			);
			$form[ 'Temporada' ] = array (
				'#type' => 'hidden',
				'#value' => $id_Temporada,
			);
			$form[ 'submit' ] = array (
				'#type' => 'submit',
				'#value' => t( 'Activar Seleccionadas' ),
			);
			$form['#theme'] = 'modalidades';
		}
	}
	return $form;
}
/**
 *	Implementación del hook submit
 *	para la tabla modalidad de juego por temporada
 */
function modalidades_temporada_submit( $form, $form_state ) {
  $count = 0; //Contador para mostrar el número de jugadores expulsados

  foreach( element_children($form_state['values']['checkboxes']) as $modalidad ) { 
    if ($form_state['values']['checkboxes'][$modalidad] != 0) {
      ++$count;
	db_query('INSERT INTO {eSM_Modalidad_Juego_Temporada }
			 (id_Modalidad_Juego, id_Temporada)
			 VALUES ( %d, %d )',
			 $modalidad, $form_state[ 'values' ][ 'Temporada' ]);
    }
  }

  if ($count > 0) {
    drupal_set_message( "Has activado ". $count ." modalidades." );
    drupal_goto( 'admin/base_datos_externa/temporada');
  }
  else {
    drupal_set_message('No has seleccionado ninguna modalidad para activar.', 'error');
  }
}
/**
 *	Implementación del hook submit
 *	para la tabla modalidad de juego por temporada eliminar
 */
function desactivar_modalidades_temporada( ) {
	return confirm_form (
		$form,
		t('¿Realmente quieres desactivar la modalidad seleccionada?'),
		isset($_GET['destination']) ? $_GET['destination'] : "admin/base_datos_externa/temporada",
		t('Todas las estadísticas de los equipos inscritos se borrarán.'),
		t('Si'),
		t('Cancelar')
	);
}
/**
 *	Implementación del hook submit
 *	para la tabla modalidad de juego por temporada eliminar
 */
function desactivar_modalidades_temporada_submit( ) {
	$id_temporada = arg(3);
	$id_modalidad_juego = arg(5);
	
	db_query('DELETE FROM {eSM_Modalidad_Juego_Temporada}
			 WHERE id_Temporada = %d AND id_Modalidad_Juego = %d',
			 $id_temporada, $id_modalidad_juego);
	
	drupal_set_message('La modalidad seleccionada ha sido eliminada de la temporada actual.');
	drupal_goto('admin/base_datos_externa/temporada');
}