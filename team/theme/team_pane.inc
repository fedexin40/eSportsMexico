<?php

/**
 * Función para dibujar el bloque que en que aparece el logo del equipo
 */
function theme_team_pane ($form) {
    
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