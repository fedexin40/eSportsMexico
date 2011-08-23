<?php
/**
 * Archivo para definir y manejar las características y configuraciones de los torneos
 */
/**
 * Page callback for the node/N/signups/settings subtab.
 */
function torneo_admin($node) {
  return drupal_get_form('torneo_admin_form', $node, $node->type, TRUE);
}
/**
 * Función para definir la forma de configuración de los torneos
 */
function torneo_admin_form($form_state, $node = NULL, $node_type = NULL, $include_buttons = FALSE) {
   $settings = get_settings_torneo($node->nid);    //Obtiene las configuraciones del torneo de la tabla si es que el torneo ya existe.
   
   if (module_exists('match')) {
     //Selecciona los partidos que estén en conflicto
     $retos = _match_torneo_get_match_list($node->nid, CONFLICTO);
     
     $form_state['Encabezdo'] = array(
      '#value' => t('A continuacion se presenta una lista de todos los desafios en conflicto de este torneo.<br/>
                    Para ver las pruebas de cada uno haz click en el link "Detalles" que se encuentra a la derecha de cada partido.<br/>
                    Para actualizar el estado de un desafio selecciona el equipo ganador de la lista desplegable y haz click en el boton Guardar Cambios.'),
     );

    $count = 0;
    
    while( $reto = db_fetch_object( $retos ) ) {
        ++$count;
        
        $home_team = node_load($reto->nid_Equipo_Local);
        $away_team = node_load($reto->nid_Equipo_Visitante);
  
        $form[ $count ][ 'Local' ] = array (
            '#type' => 'item',
            '#value' => l($home_team->title, 'node/'. $home_team->nid),
        );
  
        $form[ $count ][ 'Visitante' ] = array (
            '#type' => 'item',
            '#value' => l($away_team->title, 'node/'. $away_team->nid),
        );
        
        $form[ $count ][ 'Fecha' ] = array (
            '#type' => 'item',
            '#value' => format_date($reto->Fecha_Inicio, 'large'),
        );
        
        $form[ $count ][ 'Ronda' ] = array (
            '#type' => 'item',
            '#value' => $reto->Ronda,
        );
        
        $form[ $count ][ 'Partido' ] = array (
            '#type' => 'item',
            '#value' => l('Detalles', 'node/'. $reto->nid_Reto, ),
        );
        
        //Prepara un arreglo con las opciones de los equipos para que los administradores puedan decidir que equipo fue ganador
        $options = array();
        $options['0'] => t('Selecciona ganador');
        $options[$home_team->nid] => $home_team->title;
        $options[$away_team->nid] => $away_team->title;
        
        $form_state[ $count ][ 'Ganador' ] = array (
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => 0,
        );
    }
    
    if ($count == 0) {
        $form['Mensaje'] = array (
            '#value' => t('No se han jugado partidos en este torneo.'),
        );
    }
    else {
      $form['Submit'] = array(
        '#type' => 'submit',
        '#value' =>  t('Guardar cambios'),
      );
    }
    
    $form['#theme'] = 'torneo_lista_desafios';
   }
   else {
    //Añadir un formulario para indicar manualmente el equipo que va ganando en cada partido.
   }
   return $form;
}
/**
 * Función para validar los cambios realizados a la configuración del torneo por el usuario
 */
function torneo_admin_form_validate ($form, $form_state) {
  // Encuentra si estamos haciendo una actualización de un Torneo o insertando uno nuevo.
   $node = $form['#node'];
   db_set_active('eSM');
    $settings = db_fetch_object(db_query("SELECT count(nid_Torneo) as n FROM {Torneos} WHERE nid_Torneo = %d", $node->nid));
   db_set_active('default');
   
   $is_update = $settings && ($settings->n > 0);
   
   if ($is_update) {
     if ($settings->Estado == PASADO) {
       drupal_set_message(t('Los torneos pasados no se pueden editar.'), 'error');
       drupal_goto('node/'. $node->nid);
     }
   }
}
/**
 * Función para guardar los cambios realizados por el usuario en la forma de configuración del torneo
 */
function torneo_admin_form_submit ($form, $form_state) {
  // Encuentra si estamos haciendo una actualización de un Torneo o insertando uno nuevo.
   $node = $form['#node'];
   
   $fecha_incio = strtotime($form_state['values']['Fecha_Inicio']);
   $fecha_final = strtotime($form_state['values']['Fecha_Final']);
   
   db_set_active('eSM');
    $settings = db_fetch_object(db_query("SELECT count(nid_Torneo) as n FROM {Torneos} WHERE nid_Torneo = %d", $node->nid));
   db_set_active('default');
   
   $is_update = $settings && ($settings->n > 0);
   
   //Encuentra el id_Modalidad_Juego_Temporada de la categoría asignada al nodo.
   if ($node->taxonomy) {
    
    foreach ($node->taxonomy as $vid => $term) {
      
      $modalidad = term_is_active($term->tid);
      
      if (is_object($modalidad)) {        //El nodo es activo en una modalidad y tenemos ya los datos de la modalidad.
        
        if ($is_update) {
          db_set_active('eSM');
            db_query("UPDATE {Torneos}
                     SET id_Modalidad_Juego_Temporada=%d, Tipo=%d, Eliminacion=%d, Rondas_Dia=%d, Fecha_Inicio=%d, Fecha_Final=%d WHERE nid_Torneo = %d", 
                    $modalidad->id_Modalidad_Juego_Temporada, $form_state['values']['Tipo'], $form_state['values']['Eliminacion'], $form_state['values']['Rondas_Dia'], $fecha_incio, $fecha_final, $node->nid);
          db_set_active('default');
          
          if (module_exists('user_badges')) {
            db_set_active('eSM');
              db_query('UPDATE {Torneos_Badges}
                       SET Badge_Win = %d, Badge_Participant = %d WHERE nid_Torneo = %d',
                      $form_state['values']['Badge_Win'], $form_state['values']['Badge_Participant'], $node->nid);
            db_set_active('default');
          }
        } 
        else {
          
          db_set_active('eSM');
            db_query("INSERT INTO {Torneos} (nid_Torneo, id_Modalidad_Juego_Temporada, Tipo, Eliminacion, Rondas_Dia, Fecha_Inicio, Fecha_Final, Estado) 
             VALUES(%d, %d, %d, %d, %d, %d, %d, %d)", 
             $node->nid, $modalidad->id_Modalidad_Juego_Temporada, $form_state['values']['Tipo'], $form_state['values']['Eliminacion'], $form_state['values']['Rondas_Dia'], $fecha_incio, $fecha_final, FUTURO);
          db_set_active('default');
          
          if (module_exists('user_badges')) {
            db_set_active('eSM');
              db_query('INSERT INTO {Torneos_Badges} (nid_Torneo, Badge_Win, Badge_Participant)
                      VALUES (%d, %d, %d)',
                      $node->nid, $form_state['values']['Badge_Win'], $form_state['values']['Badge_Participant']);
            db_set_active('default');
          }
        }
        drupal_set_message(t('Se han guardado las opciones correctamente.'));
        break;
      }
    }
   }
   else {
    
    drupal_set_message (t('Este nodo no esta asociado a ninguna modalidad.'), 'error');
    drupal_goto('node/'. $node->nid);
   }
}