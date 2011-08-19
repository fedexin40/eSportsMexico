<?php
/**
 * Función para mostrar la vista de los participantes y equipos registrados en un torneo
 */
function torneo_resultados () {
  //Obtiene el nid_Torneo de la barra de dirección
  $nid_Torneo = arg(1);
  
  drupal_set_title ('Partidos Finalizados');
  
  //Obtiene la lista de retos
  $retos = _match_torneo_get_match_list($nid_Torneo, COMPLETADO);
  
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
      
      if($reto->Estado == COMPLETADO) {
        //Obtiene el resultado del desafio
        $resultado = _match_get_resultado($reto->nid_Reto);
        
        if ($resultado->nid_Equipo_Ganador == $home_team->nid) {
          $form[ $count ][ 'Resultado' ] = array (
              '#type' => 'item',
              '#value' => l($home_team->title, 'node/' .$home_team->nid),
          );
        }
        else {
          $form[ $count ][ 'Resultado' ] = array (
              '#type' => 'item',
              '#value' => l($away_team->title, 'node/' .$away_team->nid),
          );
        }
      }
      
      $form[ $count ][ 'Ronda' ] = array (
          '#type' => 'item',
          '#value' => $reto->Ronda,
      );
      
      $form[ $count ][ 'Partido' ] = array (
          '#type' => 'item',
          '#value' => l('Detalles', 'node/'. $reto->nid_Reto),
      );
  }
  
  if ($count == 0) {
      $form['Mensaje'] = array (
          '#value' => t('No se han jugado partidos en este torneo.'),
      );
  }
  
  $form['#theme'] = 'torneo_lista_desafios';
  return $form;
}