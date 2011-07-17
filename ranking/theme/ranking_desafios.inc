<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_ranking_desafios ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $output = '<p>Por favor ingresa la cantidad de puntos que van a ganar o perder los equipos de acuerdo con el resultado de los desafios</p>';
    $output .= '<p>Ingresa los puntos ganados/perdidos por los equipos, el de posicion superior a la izquierda y el de posicion inferior a la derecha.</p>';
    
    $header = array(t('Diferencia en Puntos'), t('(Superior) Ganados por Victoria'), t('(Superior) Perdidos por Derrota'), t('(Inferior) Ganados por Victoria'), t('(Inferior) Perdidos por Derrota')) ;
    $rows = array();
    
    for ($i = 0; $i < 11; $i++){
        $row = array();
        $row[] = drupal_render($form['Diferencia_'. $i]);
        $row[] = drupal_render($form['Equipo_Superior_Win_'. $i]);
        $row[] = drupal_render($form['Equipo_Superior_Loss_'. $i]);
        $row[] = drupal_render($form['Equipo_Inferior_Win_'. $i]);
        $row[] = drupal_render($form['Equipo_Inferior_Loss_'. $i]);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}