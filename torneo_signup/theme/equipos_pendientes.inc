<?php

/**
 * Funci�n para hacer la personalizaci�n de la vista de puntuaciones de desaf�os
 */
function theme_equipos_pendientes ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');

    //$output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $header = array(t('Equipo'), t('No. Jugadores Inscritos'));
    $rows = array();
    $row = array();
    $count = 1;
    
    foreach (element_children($form['pendientes']) as $element)   {
        $row[] = drupal_render($form['pendientes'][$element]['Nombre']);
        $row[] = drupal_render($form['pendientes'][$element]['Inscritos']);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}