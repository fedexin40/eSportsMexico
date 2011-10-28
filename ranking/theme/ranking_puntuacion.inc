<?php

/**
 * Función para hacer la personalización de la vista de puntuaciones de desafíos
 */
function theme_ranking_puntuacion ($form) {
    $output = '';
    
    $header = array(t('Pos.'), '', t('Equipo'), t('DJ'), t('DG'), t('DP'), t('Win'), t('TJ'),  t('Racha'), t('Actividad'), t('Puntos'));
    $rows = array();
    
    foreach (element_children($form) as $element)   {
        if ($element != "form_build_id" && $element != "form_token" && $element != "form_id") {
            $row = array();
            $row[] = array('data' => drupal_render($form[$element]['Ranking']), 'class'=>'ranking');
            $row[] = array('data' => drupal_render($form[$element]['Image']), 'class'=>'image');
            $row[] = array('data' => drupal_render($form[$element]['Title']), 'class'=>'title');
            $row[] = array('data' => drupal_render($form[$element]['PJ']), 'class'=>'pj');
            $row[] = array('data' => drupal_render($form[$element]['PG']), 'class'=>'pg');
            $row[] = array('data' => drupal_render($form[$element]['PP']), 'class'=>'pp');
            $row[] = array('data' => drupal_render($form[$element]['Victorias']), 'class'=>'victorias');
            $row[] = array('data' => drupal_render($form[$element]['TJ']), 'class'=>'tj');
            $row[] = array('data' => drupal_render($form[$element]['Racha']), 'class'=>'racha');
            $row[] = array('data' => drupal_render($form[$element]['Actividad']), 'class'=>'actividad');
            $row[] = array('data' => drupal_render($form[$element]['Puntuacion']), 'class'=>'puntuacion');
            $rows[] = $row;
        }
    }
    
    $output .= theme('table', $header, $rows);
    
    
    return $output . drupal_render($form);
}