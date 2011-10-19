<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_team_inscripcion ($form) {
    //$output = t('<p>Equipos en formacion.</p>');
    
    $header = array(t(' '), t('Equipo'), t('Tag'), t('Recruiting'), t('Win'), t('Lose'), t('Reputation'));
    $rows = array();
    
    foreach (element_children($form['equipos_disponibles']) as $element)   {
        $row = array();
        $row[] = drupal_render($form['equipos_disponibles'][$element]['imagen']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['nombre']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['tag']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['recruit']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['win']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['lose']);
        $row[] = drupal_render($form['equipos_disponibles'][$element]['reputation']);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}