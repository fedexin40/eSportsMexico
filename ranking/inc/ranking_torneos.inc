<?php

function ranking_torneos() {
    db_set_active('eSM');
        $puntuaciones = db_query('SELECT * FROM {Puntuacion_Torneos}');
    db_set_active('default');
    
    for ($i = 0; $i < 5; $i++) {
        $puntuacion = db_fetch_array($puntuaciones);
            
        $form['Inscritos_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['Inscritos'],
            '#required' => TRUE,
        );
        $form['1_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['1'],
            '#required' => TRUE,
        );
        $form['2_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['2'],
            '#required' => TRUE,
        );
        $form['4_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['4'],
            '#required' => TRUE,
        );
        $form['8_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['8'],
            '#required' => TRUE,
        );
        $form['16_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['16'],
            '#required' => TRUE,
        );
        $form['32_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['32'],
            '#required' => TRUE,
        );
        $form['64_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['64'],
            '#required' => TRUE,
        );
        $form['128_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['128'],
            '#required' => TRUE,
        );
        $form['256_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['256'],
            '#required' => TRUE,
        );
        $form['512_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['512'],
            '#required' => TRUE,
        );
        $form['1024_'. $i] = array (
            '#type' => 'textfield',
            '#maxlength' => 4,
            '#size' => 4,
            '#default_value' => $puntuacion['1024'],
            '#required' => TRUE,
        ); 
    }
    
    $form['submit'] = array (
        '#type' => 'submit',
        '#value' => t('Save'),
    );
    
    $form['#theme'] = 'ranking_torneos';
    
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
function ranking_torneos_submit ($form, $form_state) {
    //Borra los datos ingresados en la tabla para que no existan confusiones
    //dsm($form_state);
    
    db_set_active('eSM');
        db_query('TRUNCATE TABLE {Puntuacion_Torneos}');
    db_set_active('default');
    
    for ($i = 0; $i < 5; $i ++) {
        db_set_active('eSM');
            db_query('INSERT INTO {Puntuacion_Torneos}
                    (Inscritos, `1`, `2`, `4`, `8`, `16`, `32`, `64`, `128`, `256`, `512`, `1024`)
                    VALUES (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)',
                    $form_state['values']['Inscritos_'. $i],
                    $form_state['values']['1_'. $i],
                    $form_state['values']['2_'. $i],
                    $form_state['values']['4_'. $i],
                    $form_state['values']['8_'. $i],
                    $form_state['values']['16_'. $i],
                    $form_state['values']['32_'. $i],
                    $form_state['values']['64_'. $i],
                    $form_state['values']['128_'. $i],
                    $form_state['values']['256_'. $i],
                    $form_state['values']['512_'. $i],
                    $form_state['values']['1024_'. $i]
            );
        db_set_active('default');
    }
    
    drupal_set_message('Las puntuaciones se han guardado correctamente.');
}