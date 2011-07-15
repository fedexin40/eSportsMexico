<?php

/**
* Forma para adquirir características de agente libre en la temporada
*/
function agente_libre() {
    $id_Modalidad_Juego_Temporada = arg(1); //Obtiene el id_Modalidad_Juego_Temporada pasado a la función en la barra de dirección
    
    $form['id_Modalidad_Juego_Temporada'] = array (
        '#type' => 'hidden',
        '#value' => $id_Modalidad_Juego_Temporada,
    );
    $form['Instrucciones'] = array (
        '#value' => t('<p>Marca en la parte derecha los dias y horas que mejor se adaptan a tu disponibilidad habitual para jugar en eSM.</p><p>Ten en cuenta que los equipos buscan jugadores que jueguen a la misma hora que ellos, asi que asegurate de que los datos sobre tu tiempo libre sean fiables</p>'),
    );
    
    $form[ 'presentacion' ] = array (
        '#type' => 'textarea',
        '#title' =>  t('Presentacion') ,
        '#required' => TRUE,
        '#description' => 'Presentacion breve: estilo de juego, motivaciones, etc.',
    );
     
    $form[ 'disponibilidad' ] = array (
        '#title' => 'Disponibilidad',
        '#type' => 'fieldset',
    );

    $form[ 'disponibilidad' ][ 'dias' ] = array (
        '#type' => 'checkboxes', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#title' => t('Dias  Disponibles'),
        '#required' => TRUE,
        '#options' => array(
            'lunes' => t('Monday'), 
            'martes' => t('Tuesday'), 
            'miercoles' => t('Wendesday'),
            'jueves' => t('Thursday'),
            'viernes' => t('Friday'),
            'sabado' => t('Saturday'),
            'domingo' => t('Sunday'),
        ),
     );

    $form[ 'disponibilidad' ][ 'horas' ] = array (
        '#type' => 'checkboxes', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
        '#title' => t('Horas Disponibles'),
        '#required' => TRUE,
        '#options' => array(
            'morning' => t('Morning'), 
            'evening' => t('Tardes'), 
            'nigth' => t('Noches'),
        ),
     );
    
    $form['submit'] = array (
        '#type' => 'submit',
        '#value' => t('Declararse Agente Libre'),
    );
      
    return $form;
}

/**
*	Implementación del hook_submit para la inscripción de
*	agentes libres
*	Insertar agentes libres es un proceso de 4 pasos
*	1.- Insertar un registro en la tabla de Jugador para reflejar que el usuario se inscribió a la modalidad como agente libre
*	2.- Insertar un registro en la tabla de Agente_Libre relacionando el id_Agente_Libre con el id_Jugador y la descripción ingresada por el usuario
*	3.- Insertar la disponibilidad de días en la tabla;
*	4.- Insertar la disponibildad de horas en la tabla;
*/
function agente_libre_submit ( $form, $form_state ) {
    drupal_set_message( "Te has agregado correctamente como Agente Libre." );
    global $user;
    $id_Modalidad_Juego_Temporada = $form_state['values']['id_Modalidad_Juego_Temporada'];
    drupal_set_message( "Te has agregado correctamente como Agente Libre." );
    
    //1.- Insertar un registro en la tabla de Jugador para reflejar que el usuario se inscribió a la modalidad como agente libre
    db_set_active( 'eSM' );
        db_query
        ( 
            "INSERT INTO { Jugador }
            ( 
                id_Modalidad_Juego_Temporada,
                uid,
                Fecha_Registro,
                Estado
            )			
            VALUES ( %d, %d, %d, %d )",
                $id_Modalidad_Juego_Temporada, $user->uid, string_to_timestamp( date( 'Y-m-d' ) ), AGENTE_LIBRE
        );
    db_set_active( 'default' );
    dsm('paso 1');
    
    $jugador = get_jugador($user->uid, $id_Modalidad_Juego_Temporada);
    dsm($jugador);
    
    //2.- Insertar un registro en la tabla de Agente_Libre relacionando el id_Agente_Libre con el id_Jugador y la descripción ingresada por el usuario
    db_set_active('eSM');
        db_query
        (
            "INSERT INTO { Agente_Libre }
            (
                id_Jugador,
                Descripcion
            )
            VALUES ( %d, '%s' )",
                $jugador->id_Jugador, $form_state['values']['presentacion']
        );
    db_set_active( 'default' );
    drupal_set_message('paso 2');
    
    //3.- Insertar la disponibilidad de días en la tabla;
    foreach ($form_state['values']['dias'] as $key => $element) {
        if (!$element == 0)
            $dias[$key] = 1;
        else
            $dias[$key] = 0;
    }
    db_set_active('eSM');
        db_query
        (
            "INSERT INTO {Disponibilidad_Dias}
            (
                id_Jugador,
                Lunes,
                Martes,
                Miercoles,
                Jueves,
                Viernes,
                Sabado,
                Domingo
            )
            VALUES
            (
                %d, %d, %d, %d, %d, %d, %d, %d
            )",
            $jugador->id_Jugador, $dias['lunes'], $dias['martes'], $dias['miercoles'], $dias['jueves'], $dias['viernes'], $dias['sabado'], $dias['domingo']
        );
    db_set_active('default');
    drupal_set_message('paso 3');
        
    //4.- Insertar la disponibildad de horas en la tabla;
    foreach ($form_state['values']['horas'] as $key => $element) {
        if (!$element == 0)
            $dias[$key] = 1;
        else
            $dias[$key] = 0;
    }
    db_set_active('eSM');
        db_query
        (
            "INSERT INTO {Disponibilidad_Horas}
            (
                id_Jugador,
                Morning,
                Evening,
                Night
            )
            VALUES
            (
                %d, %d, %d, %d
            )",
            $jugador->id_Jugador, $dias['morning'], $dias['evening'], $dias['nigth']
        );
    drupal_set_message( "Te has agregado correctamente como Agente Libre." );
    drupal_goto('user/'. $user->uid);
}
function agente_libre_validate ( $form, $form_state ) {
    global $user;
    $id_Modalidad_Juego_Temporada = $form_state['values']['id_Modalidad_Juego_Temporada'];
    $tiene_equipo = tiene_equipo($user->uid, $id_Modalidad_Juego_Temporada);
    
    if ( $tiene_equipo == TIENE_EQUIPO)
        form_set_error('id_Modalidad_Juego_Temporada', '<p>No puedes declararte como agente libre porque ya perteneces a un equipo.</p><p>Abandona tu equipo actual para poder declararte como agente libre.</p>');
        
    if ($tiene_equipo == AGENTE_LIBRE)
        form_set_error('id_Modalidad_Juego_Temporada', '<p>Ya te has declarado como agente libre.</p>');
}
       
/**
*	Función para listar a los agentes libres
*/
function agente_libre_listar ( ) {
    global $user;
    $id_Modalidad_Juego_Temporada = arg(1);     //obtiene el id_Modalidad_Juego_Temporada de la barra de direcciones
    $count = 0;
    $is_capitan = is_capitan($user->uid, $id_Modalidad_Juego_Temporada);
    
    //Obtiene la lista de jugadores libres para una modalidad y temporada
    db_set_active( 'eSM' );
        $result = db_query( "SELECT { id_Jugador }, { uid } FROM { Jugador } WHERE id_Modalidad_Juego_Temporada = %d AND Estado = %d", $id_Modalidad_Juego_Temporada, AGENTE_LIBRE );
    db_set_active( 'default' );
    
    $form['Jugadores_Libres'] = array (
        '#type' => 'fieldset',
        '#collapsible' => 'FALSE',
    );
    //TODO: Elaborar función theme para darle formato a esta tabla de jugadores libres.
    while ( $jugador = db_fetch_array( $result ) ) {
        ++ $count;
        //Obtiene los datos del jugador
        db_set_active('eSM');
            $descripcion = db_fetch_array(db_query('SELECT {Descripcion} FROM {Agente_Libre} WHERE id_Jugador = %d', $jugador['id_Jugador']));
            $disponibilidad_dias = db_fetch_array(db_query('SELECT * FROM {Disponibilidad_Dias} WHERE id_Jugador = %d', $jugador['id_Jugador']));
            $disponibilidad_horas = db_fetch_array(db_query('SELECT * FROM {Disponibilidad_Horas} WHERE id_Jugador = %d', $jugador['id_Jugador']));
        db_set_active('default');
        
        //TODO: Revisar la conveniencia de añadir al objeto user los datos del jugador para que sea más sencillo de manejar y menos consultas que realizar.
        $jugador = user_load($jugador['uid']);
        
        //TODO: Agregar el campo de imágen del jugador a la lista de los agentes libres
        $form['Jugadores_Libres'][$jugador->uid]['Nombre'] = array (
            '#value' => l($jugador->name, 'user/'. $jugador->uid)
        );
        $form['Jugadores_Libres'][$jugador->uid]['Descripcion'] = array (
            '#value' => $descripcion['Descripcion']
        );
        
        foreach ($disponibilidad_dias as $dia => $valor) {
            if ($valor == 1) {
                $form['Jugadores_Libres'][$jugador->uid]['Dias'][$dia] = array (
                    '#value' => $dia
                );
            }
        }
        
        $horas = '';
        foreach ($disponibilidad_horas as $key => $valor) {
            if ($valor == 1)
                $horas .= $key .' and ';
        }
        if ($horas == 'Morning and Evening and Night and ')
            $horas = t('Always');
            
        if (substr($horas,-5) == ' and ')
            $horas = substr($horas, 0, -5);
    
        $form['Jugadores_Libres'][$jugador->uid]['Horas'] = array (
            '#value' => $horas,
        );
        
        if (isset($is_capitan)) {
            $form['Jugadores_Libres'][$jugador->uid]['link'] = array (
                '#value' => l('Invitar', 'user/'. $is_capitan . '/invitar'),
            );
        }
    }
    
    if ($count == 0 ) {
        $form['Jugadores_Libres']['Lista_Vacia'] = array (
            '#value' => 'No hay jugadores libres registrados actualmente.',
        );
    }
    return $form;
}
