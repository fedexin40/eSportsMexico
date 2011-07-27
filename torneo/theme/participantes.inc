<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_participantes ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');

    //$output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $header = array(t(' '), t(' '), t(' '), t(' '));
    $rows = array();
    $row = array();
    $count = 1;
    
    foreach (element_children($form['participantes']) as $element)   {
        if ($count < 5) {
            ++$count;
            $row[] = drupal_render($form['participantes'][$element]['nombre']);
        }
        else {
            $rows[] = $row;
            $count = 1;
            $row = array();
            $row[] = drupal_render($form['participantes'][$element]['nombre']);
        }
    }
    $rows[] = $row;
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}