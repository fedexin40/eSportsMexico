<?php
/**
 * Archivo para definir y manejar las características y configuraciones de los torneos
 */
/**
 * Page callback for the node/N/signups/settings subtab.
 */
function torneo_admin_settings_page($node) {
  return drupal_get_form('torneo_admin_settings_form', $node, $node->type, TRUE);
}
/**
 * Función para definir la forma de configuración de los torneos
 */
function torneo_admin_settings_form($form_state, $node = NULL, $node_type = NULL, $include_buttons = FALSE) {
   $settings = get_settings_torneo($node->nid);    //Obtiene los resultados de la tabla si es que el torneo ya existe.
   
    $date = '2011-12-31 00:00';
    $format = 'Y-m-d H:i';
   
   if (module_exists('user_badges')) {
    $badges = user_badges_get_badges('all');
    $options[0] = 'Ninguna';
    foreach ($badges as $badge) {
     $options[$badge->bid] = $badge->name;
    }
   }
   //Verfica que el nodo que se está visualizando esté asociado por medio de un término de taxonomía
   //a una modalidad activa en la temporada actual para poder ver la forma de administración
   //en caso contrario regresa un mensaje de error de acceso.
   if ($node->taxonomy) {
    $form['torneo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => -3,
      );
    $form['torneo']['instrucciones'] = array(
      '#value' => '<p>Define las caracteristicas de este evento.</p>',
      '#weight' => -3,
      );
    $form['torneo']['Tipo'] = array(
      '#type' => 'radios',
        '#title' => t("Tipo de Torneo"),
        '#description' => t('Select the type of the event'),
        '#options' => array(
          MASTER_CUP => t('Master Cup. Para los eventos trimestrales y finales.'),
          MINI_CUP => t('Mini Cup. Para los eventos regionales.'),
          FUN_CUP => t('Fun Cup. Para los torneos de diversion.'),
          ),
        '#default_value' => $settings->Tipo,
        '#weight' => -100,
        '#required' => TRUE,
      );
    $form['torneo']['Eliminacion'] = array(
      '#type' => 'select',
      '#title' => t('Simple o Doble eliminacion'),
      '#description' => t('Selecciona si es un evento de eliminacion doble o directa.'),
      '#options' => array(
        ELIMINACION_SIMPLE => t('Eliminacion directa'),
        ELIMINACION_DOBLE => t('Doble eliminacion'),
        ),
      '#default_value' => $settings->Eliminacion,
      '#weight' => -2,
      '#required' => TRUE,
      );
    $form['torneo']['Rondas_Dia'] = array(
      '#type' => 'select',
      '#title' => t('Rondas por Dia'),
      '#description' => t('Selecciona el numero de rondas a jugarse en cada dia de torneo.'),
      '#options' => array(
        1 => t('1'),
        2 => t('2'),
        3 => t('3'),
        4 => t('4'),
        5 => t('5'),
        ),
      '#default_value' => $settings->Rondas_Dia,
      '#weight' => -2,
      '#required' => TRUE,
      );
    $form['torneo']['Fecha_Inicio'] = array(
      '#type' => 'date_select',
      '#title' => t('Fecha de Inicio del torneo'),
      '#description' => t('Selecciona la fecha y hora en la que inicia el torneo.'),
      '#default_value' =>  format_date($settings->Fecha_Inicio, 'small', $format),//timestamp_to_string($settings->Fecha_Inicio),
      '#date_format' => $format,
      '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
      '#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
      '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
      '#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
      '#weight' => -4,
      );
    $form['torneo']['Fecha_Final'] = array(
      '#type' => 'date_select',
      '#title' => t('Fecha de la Final'),
      '#description' => t('Selecciona la fecha y hora en la que se va a jugar la final si no sigue la secuencia de los primeros dias de juego.'),
      '#default_value' => format_date($settings->Fecha_Final, 'small', $format),//timestamp_to_string($settings->Fecha_Final),
      '#date_format' => $format,
      '#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
      '#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
      '#date_increment' => 15, // Optional, used by the date_select and date_popup elements to increment minutes and seconds.
      '#date_year_range' => '0:+2', // Optional, used to set the year range (back 3 years and forward 3 years is the default).
      '#weight' => -2,
      );
 // Si existe el módulo user_badges asigna un espacio para definir las insignias que ganarán los equipos participantes y ganadores.
     if (is_array($options)) {
       $form['badges'] = array(
         '#type' => 'fieldset',
         '#title' => t('Badges for winners & participants'),
         '#required' => TRUE,
         '#collapsible' => TRUE,
         '#collapsed' => TRUE,
         '#weight' => -1,
         );
       $form['badges']['instrucciones'] = array(
         '#value' => t('<p>Define las inignias que van a ganar los equipos participantes y ganadores de este torneo.</p>'),
         '#weight' => -1,
         );
       $form['badgesm']['Badge_Win']= array(
         '#type' => 'select',
         '#title' => t("Badged awarded for the winner"),
         '#description' => t('Select the badged that will be awarded to the winner of the event.'),
         '#default_value' => $settings->Badge_Win,
         '#options' => $options,
         '#weight' => 2,
         );
       $form['badgesm']['Badge_Participant']= array(
         '#type' => 'select',
         '#title' => t("Badged awarded for participants"),
         '#description' => t('Select the badged that will be awarded to all the participants on this event.'),
         '#default_value' => $settings->Badge_Participant,
         '#options' => $options,
         '#weight' => 2,
         );
     }
      
     $form['#node'] = $node;    //Agrega el objeto node a la forma para poder manipularlo dentro de la función de submit
     $form['buttons']['submit'] = array(
       '#type' => 'submit',
       '#value' => t('Save configuration'),
     );
     $form['buttons']['reset'] = array(
       '#type' => 'submit',
       '#value' => t('Cancel'),
     );
     
     $form['#validate'][] = 'torneo_admin_settings_form_validate';
     $form['#submit'][] = 'torneo_admin_settings_form_submit';
   }
   else {
    $form['torneo'] = array (
      '#value' => t('Este torneo no pertenece a una modalidad activa en la temporada actual.'),
    );
   }
   return $form;
}
/**
 * Función para validar los cambios realizados a la configuración del torneo por el usuario
 */
function torneo_admin_settings_form_validate ($form, $form_state) {
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
function torneo_admin_settings_form_submit ($form, $form_state) {
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
            db_query("INSERT INTO {Torneos} (nid_Torneo, is_Modalidad_Juego_Temporada, Tipo, Eliminacion, Rondas_Dia, Fecha_Inicio, Fecha_Final, Estado) 
             VALUES(%d, %d, %d, %d, %d, %d, %d)", 
             $node->nid, $form_state['values']['Tipo'], $form_state['values']['Eliminacion'], $form_state['values']['Rondas_Dia'], $fecha_incio, $fecha_final, FUTURO);
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