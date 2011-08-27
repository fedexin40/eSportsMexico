<?php

/**
 * Formulario de vista para los equipos de un usuario
 */
function equipos_usuario() {
    module_load_include( 'php', 'base_datos_externa', 'db_functions' );
    
    $usuario['uid'] = arg(1);
    $usuario = user_load($usuario);
    
    $modalidades_activas = get_modalidades_juego_temporada_activas();   //Obtiene todas las modalidades activas en una temporada
    $count = 0;
    
    foreach($modalidades_activas as $indice => $modalidad) {
        db_set_active('eSM');       //Obtiene los equipos inscritos en la modalidad actual
            $result = db_query('SELECT {nid_Equipo} FROM {Equipo}
                               WHERE id_Modalidad_Juego_Temporada = %d AND Estado = %d',
                               $modalidad->id_Modalidad_Juego_Temporada, ACTIVO);
        db_set_active('default');
        
        while ($equipo = db_fetch_object($result)) {
            $equipo = node_load($equipo->nid_Equipo);       //Carga el nodo equipo para obtener los jugadores inscritos en el equipo.
            foreach($equipo->Integrantes as $indice => $jugador) {
                if ($jugador->uid == $usuario->uid) {          //Revisa cada uno de los integrantes para saber si el jugador pertenece
                    $form[$equipo->nid][$modalidad->id_Modalidad_Juego_Temporada] = array (
                        '#value' =>   l($modalidad->Nombre, 'taxonomy/term/'. $modalidad->tid),
                    );
                    $form[$equipo->nid][$jugador->uid] = array (
                        '#value' =>   l($equipo->title, 'node/'. $equipo->nid),
                    );
                }
            }
            ++$count;
        }
    }
    
    if ($count == 0) {
        $form['mensaje'] = array (
            '#value' => t('No perteneces todavia a ningun equipo activo en esta temporada.'),
        );
    }
    
    return $form;
}