<?php
/**
 * This function is to display a list of matches
 */
function modalidad_lista_desafios () {
  $op = arg(4);
  $tid = arg(2);
  
  //Obtiene los datos de la modalidad con base en el tid que proporciona la ruta
  $modalidad = term_is_active($tid);  
  drupal_set_title(t('Matches from '). $modalidad->Nombre);
  
  switch ($op) {
    case 'scheduled':
      $op = 'programados';
      $retos = _match_modalidad_get_match_list($modalidad->id_Modalidad_Juego_Temporada, PROGRAMADO);
      break;
    
    case 'results':
      $op = 'terminados';
      $retos = _match_modalidad_get_match_list($modalidad->id_Modalidad_Juego_Temporada, COMPLETADO);
      break;
    
    case 'conflict':
      $op = 'conflicto';
      $retos = _match_modalidad_get_match_list($modalidad->id_Modalidad_Juego_Temporada, CONFLICTO);
      break;
  }
  
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
      
      if($reto->Tipo == IS_TORNEO) {
        //Verifica si el desafío forma parte de un torneo o no
        $form[ $count ][ 'Tipo' ] = array (
            '#type' => 'item',
            '#value' => t('Torneo'),
        );
      } else {
        $form[ $count ][ 'Tipo' ] = array (
            '#type' => 'item',
            '#value' => t('Desafio'),
        );
      }
      
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
      
      $form[ $count ][ 'Partido' ] = array (
          '#type' => 'item',
          '#value' => l('Detalles', 'node/'. $reto->nid_Reto),
      );
  }
  
  if ($count == 0) {
      $form['Mensaje'] = array (
          '#value' => t('No existen desafios '. $op .'.'),
      );
  }
  
  $form['#theme'] = 'match_lista_desafios_modalidad';
  return $form;
}