<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_team_plantilla ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');

    //$output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $header = array(t(' '), t('Jugador'), t('Rol'), t('Incorporacion'), t('GameTag'), t('Relaciones'));
    $rows = array();
    
    foreach (element_children($form['jugadores_activos']) as $element)   {
        $row = array();
        $row[] = drupal_render($form['jugadores_activos'][$element]['imagen']);
        $row[] = drupal_render($form['jugadores_activos'][$element]['nombre']);
        $row[] = drupal_render($form['jugadores_activos'][$element]['rol']);
        $row[] = drupal_render($form['jugadores_activos'][$element]['incorporacion']);
        $row[] = drupal_render($form['jugadores_activos'][$element]['gametag']);
        $row[] = drupal_render($form['jugadores_activos'][$element]['relaciones']);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}