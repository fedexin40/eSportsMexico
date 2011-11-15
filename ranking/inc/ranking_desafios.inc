<?php

function ranking_desafios() {
    $superior = db_query('SELECT * FROM {eSM_Puntos_Desafio_Jugador_Superior}');
    $inferior = db_query('SELECT * FROM {eSM_Puntos_Desafio_Jugador_Inferior}');
    
    for ($i = 0; $i < 11; $i++){
        $puntuacion_superior = db_fetch_array($superior);
        $puntuacion_inferior = db_fetch_array($inferior);
        
        $form['Diferencia_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion_superior['Diferencia_Puntos'],
            '#required' => TRUE,
        ); 
        $form['Equipo_Superior_Win_'. $i] = array (
          '#type' => 'textfield',
          '#maxlength' => 3,
          '#size' => 4,
          '#default_value' => $puntuacion_superior['Puntos_Win'],
          '#required' => TRUE,
        );
        $form['Equipo_Superior_Loss_'. $i] = array (
          '#type' => 'textfield',
          '#maxlength' => 3,
          '#size' => 4,
          '#default_value' => $puntuacion_superior['Puntos_Loss'],
          '#required' => TRUE,
        );
        $form['Equipo_Inferior_Win_'. $i] = array (
          '#type' => 'textfield',
          '#maxlength' => 3,
          '#size' => 4,
          '#default_value' => $puntuacion_inferior['Puntos_Win'],
          '#required' => TRUE,
        );
        $form['Equipo_Inferior_Loss_'. $i] = array (
          '#type' => 'textfield',
          '#maxlength' => 3,
          '#size' => 4,
          '#default_value' => $puntuacion_inferior['Puntos_Loss'],
          '#required' => TRUE,
        );
    }
    
    $form['submit'] = array (
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    
    $form['#theme'] = 'ranking_desafios';
    
    return $form;
}
/**
 * Validate para el ranking_desafios
 */
function ranking_desafios_validate ($form, $form_state) {
    
}
/**
 * Submit para el ranking_desafios
 */
function ranking_desafios_submit ($form, $form_state) {
    //Borra los datos ingresados en la tabla para que no existan confusiones
    //dsm($form_state);
    
    db_query('TRUNCATE TABLE {eSM_Puntos_Desafio_Jugador_Superior}');
    db_query('TRUNCATE TABLE {eSM_Puntos_Desafio_Jugador_Inferior}');
        
    for ($i = 0; $i < 11; $i ++) {
        db_query('INSERT INTO {eSM_Puntos_Desafio_Jugador_Superior} (Diferencia_Puntos, Puntos_Win, Puntos_Loss) VALUES (%d, %d, %d)', $form_state['values']['Diferencia_'. $i], $form_state['values']['Equipo_Superior_Win_'. $i], $form_state['values']['Equipo_Superior_Loss_'. $i]);
        db_query('INSERT INTO {eSM_Puntos_Desafio_Jugador_Inferior} (Diferencia_Puntos, Puntos_Win, Puntos_Loss) VALUES (%d, %d, %d)', $form_state['values']['Diferencia_'. $i], $form_state['values']['Equipo_Inferior_Win_'. $i], $form_state['values']['Equipo_Inferior_Loss_'. $i]);
    }
    drupal_set_message('Las puntuaciones se han guardado correctamente.');
}