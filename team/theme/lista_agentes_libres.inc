<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_lista_agentes_libres ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $header = array(t('Imagen'), t('Jugador'), t('Presentacion'), t('Dias'), t('Horarios'), t(' ')) ;
    $rows = array();
    
    foreach (element_children($form['Jugadores_Libres']) as $user)   {
        
        $row = array();
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['Imagen']);
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['Nombre']);
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['Descripcion']);
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['Dias']);
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['Horas']);
        $row[] = drupal_render($form['Jugadores_Libres'][$user]['link']);
        
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}