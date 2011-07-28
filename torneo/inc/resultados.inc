<?php
/**
 * Función para mostrar la vista de los participantes y equipos registrados en un torneo
 */
function torneo_resultados () {
  $torneo = get_settings_torneo(arg(1));                                 //Obtiene las configuraciones del torneo del nid obtenido en la barra de dirección
  $modalidad = get_modalidad($torneo->id_Modalidad_Juego_Temporada);    //Obtiene los datos de la modalidad
  
  db_set_active('eSM');                                                 //Obtiene la lista de participantes
    $participantes = db_query('SELECT * FROM {Torneo_Participantes} WHERE nid = %d', $torneo->nid_Torneo);
  db_set_active('default');
  
  $count = 0;                                                           //Inicia el contador de los participantes
  
  //Despliega la lista de participatnes
  while ( $equipo = db_fetch_object( $participantes ) )
  {
    $equipo = node_load( array( 'nid' => $equipo->nid_Equipo ) );

    $form['participantes'][ $equipo->nid ][ 'nombre' ] = array (
      '#value' => l($equipo->title, 'node/'. $equipo->nid),
    );
    
    ++$count;
  }
  
  if ($count == 0) {
    $form['mensaje'] = array (
      '#value' => t('No hay equipos inscritos en este torneo todavia.'),
    );
  }
  
  return $form;
}