<?php

/**
 * Archivo para presentar el formulario de invitación a un jugador o agente libre
 */
/**
 * Función para crear la forma de invitación
 */
function invitar_usuario() {
    global $user;
    $usuario_invitado = user_load(arg(1));  //El primer argumento en la ruta define el uid del jugador a invitar
    $equipo = node_load(array('nid' => arg(3)));            //El tercer argumento en la ruta define el nid del equipo al que se le va a invitar.
    $jugador = get_jugador($usuario_invitado->uid, $equipo->id_Modalidad_Juego_Temporada);
    
    if ($user->uid == $equipo->uid) {
        $form['nid_Equipo'] = array (
            '#type' => 'hidden',
            '#value' => $equipo->nid,
        );
        $form['id_Jugador'] = array (
            '#type' => 'hidden',
            '#value' => $jugador->id_Jugador,
        );
        $form['uid_invitado'] = array (
            '#type' => 'hidden',
            '#value' => $usuario_invitado->uid,
        );
        $form['Mensaje'] = array (
            '#type' => 'textarea',
            '#title' => t('Escribe el texto de tu invitacion para '. $usuario_invitado->name),
        );
        $form['Submit'] = array (
            '#type' => 'submit',
            '#value' => t( 'Invitar' ),
        );
    }
    else {
        $form['mensaje'] = array (
            '#value' => t('Solo los capitanes pueden invitar jugadores.'),
        );
    }
    return $form;
}
/**
 * Función para crear la forma de invitación
 */
function invitar_usuario_validate($form, $form_state) {
    $equipo = node_load(array('nid' => $form_state['values']['nid_Equipo']));

    if (is_null($form_state['values']['id_Jugador'])) {
        form_set_error('invitado', 'El jugador no está registrado en esta modalidad.');
    }
    else {
      $jugador = get_jugador($form_state['values']['uid_invitado'], $equipo->id_Modalidad_Juego_Temporada);
      if ($jugador->Estado == TIENE_EQUIPO) {
          form_set_error('invitado', 'No puedes invitar a este jugador porque ya pertenece a un equipo.');
      }
      else {
          db_set_active('eSM');
              $relaciones = db_query('SELECT * FROM {Integrantes_Equipo}
                                     WHERE id_Jugador = %d AND nid_Equipo = %d',
                                     $jugador->id_Jugador, $equipo->nid);
          db_set_active('default');
          while ($relacion = db_fetch_array($relaciones)) {
              switch ($relacion['Estado']) {
                  case 1:
                      form_set_error('invitado', t('Ya has invitado a este jugador. Espera a que acepte tu invitacion.'));
                      break;
                  case 2:
                      form_set_error('invitado', 'Este jugador ya ha solicitado ser parte del equipo.');
                      break;
                  case 3:
                      form_set_error('invitado', 'Este jugador ya forma parte de tu equipo.');
                      break;
              }
          }
      }
    }
    //Verifica que el equipo tenga espacio suficiente para invitar un jugador
    db_set_active('eSM');
      $num_players = db_result(db_query('SELECT COUNT {*} FROM {Integrantes_Equipo}
                                        WHERE nid_Equipo = %d AND Estado = %d',
                                        $nid_Equipo, ACEPTADO));
    db_set_active('default');
    $modalidad = get_modalidad($equipo->id_Modalidad_Juego_Temporada);
    if ($num_players == $modalidad->Maximo_Jugadores)
        form_set_error('invitado', t('No puedes invitar a este jugador porque tu equipo ya alcanzó el máximo de jugadores permitidos en esta modalidad.'));
}
/**
 * Función para crear la forma de invitación
 */
function invitar_usuario_submit ($form, $form_state) {
    global $user;    
    $nid_Equipo = $form_state['values']['nid_Equipo'];
    $equipo = node_load(array('nid' => $nid_Equipo));
    
    $id_Jugador = $form_state['values']['id_Jugador'];
    $uid_Invitado = $form_state['values']['uid_invitado'];
    
    db_set_active('eSM');
        db_query('INSERT INTO {Integrantes_Equipo}
                 (nid_Equipo, id_Jugador, Estado, Mensaje)
                 VALUES (%d, %d, %d, "%s")',
                 $equipo->nid, $id_Jugador, INVITADO, $form_state['values']['Mensaje']);
    db_set_active('default');
    
    //Notifica al usuario que ha sido invitado a un equipo
    rules_invoke_event('usuario_invitado', $user, user_load(array('uid' => $uid_Invitado)), $equipo);
    
    drupal_set_message(t('Tu invitacion se ha enviado correctamente.'));
}