<?php

function ranking_divisiones() {
    $divisiones = db_query('SELECT * FROM {eSM_Puntuacion_Divisiones}');
    
    for ($i = 1; $i <= 5; $i++) {
        $division = db_fetch_array($divisiones);
            
        $form['Division_'. $i] = array (
            '#value' => t('Division '. $i),
        );
        $form['Nombre_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 20,
            '#size' => 20,
            '#default_value' => $division['Nombre'],
            '#required' => TRUE,
        );
        $form['Jugadores_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $division['Jugadores'],
            '#required' => TRUE,
        );
    }
    
    $form['submit'] = array (
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    
    $form['#theme'] = 'ranking_divisiones';
    
    return $form;
}
/**
 * Validate para el ranking_desafios
 */
function ranking_torneos_validate ($form, $form_state) {
    
}
/**
 * Submit para el ranking_desafios
 */
function ranking_divisiones_submit ($form, $form_state) {
    //Borra los datos ingresados en la tabla para que no existan confusiones
    //dsm($form_state);
    
    db_query('TRUNCATE TABLE {eSM_Puntuacion_Divisiones}');
    
    for ($i = 1; $i <= 5; $i ++) {
        db_query('INSERT INTO {eSM_Puntuacion_Divisiones}
                (Division, Nombre, Jugadores)
                VALUES (%d, "%s", %d)',
                $i,
                $form_state['values']['Nombre_'. $i],
                $form_state['values']['Jugadores_'. $i]);
    }
    
    drupal_set_message('Las divisiones se han guardado correctamente.');
}