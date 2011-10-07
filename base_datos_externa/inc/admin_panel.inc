<?php

/**
 *	Aquí checaremos que exista un vocabulario en drupal para poder trabajar
 *	con él vocabulario_modalidades.
 *	vocabulario_modalidades es una variable guardada en la base de datos de
 *	drupal para asegurrnos de que se ligue el vocabulario con la modalidad
 */
function bd_admin_panel( ) {
	$vocabulario = variable_get('vocabulario_modalidades', NULL);
	
	if ($vocabulario == NULL) 
		drupal_set_message(t('No se ha definido ningún vocabulario para las modalidades.'), 'error');

	$form[ 'informacion' ] = array (
		'#type' => 'item',
		'#disabled' => FALSE,
		'#value' => 'Panel de administración de las bases de datos',
	);
	
	$tree = menu_navigation_links('navigation', 2);

    $main_menu = menu_get_item('admin/base_datos_externa');
    $content = system_admin_menu_block((array)$main_menu);

	$output = theme('admin_block_content', $content);
  
	return $output;
}
