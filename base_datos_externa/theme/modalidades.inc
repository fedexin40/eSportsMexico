<?php

/**
 * Funci�n para hacer la personalizaci�n de la vista de puntuaciones de desaf�os
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