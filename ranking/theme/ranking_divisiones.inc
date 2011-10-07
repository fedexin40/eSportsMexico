<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_ranking_divisiones ($form) {
    
    $output = t('<p>Por favor ingresa el nombre de cada division y la cantidad de jugadores que forman parte de ella.</p>');
    $output .= t('<p>Al llegar a la ultima divison pon de numero de jugadores 0 para que el sistema ponga a todos los jugadores restantes en esa division.</p>');
    
    $header = array(t('Division'), t('Nombre'), t('Jugadores')) ;
    $rows = array();
    
    for ($i = 1; $i <= 5; $i++){
        $row = array();
        $row[] = drupal_render($form['Division_'. $i]);
        $row[] = drupal_render($form['Nombre_'. $i]);
        $row[] = drupal_render($form['Jugadores_'. $i]);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}