<?php
/**
 * Archivo para definir y manejar las características y configuraciones de los torneos
 */
/**
 * Page callback for the node/N/signups/settings subtab.
 */
function torneo_admin_results($node) {
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
     
     $form['Encabezado'] = array(
      '#value' => t('A continuacion se presenta una lista de todos los desafios en conflicto de este torneo.<br/>
                    Para ver las pruebas de cada uno haz click en el link "Detalles" que se encuentra a la derecha de cada partido.<br/>
                    Para actualizar el estado de un desafio selecciona el equipo ganador de la lista desplegable y haz click en el boton Guardar Cambios.<br/>'),
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
            '#value' => l('Detalles', 'node/'. $reto->nid_Reto),
        );
        
        $form[ 'Ganador_'. $count ] = array (
          '#type' => 'select',
          '#options' => array(
            0 => t('Selecciona al equipo ganador'),
            $home_team->nid => $home_team->title,
            $away_team->nid => $away_team->title,   
            ),
          '#default_value' => 0,
        );
        $form[ 'nid_Reto_'. $count ] = array (
          '#type' => 'hidden',
          '#value' => $reto->nid_Reto,
        );
    }
    
    if ($count == 0) {
        $form['Mensaje'] = array (
            '#value' => t('No existe ningun partido en conflico para este torneo.'),
        );
    }
    else {
      $form['Submit'] = array(
        '#type' => 'submit',
        '#value' =>  t('Guardar cambios'),
      );
    }
    
    $form['#theme'] = 'torneo_lista_desafios_conflicto';
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

}
/**
 * Función para guardar los cambios realizados por el usuario en la forma de configuración del torneo
 */
function torneo_admin_form_submit ($form, $form_state) {
  $count = 0;
  
  //Recorre todos los elementos de la tabla para ver si se han establecido ganadores 
  foreach($form_state['values'] as $index => $element) {
    if (substr($index, 0, 7) == 'Ganador') {
      if ($element != 0) {
        //Si el elemento ha sido seleccionado hacer los cambios correspondientes
        $identificador = substr($index, 8, 1);
        
        $nid_Reto = $form_state['values']['nid_Reto_'. $identificador];
        $nid_Ganador = $element;
        
        if (module_exists('match')) {
          _match_end_match($nid_Reto, $nid_Ganador);
        }
        
        ++$count; 
      }
    }
  }
  
  if ($count == 0) 
    drupal_set_message (t('Debes seleccionar por lo menos un desafio para actualizar el resultado.'), 'error');
  else
    drupal_set_message(t('Se han actualizado '. $count .' desafios'));
}