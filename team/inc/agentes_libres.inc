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
        '#title' => check_plain( t('Presentación') ),
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
    global $user;
    $id_Modalidad_Juego_Temporada = $form_state['values']['id_Modalidad_Juego_Temporada'];
    
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
}
function agente_libre_validate ( $form, $form_state ) {
    global $user;
    $id_Modalidad_Juego_Temporada = $form_state['values']['id_Modalidad_Juego_Temporada'];
    $tiene_equipo = tiene_equipo($user->uid, $modalidad->id_Modalidad_Juego_Temporada);
    
    if ( $tiene_equipo == TIENE_EQUIPO)
        form_set_error('id_Modalidad_Juego_Temporada', '<p>No puedes declararte como agente libre porque ya perteneces a un equipo.</p><p>Abandona tu equipo actual para poder declararte como agente libre.</p>');
        
    if ($tiene_equipo == AGENTE_LIBRE)
        form_set_error('id_Modalidad_Juego_Temporada', '<p>Ya te has declarado como agente libre.</p>');
}
       
/**
*	Función para listar a los agentes libres
*/
function agente_libre_listar ( ) {
       
       $disponibilidad = array ( );
       
       
       db_set_active( 'eSM' );
       
       $jugador_consulta = db_query( "SELECT { id_Jugador } FROM { Jugador } WHERE Estado = %d", AGENTE_LIBRE );
       
       db_set_active( 'default' );
       
       /*while ( $jugador_result = db_fetch_array( $jugador_consulta ) )
       {
               $jugador_atributos[ $jugador_result->id_Jugador ] = $jugador_result->id_Jugador;
       }*/
       
       
       db_set_active( 'eSM' );
       $agente_libre_consulta = db_query( "SELECT { uid, Fecha_Registro, Estado, Descripcion, Fecha_Inicio, Fecha_Fin } FROM { Jugador, Agente_Libre, Disponibilidad } WHERE {Estado} = %d AND {Agente_Libre.id_Jugador} = {Jugador.id_Jugador } AND {Agente_Libre.id_Agente_Libre} ={ Disponibilidad.id_Agente_Libre};", AGENTE_LIBRE );
       db_set_active( 'default' );


       $form[ 'agente_libre_inscripcion' ] = array (
               '#title' => 'Agentes libres',
               '#description' => 'Bienvenido, introduce tus datos',
               '#type' => 'fieldset',
               '#collapsed' => 0,
               '#colapsible' => 0,
       );

       
       while ( $agente_libre_consulta = db_fetch_array( $agente_libre_consulta ) )
       {
               $agentes_libres_array[ $agente_libre_consulta->uid ] = $agente_libre_consulta->uid;
       
               
               $fecha_inicio = timestamp_to_string( $agente_libre_consulta->Fecha_Inicio );
               $fecha_fin = timestamp_to_string( $agente_libre_consulta->Fecha_Fin );
               

               $form[ 'agente_libre_inscripcion' ][ 'presentacion' ] = array (
                       '#type' => 'textarea',
                       '#title' => check_plain( t('Presentación') ),
                       '#default_value' => $agente_libre_consulta->presentacion,
                       '#required' => TRUE,
                       '#description' => 'Presentación breve: estilo de juego, motivaciones, etc.',
               );

               $form[ 'agente_libre_inscripcion' ][ 'disponibilidad' ] = array (
                       '#title' => 'Disponibilidad',
                       '#type' => 'fieldset',
                       '#collapsed' => 0,
                       '#collapsible' => 0,
               );

               
               
               $form[ 'agente_libre_inscripcion' ][ 'disponibilidad' ][ 'inicio' ] = array (
                       '#type' => 'date_text', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
                       '#disabled' => 'TRUE',
                       '#title' => 'Fecha de inicio',
                       '#default_value' => $fecha_inicio,
                       '#date_format' => $format,
                       '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
                       '#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
                       '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
                       '#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
                       '#size' => 17,
                       '#maxlength' => 17,
               );
               
               
               $form[ 'agente_libre_inscripcion' ][ 'disponibilidad' ][ 'fin' ] = array (
                       '#type' => 'date_text', // types 'date_text' and 'date_timezone' are also supported. See .inc file.
                       '#disabled' => 'TRUE',
                       '#title' => 'Fecha de Fin',
                       '#default_value' => $fecha_fin,
                       '#date_format' => $format,
                       '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
                       '#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
                       '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
                       '#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
                       '#size' => 17,
                       '#maxlength' => 17,
               );
               
               $form[ 'submit' ] = array (
                       '#type' => 'submit',
                       '#value' => l( t( 'team/node/'.$user->id.'edit') ),
               );

       }
       
       return $form;
}
