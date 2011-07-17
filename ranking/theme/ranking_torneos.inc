<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_ranking_torneos ($form) {
    //die('<pre>' . print_r($form, TRUE) . '</pre>');
    
    $output = '<p>Por favor ingresa la cantidad de puntos que van a ganar los equipos de acuerdo con el resultado del torneo.</p>';
    $output .= t('<p>De acuerdo al numero de inscritos y la ronda alcanzada.</p>');
    
    $header = array(t('Inscritos'), t('1o'), t('2o'), t('Semifinal'), t('Cuartos'), t('Octavos'), t('1/16'), t('1/32'), t('1/64'), t('1/128'), t('1/256'), t('1/512')) ;
    $rows = array();
    
    for ($i = 0; $i < 5; $i++){
        $row = array();
        $row[] = drupal_render($form['Inscritos_'. $i]);
        $row[] = drupal_render($form['1_'. $i]);
        $row[] = drupal_render($form['2_'. $i]);
        $row[] = drupal_render($form['4_'. $i]);
        $row[] = drupal_render($form['8_'. $i]);
        $row[] = drupal_render($form['16_'. $i]);
        $row[] = drupal_render($form['32_'. $i]);
        $row[] = drupal_render($form['64_'. $i]);
        $row[] = drupal_render($form['128_'. $i]);
        $row[] = drupal_render($form['256_'. $i]);
        $row[] = drupal_render($form['512_'. $i]);
        $row[] = drupal_render($form['1024_'. $i]);
        $rows[] = $row;
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}