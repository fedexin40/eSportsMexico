<?php

/**
 * Función para desplegar la vista de actividad de la comunidad en una página
 */
function taxonomy_term_rules () {
  $tid = arg(2);
  $modalidad = term_is_active($tid);
  
  
  //Obtenemos las reglas
  db_set_active('eSM');
	$reglas = db_fetch_object(db_query('SELECT * FROM {Reglas} WHERE id_Modalidad_Juego = %d', $modalidad->id_Modalidad_Juego));
  db_set_active('default');
  
  //Colocamos el título de la pestaña
  drupal_set_title(t('Reglamento '. $modalidad->Nombre));
  
  //Agregamos un div para poner las reglas para descarga
  $content = '<div class="rules_download">'. l('Download', $reglas->Archivo) .'</div>';
  
  //Agregamos un div para poner las reglas escritas
  $content .= '<div class="rules">'. $reglas->Contenido .'</div>';
  
  return $content;
}