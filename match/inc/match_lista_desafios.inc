<?php
/**
 * This function is to display a list of matches
 */
function team_lista_desafios () {
  $op = arg(3);
  $nid_Equipo = arg(1);
  
  switch ($op) {
    case 'confirmar':
      $op = 'por confirmar';
      $retos = _match_get_team_match_list($nid_Equipo, CREADO);
      break;
    
    case 'conflicto':
      $op = 'en conflicto';
      $retos = _match_get_team_match_list($nid_Equipo, CONFLICTO);
      break;
    
    case 'terminados':
      $retos = _match_get_team_match_list($nid_Equipo, COMPLETADO);
      break;
    
    case 'programados':
    default:
      $op = 'programados';
      $retos = _match_get_team_match_list($nid_Equipo, PROGRAMADO);
      break;
    
  }
  
  while( $reto = db_fetch_object( $retos ) ) {
      ++$count;
      
      if ($reto->nid_Equipo_Local == $nid_Equipo)
          $rival = node_load($reto->nid_Equipo_Visitante);
      else
          $rival = node_load($reto->nid_Equipo_Local);

      $form[ $count ][ 'Rival' ] = array (
          '#type' => 'item',
          '#value' => l($rival->title, 'node/'. $rival->nid),
      );
      
      $form[ $count ][ 'Fecha' ] = array (
          '#type' => 'item',
          '#value' => format_date($reto->Fecha_Inicio, 'large'),
      );
      
      if($reto->Estado == COMPLETADO) {
        //Obtiene el resultado del desafio
        $resultado = _match_get_resultado($reto->nid_Reto);
        
        if ($resultado->nid_Equipo_Ganador == $nid_Equipo) {
          $form[ $count ][ 'Resultado' ] = array (
              '#type' => 'item',
              '#value' => 'Victoria'
          );
        }
        else {
          $form[ $count ][ 'Resultado' ] = array (
              '#type' => 'item',
              '#value' => 'Derrota'
          );
        }
        if ($reto->nid_Equipo_Local == $nid_Equipo) {
          $form[ $count ][ 'Puntos' ] = array (
              '#type' => 'item',
              '#value' => $resultado->Puntuacion_Final_Equipo_Local - $reto->Puntuacion_Inicial_Equipo_Local,
          );
        }
        else {
          $form[ $count ][ 'Puntos' ] = array (
              '#type' => 'item',
              '#value' => $resultado->Puntuacion_Final_Equipo_Visitante - $reto->Puntuacion_Inicial_Equipo_Visitante,
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
  
  $form['#theme'] = 'match_lista_desafios';
  return $form;
}