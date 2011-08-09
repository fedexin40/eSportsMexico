<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_match_lista_desafios ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    //$output = '<p>Por favor ingresa la cantidad de puntos que van a ganar o perder los equipos de acuerdo con el resultado de los desafios</p>';
    //$output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $output = '';
    
    $header = array(t('Rival'), t('Fecha'), t('Resultado'), t('Puntos'), t('Partido'));
    $rows = array();
    
    foreach (element_children($form) as $element)   {
        if ($element != "form_build_id" && $element != "form_token" && $element != "form_id") {
            $row = array();
            $row[] = drupal_render($form[$element]['Rival']);
            $row[] = drupal_render($form[$element]['Fecha']);
            $row[] = drupal_render($form[$element]['Resultado']);
            $row[] = drupal_render($form[$element]['Puntos']);
            $row[] = drupal_render($form[$element]['Partido']);
            $rows[] = $row;
        }
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}