<?php

/**
* Forma para adquirir características de agente libre en la temporada
*/
function agente_libre() {
  $tid = arg( 2 );  //Obtiene el tid de la modalidad de la barra de direcciones
  global $user;
  
  if (!($modalidad = term_is_active($tid)))
    return t('Esta modalidad no esta activa en esta temporada.');
  
  if($modalidad->identificador == '')
    return t('El identificador de este juego no esta configurado.</br>Por favor contacta a un administrador.');
  
  $profile_node = content_profile_load('profile', $user->uid);
  if ($profile_node->{$modalidad->identificador}['0']['value'] == '')
    return t('Para registrarte primero debes dar de alta tu identificador de juego en tu perfil de usuario.</br>');
  
  $tiene_equipo = tiene_equipo( $user->uid, $modalidad->id_Modalidad_Juego_Temporada );
  if ($tiene_equipo == TIENE_EQUIPO)
    return t('Ya perteneces a un equipo de esta modalidad. Para buscar otro equipo primero debes abandonar el equipo al que perteneces actualmente.');
  
  if ($tiene_equipo == AGENTE_LIBRE)
    return t('Ya estas registrado como Agente Libre en esta modalidad.');
      
    //Trata de obtener la información del usuario
    db_set_active('eSM');
        $jugador = db_fetch_object(db_query('SELECT * FROM {Agente_Libre} AS al
                                            INNER JOIN {Jugador} AS j ON al.id_Jugador = j.id_Jugador
                                            WHERE j.uid = $d',
                                            $user->uid));
    db_set_active('default');
    
    $form['id_Modalidad_Juego_Temporada'] = array (
        '#type' => 'hidden',
        '#value' => $modalidad->id_Modalidad_Juego_Temporada,
    );
    $form['Instrucciones'] = array (
        '#value' => t('<p>Marca en la parte derecha los dias y horas que mejor se adaptan a tu disponibilidad habitual para jugar en eSM.</p><p>Ten en cuenta que los equipos buscan jugadores que jueguen a la misma hora que ellos, asi que asegurate de que los datos sobre tu tiempo libre sean fiables</p>'),
    );
    
    $form[ 'presentacion' ] = array (
        '#type' => 'textarea',
        '#title' =>  t('Presentacion') ,
        '#required' => TRUE,
        '#description' => 'Presentacion breve: estilo de juego, motivaciones, etc.',
        'default_value' => $jugador->descripcion,
    );
     
    $form[ 'disponibilidad' ] = array (
        '#title' => 'Disponibilidad',
        '#type' => 'fieldset',
    );
    
    $form[ 'disponibilidad' ][ 'dias' ] = array (
        '#type' => 'checkboxes', 
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
        '#type' => 'checkboxes', 
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
      
    return drupal_render_form($form);
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
    
    foreach ($form_state['values']['dias'] as $key => $element) {
        if (!$element == 0)
            $dias[$key] = 1;
        else
            $dias[$key] = 0;
    }
    
    foreach ($form_state['values']['horas'] as $key => $element) {
        if (!$element == 0)
            $horas[$key] = 1;
        else
            $horas[$key] = 0;
    }
    
    //0.- Verifica si existe o no el registro del jugador en la tabla
    db_set_active('eSM');
        $result = db_result(db_query('SELECT COUNT FROM {Jugador} WHERE uid = %d', $user->uid));
    db_set_active('default');
    
    if($result > 0)
        $op = 'update';
    else
        $op = 'insert';
    
    //1.- Insertar un registro en la tabla de Jugador para reflejar que el usuario se inscribió a la modalidad como agente libre
    db_set_active( 'eSM' );
        if ($op == 'insert') {
            db_query("INSERT INTO { Jugador }
                    (id_Modalidad_Juego_Temporada, uid, Fecha_Registro, Estado)
                    VALUES ( %d, %d, %d, %d )",
                    $id_Modalidad_Juego_Temporada, $user->uid, string_to_timestamp( date( 'Y-m-d' ) ), AGENTE_LIBRE);
        }
        if ($op == 'create') {
            db_query("UPDATE {Jugador}
                    SET Estado = %d
                    WHERE id_Modalidad_Juego_Temporada = %d AND uid = %d",
                    AGENTE_LIBRE, $id_Modalidad_Juego_Temporada, $user->uid);
        }
    db_set_active( 'default' );
    
    $jugador = get_jugador($user->uid, $id_Modalidad_Juego_Temporada);
    
    if (db_result(db_query('SELECT COUNT FROM {Agente_Libre} WHERE id_Jugador = %d', $jugador->id_Jugador)) > 0) {
        db_set_active('eSM');
            //2.- Actualiza la descripción ingresada por el usuario
            db_query("UPDATE {Agente_Libre} SET Descripcion = '%s'
                     WHERE id_Jugador = %d",
                    $form_state['values']['presentacion'], $jugador->id_Jugador);
            //3.- Acutaliza la disponibilidad de días en la tabla;
            db_query("UPDATE {Disponibilidad_Dias}
                     SET Lunes = %d, Martes = %d, Miercoles = %d, Jueves = %d, Viernes = %d, Sabado = %d, Domingo= %d
                     WHERE id_Jugador = %d",
                     $dias['lunes'], $dias['martes'], $dias['miercoles'], $dias['jueves'], $dias['viernes'], $dias['sabado'], $dias['domingo'], $jugador->id_Jugador);
            //4.- Actualiza la disponibildad de horas en la tabla;
            db_query("UPDATA {Disponibilidad_Horas}
                     SET  Morning = %d, Evening = %d, Night = %d
                     WHERE id_Jugador = %d",
                     $horas['morning'], $horas['evening'], $horas['nigth'], $jugador->id_Jugador);
        db_set_active( 'default' );
    }
    else {
        db_set_active('eSM');
            //2.- Insertar un registro en la tabla de Agente_Libre relacionando el id_Agente_Libre con el id_Jugador y la descripción ingresada por el usuario
            db_query("INSERT INTO { Agente_Libre } (id_Jugador, Descripcion)
                     VALUES ( %d, '%s' )",
                    $jugador->id_Jugador, $form_state['values']['presentacion']);
            //3.- Insertar la disponibilidad de días en la tabla;
            db_query("INSERT INTO {Disponibilidad_Dias}
                     (id_Jugador, Lunes, Martes, Miercoles, Jueves, Viernes, Sabado, Domingo)
                     VALUES (%d, %d, %d, %d, %d, %d, %d, %d)",
                     $jugador->id_Jugador, $dias['lunes'], $dias['martes'], $dias['miercoles'], $dias['jueves'], $dias['viernes'], $dias['sabado'], $dias['domingo']);
            //4.- Insertar la disponibildad de horas en la tabla;
            db_query("INSERT INTO {Disponibilidad_Horas}
                     (id_Jugador, Morning, Evening, Night)
                     VALUES (%d, %d, %d, %d)",
                     $jugador->id_Jugador, $horas['morning'], $horas['evening'], $horas['nigth']);
        db_set_active( 'default' );
    }
    drupal_set_message( "Te has agregado correctamente como Agente Libre." );
} 
/**
*	Función para listar a los agentes libres
*/
function agente_libre_listar ( ) {
    module_load_include( 'php', 'base_datos_externa', 'db_functions' );
    
    global $user;
    $tid = arg(1);     //obtiene el tid de la barra de direcciones

    if ($modalidad = term_is_active($tid)) {
        $count = 0;
        $is_capitan = is_capitan($user->uid, $modalidad->id_Modalidad_Juego_Temporada);
    
        //Obtiene la lista de jugadores libres para una modalidad y temporada
        db_set_active( 'eSM' );
            $result = db_query( "SELECT { id_Jugador }, { uid } FROM { Jugador } WHERE id_Modalidad_Juego_Temporada = %d AND Estado = %d", $modalidad->id_Modalidad_Juego_Temporada, AGENTE_LIBRE );
        db_set_active( 'default' );
        
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
                        '#value' => substr($dia, 0, 2)
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
                    '#value' => l('Invitar', 'user/'. $jugador->uid . '/invitar/'. $is_capitan->nid),
                );
            }
        }
        
        if ($count == 0 ) {
            $form['Jugadores_Libres']['Lista_Vacia'] = array (
                '#value' => 'No hay jugadores libres registrados actualmente.',
            );
        }
    }
    else {
        $form['Jugadores_Libres']['Temporada_Inactiva'] = array (
            '#value' => t('Esta modalidad no esta activa en esta temporada.'),
        );
    }
    
    $form['#theme'] = 'lista_agentes_libres';
    
    return $form;
}
