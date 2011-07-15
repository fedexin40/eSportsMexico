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
    $equipo = node_load(arg(3));            //El tercer argumento en la ruta define el nid del equipo al que se le va a invitar.
    
    if ($user->uid == $equipo->uid) {
        $form['nid_Equipo'] = array (
            '#type' => 'hidden',
            '#value' => $equipo->nid,
        );
        $form['invitado'] = array (
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
        dsm('No eres el capitán de este equipo equipo y no puedes invitar a nadie.', 'error');
        
    }
}
/**
 * Función para crear la forma de invitación
 */
function invitar_usuario_validate($form, $form_state) {
    $equipo = node_load($form_state['values']['nid_Equipo']);
    $jugador = get_jugador($form_state['values']['invitado'], $equipo->id_Modalidad_Juego_Temporada);
    
    if (is_null($jugador)) {
        form_set_error('invitado', 'El jugador no está registrado en esta modalidad.');
    }
    else {
        if ($jugador->Estado == TIENE_EQUIPO)
            form_set_error('invitado', 'El jugador ya pertenece a otro equipo.');
    }
}
/**
 * Función para crear la forma de invitación
 */
function invitar_usuario_submit ($form, $form_state) {
    $equipo = $form_state['values']['nid_Equipo'];
    $jugador = get_jugador($form_state['values']['invitado'], $equipo->id_Modalidad_Juego_Temporada);
    
    db_set_active('eSM');
        db_query('INSERT INTO {Integrantes_Equipo} (nid_Equipo, id_Jugador, Estado, Mensaje) VALUES (%d, %d, %d, "%s")', $equipo, $jugador->id_Jugador, INVITADO, $form_state['values']['Mensaje']);
    db_set_active('default');
    
    dsm('Tu invitación se ha enviado correctamente.');
}