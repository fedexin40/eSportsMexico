<?php

/**
 * Función para desplegar la vista de actividad de la comunidad en una página
 */
function taxonomy_term_comunity () {
  $tid = arg(2);
  $modalidad = term_is_active($tid);
  //Colocamos el título de la pestaña
  drupal_set_title(t('Comunidad de '. $modalidad->Nombre));
  
  //Agregamos el bloque para escribir nuevos mensajes
  $block =  module_invoke('facebook_status', 'block' , 'view', 'facebook_status');
  $content = $block['content'];

  //Agregamos la vista para mostrar todos los estados
  $content .= views_embed_view('facebook_status_stream', 'default');
  
  //Agregamos la vista para mostrar la actividad
  //Esta debería de ser la única que se muestra, pero todavía no funciona
  $content .= views_embed_view('activity_log_stream', 'default');
  return $content;
}