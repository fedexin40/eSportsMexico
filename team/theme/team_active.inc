<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_team_active ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $header = array(t('Remove'), t('Jugador'), t('Mensaje')) ;
    $rows = array();
    
    foreach (element_children($form['Solicitudes']) as $user)   {
        
        $row = array();
        $row[] = drupal_render($form['checkboxes'][$user]);
        $row[] = drupal_render($form['Solicitudes'][$user]['nombre']);
        $row[] = drupal_render($form['Solicitudes'][$user]['mensaje']);
        
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}