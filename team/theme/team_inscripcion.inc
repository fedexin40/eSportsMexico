<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_team_inscripcion ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $output = t('<p>Equipos en formacion.</p>');
    //$output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $header = array(t(' '), t('Equipo'), t('Tag'), t('Capitan'), t('Lema'), t('Plazas'), t(' '));
    $rows = array();
    
    foreach (element_children($form['equipos_disponibles']) as $element)   {
        $row = array();
        $row[] = drupal_render($form['equipos_disponibles'][$element]['imagen']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['nombre']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['tag']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['capitan']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['lema']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['plazas']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['link']);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}