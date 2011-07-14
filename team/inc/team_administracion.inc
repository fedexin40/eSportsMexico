<?php

/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_activos() {
                $equipo = node_load(arg(1));  //carga el objeto nodo o equipo
                
                db_set_active('eSM');  //Carga todos los jugadores activos en el equipo.
                                $jugadores = db_query('SELECT * FROM {Jugador} AS j INNER JOIN {Integrantes_Equipo} AS ie ON j.{nid_Equipo} = ie.{nid_Equipo} WHERE ie.{nid_Equipo} = %d AND ie.{Estado} = %d', $equipo->nid, ACEPTADO);
                db_set_active('default');
                
                //Despliega los jugadores 
                while ($jugador = db_fetch_object($jugadores)) {
                                $account = user_load(array('uid' => $jugador->uid));
                                $form[$jugador->uid]['nombre'] = array (
                                                '#value' => $account->name,
                                );
                                $form[$jugador->uid]['check'] = array (
                                                '#type' => 'checkbox',
                                );
                }
                $form['submit'] = array (
                                '#type' => 'submit',
                                '#value' => t('Expulsar Jugadores Seleccionados.'),
                );
}
/**
 * Submit para la la forma de jugadores activos
 */
function team_administrar_activos_submit($form, $form_state) {
                
}

/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_solicitudes() {
                $equipo = node_load(arg(1));  //carga el objeto nodo o equipo
                
                db_set_active('eSM');  //Carga todos los jugadores que solicitaron entrar en el equipo.
                                $jugadores = db_query('SELECT * FROM {Jugador} AS j INNER JOIN {Integrantes_Equipo} AS ie ON j.{nid_Equipo} = ie.{nid_Equipo} WHERE ie.{nid_Equipo} = %d AND ie.{Estado} = %d', $equipo->nid, SOLICITA);
                db_set_active('default');
                
                //Despliega los jugadores 
                while ($jugador = db_fetch_object($jugadores)) {
                                $account = user_load(array('uid' => $jugador->uid));
                                $form[$jugador->uid]['nombre'] = array (
                                                '#value' => $account->name,
                                );
                                $form[$jugador->uid]['check'] = array (
                                                '#type' => 'checkbox',
                                );
                }
                $form['submit'] = array (
                                '#type' => 'submit',
                                '#value' => t('Aceptar Jugadores Seleccionados.'),
                );
                
}


/**
 * Función para administrar las invitaciones y jugadores de un equipo
 */
function team_administrar_invitados() {
                $equipo = node_load(arg(1));  //carga el objeto nodo o equipo
                
                db_set_active('eSM');  //Carga todos los jugadores invitados en el equipo.
                                $jugadores = db_query('SELECT * FROM {Jugador} AS j INNER JOIN {Integrantes_Equipo} AS ie ON j.{nid_Equipo} = ie.{nid_Equipo} WHERE ie.{nid_Equipo} = %d AND ie.{Estado} = %d', $equipo->nid, INVITADO);
                db_set_active('default');
                
                //Despliega los jugadores 
                while ($jugador = db_fetch_object($jugadores)) {
                                $account = user_load(array('uid' => $jugador->uid));
                                $form[$jugador->uid]['nombre'] = array (
                                                '#value' => $account->name,
                                );
                                $form[$jugador->uid]['check'] = array (
                                                '#type' => 'checkbox',
                                );
                }
}
