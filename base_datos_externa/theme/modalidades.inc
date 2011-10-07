<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_modalidades ($form) {
    $header = array(t(' '), t('Modalidad')) ;
    $rows = array();
    
    foreach (element_children($form['Solicitudes']) as $modalidad)   {
        $row = array();
        $row[] = drupal_render($form['checkboxes'][$modalidad]);
        $row[] = drupal_render($form['insertar'][$modalidad]['nombre']);
        
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);

    return $output . drupal_render($form);
}