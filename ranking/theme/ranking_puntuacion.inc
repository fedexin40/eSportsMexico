<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_ranking_puntuacion ($form) {
    $output = '';
    
    $header = array(t('Pos.'), t('Equipo'), t('DJ'), t('DG'), t('DP'), t('Victorias'), t('TJ'),  t('Racha'), t('Actividad'), t('Puntos'));
    $rows = array();
    
    foreach (element_children($form) as $element)   {
        if ($element != "form_build_id" && $element != "form_token" && $element != "form_id") {
            $row = array();
            $row[] = drupal_render($form[$element]['Ranking']);
            $row[] = drupal_render($form[$element]['Title']);
            $row[] = drupal_render($form[$element]['PJ']);
            $row[] = drupal_render($form[$element]['PG']);
            $row[] = drupal_render($form[$element]['PP']);
            $row[] = drupal_render($form[$element]['Victorias']);
            $row[] = drupal_render($form[$element]['TJ']);
            $row[] = drupal_render($form[$element]['Racha']);
            $row[] = drupal_render($form[$element]['Actividad']);
            $row[] = drupal_render($form[$element]['Puntuacion']);
            $rows[] = $row;
        }
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}