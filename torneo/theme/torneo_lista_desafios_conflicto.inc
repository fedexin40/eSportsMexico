<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_torneo_lista_desafios_conflicto ($form) {
    $output = '';
    $output .= drupal_render($form['Encabezado']);
    
    $header = array(t('Local'), t('Visitante'), t('Fecha'), t('Ronda'), t('Partido'), t('Actualiza'));
    $rows = array();
    
    foreach (element_children($form) as $element)   {
        if (is_numeric($element)) {
            $row = array();
            $row[] = drupal_render($form[$element]['Local']);
            $row[] = drupal_render($form[$element]['Visitante']);
            $row[] = drupal_render($form[$element]['Fecha']);
            $row[] = drupal_render($form[$element]['Ronda']);
            $row[] = drupal_render($form[$element]['Partido']);
            $row[] = drupal_render($form['Ganador_'. $element]);
            $rows[] = $row;
        }
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}