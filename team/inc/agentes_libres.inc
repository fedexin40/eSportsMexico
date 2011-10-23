<?php

/**
* Forma para adquirir características de agente libre en la temporada
*/
function agente_libre() {
  $tid = arg( 2 );  //Obtiene el tid de la modalidad de la barra de direcciones
  global $user;
  
  if (!($modalidad = term_is_active($tid))) {
    $form['mensaje'] = array ('#value' => t('Esta modalidad no esta activa en esta temporada.'));
    return $form;
  }
  
  if($modalidad->identificador == '') {
    $form['mensaje'] = array ('#value' => t('El identificador de este juego no esta configurado.</br>Por favor contacta a un administrador.'));
    return $form;
  }
  
  $profile_node = content_profile_load('profile', $user->uid);
  if ($profile_node->{$modalidad->identificador}['0']['value'] == '') {
    $form['mensaje'] = array ('#value' => t('Para registrarte primero debes dar de alta tu identificador de juego en tu perfil de usuario.</br>'));
    return $form;
  }
  
  $tiene_equipo = tiene_equipo( $user->uid, $modalidad->id_Modalidad_Juego_Temporada );
  if ($tiene_equipo == TIENE_EQUIPO) {
    $form['mensaje'] = array ('#value' => t('Ya perteneces a un equipo de esta modalidad. Para buscar otro equipo primero debes abandonar el equipo al que perteneces actualmente.'));
    return $form;
  }
  
  if ($tiene_equipo == AGENTE_LIBRE) {
    $form['mensaje'] = array ('#value' => t('Ya estas registrado como Agente Libre en esta modalidad.'));
    return $form;
  }
      
    //Trata de obtener la información del usuario
    db_set_active('eSM');
        $jugador = db_fetch_object(db_query('SELECT * FROM {Agente_Libre} AS al
                                            INNER JOIN {Jugador} AS j ON al.id_Jugador = j.id_Jugador
                                            WHERE j.uid = %d',
                                            $user->uid));
    db_set_active('default');
    
    $form['id_Modalidad_Juego_Temporada'] = array (
        '#type' => 'hidden',
        '#value' => $modalidad->id_Modalidad_Juego_Temporada,
    );
    $form['Instrucciones'] = array (
        '#value' => t('<p>Marca en la parte derecha los dias y horas que mejor se adaptan a tu disponibilidad habitual para jugar en eSM.</p><p>Ten en cuenta que los equipos buscan jugadores que jueguen a la misma hora que ellos, asi que asegurate de que los datos sobre tu tiempo libre sean fiables</p>'),
        '#weight' => -100,
    );
    
    $form[ 'presentacion' ] = array (
        '#type' => 'textarea',
        '#title' =>  t('Presentacion') ,
        '#required' => TRUE,
        '#description' => 'Presentacion breve: estilo de juego, motivaciones, etc.',
        'default_value' => $jugador->descripcion,
        '#weight' => -50,
    );
     
    $form[ 'disponibilidad' ] = array (
        '#title' => 'Disponibilidad',
        '#type' => 'fieldset',
        '#weight' => 0,
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
        '#weight' => 100,
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
    $jugador = get_jugador($user->uid, $id_Modalidad_Juego_Temporada);
    
    if(is_object($jugador))
        $op = 'update';
    else
        $op = 'insert';
    
    //1.- Insertar un registro en la tabla de Jugador para reflejar que el usuario se inscribió a la modalidad como agente libre
    if ($op == 'insert') {
      db_set_active( 'eSM' );
        db_query("INSERT INTO { Jugador }
                (id_Modalidad_Juego_Temporada, uid, Fecha_Registro, Estado)
                VALUES ( %d, %d, %d, %d )",
                $id_Modalidad_Juego_Temporada, $user->uid, string_to_timestamp( date( 'Y-m-d' ) ), AGENTE_LIBRE);
      db_set_active('default');
      $jugador = get_jugador($user->uid, $id_Modalidad_Juego_Temporada);
    }
    if ($op == 'update') {
      db_set_active('eSM');
        db_query("UPDATE {Jugador}
                SET Estado = %d
                WHERE id_Modalidad_Juego_Temporada = %d AND uid = %d",
                AGENTE_LIBRE, $id_Modalidad_Juego_Temporada, $user->uid);
      db_set_active( 'default' );
    }
    
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
            db_query("UPDATE {Disponibilidad_Horas}
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
    global $user;
    $tid = arg(2);     //obtiene el tid de la barra de direcciones

    if ($modalidad = term_is_active($tid)) {
        drupal_set_title(t('Free Agents for '. $modalidad->Nombre));
        
        $lista = _team_agente_libre_lista($modalidad->id_Modalidad_Juego_Temporada);
        
        if ($lista == '' ) 
            $output = '<div class="form_item">'. t('No hay jugadores libres registrados actualmente.') .'</div>';
        else 
            $output = $lista;
    }
    else {
        $output = '<div class="form_item">'. t('Esta modalidad no esta activa en esta temporada.') .'</div>';
    }
    
    return $output;
}
