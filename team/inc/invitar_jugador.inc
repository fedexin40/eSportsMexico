<?php

/**
 * Archivo para presentar el formulario de invitaci�n a un jugador o agente libre
 */
/**
 * Funci�n para crear la forma de invitaci�n
 */
function invitar_usuario() {
    global $user;
    $usuario_invitado = user_load(arg(1));  //El primer argumento en la ruta define el uid del jugador a invitar
    $equipo = node_load(arg(3));            //El tercer argumento en la ruta define el nid del equipo al que se le va a invitar.
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
            '#value' => t('Invitar'),
        );
        return $form;
    }
    else {
        dsm('No eres el capit�n de este equipo equipo y no puedes invitar a nadie.', 'error');
        
    }
}
/**
 * Funci�n para crear la forma de invitaci�n
 */
function invitar_usuario_validate($form, $form_state) {
    $equipo = node_load($form_state['values']['nid_Equipo']);

    if (is_null($form_state['values']['id_Jugador'])) {
        form_set_error('invitado', 'El jugador no est� registrado en esta modalidad.');
    }
    else {
        $jugador = get_jugador($form_state['values']['uid_invitado'], $equipo->id_Modalidad_Juegos_Temporadas);
        if ($jugador->Estado == TIENE_EQUIPO) {
            form_set_error('invitado', 'No puedes invitar a esta jugador porque ya pertenece a un equipo.');
        }
        else {
            db_set_active('eSM');
                $relaciones = db_query('SELECT * FROM {Integrantes_Equipo} WHERE id_Jugador = %d AND nid_Equipo = %d', $jugador->id_Jugador, $equipo->nid);
            db_set_active('default');
            while ($relacion = db_fetch_array($relaciones)) {
                switch ($relacion['Estado']) {
                    case 1:
                        form_set_error('invitado', t('Ya has invitado a este jugador. Espera a que acepte tu invitaci�n.'));
                        break;
                    case 2:
                        form_set_error('invitado', 'Este jugador ya ha solicitado ser parte del equipo.');
                        break;
                    case 3:
                        form_set_error('invitado', 'Este jugador ya forma parte de tu equipo.');
                        break;
                    default :
                        break;
                }
            }
        }
    }
}
/**
 * Funci�n para crear la forma de invitaci�n
 */
function invitar_usuario_submit ($form, $form_state) {
    $equipo = $form_state['values']['nid_Equipo'];
    $id_Jugador = $form_state['values']['id_Jugador'];
    
    db_set_active('eSM');
        db_query('INSERT INTO {Integrantes_Equipo} (nid_Equipo, id_Jugador, Estado, Mensaje) VALUES (%d, %d, %d, "%s")', $equipo, $id_Jugador, INVITADO, $form_state['values']['Mensaje']);
    db_set_active('default');
    
    dsm(t('Tu invitacion se ha enviado correctamente.'));
    $modalidad = get_modalidad($equipo->id_Modalidad_Juego_Temporada);
    drupal_goto('/libres/'. $modalidad->tid .'/lista');
}