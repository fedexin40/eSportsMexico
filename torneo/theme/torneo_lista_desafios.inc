<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_torneo_lista_desafios ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $output = '';
    
    $header = array(t('Local'), t('Visitante'), t('Fecha'), t('Ganador'), t('Ronda'), t('Partido'));
    $rows = array();
    
    foreach (element_children($form) as $element)   {
        if ($element != "form_build_id" && $element != "form_token" && $element != "form_id") {
            $row = array();
            $row[] = drupal_render($form[$element]['Local']);
            $row[] = drupal_render($form[$element]['Visitante']);
            $row[] = drupal_render($form[$element]['Fecha']);
            $row[] = drupal_render($form[$element]['Resultado']);
            $row[] = drupal_render($form[$element]['Ronda']);
            $row[] = drupal_render($form[$element]['Partido']);
            $rows[] = $row;
        }
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}