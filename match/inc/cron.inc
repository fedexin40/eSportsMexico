<?php
/**
 * Funci�n para propagar los resultados en los desaf�os con m�s de X horas de programados
 * y para los que todav�a no se hayan reportado resultados de los dos participantes
 */
function _match_cron_task () {
  //Define el tiempo m�ximo de duraci�n de los partidos en espera de reporte de resultado
  $horas = 3; //Cambiar el valor de esta variable para cambiar el n�mero de horas de tolerancia para el reporte de resultados
  
  $fecha = time() - ($horas * 60 * 60);
  
  //Obtiene una lista de partidos con m�s de 3 horas de programado y que contin�an en estado de programado
  db_set_active('eSM');
    $retos = db_query('SELECT * FROM {Reto} AS r INNER JOIN {Inicio_Reto} AS ir ON r.nid_Reto = ir.nid_Reto
                         WHERE Estado = %d AND Fecha_Inicio <= %d', PROGRAMADO, $fecha);
  db_set_active('default');
  
  while ($reto = db_fetch_object($retos)) {
    $reporte_equipo_local = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Local);
    $reporte_equipo_visitante = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Visitante);
    $nid_Ganador = 0;
    
    //Resultado reportado �nicamente por el equipo local
    if(is_object($reporte_equipo_local)) {
      if ($reporte_equipo_local->Resultado == VICTORIA) {
        //Resultado coherente indicando que gan� el equipo local
        $nid_Ganador = $reto->nid_Equipo_Local;
      }
      elseif ($reporte_equipo_local->Resultado == DERROTA) {
        //Resultado coherente indicando que gan� el equipo visitante
        $nid_Ganador = $reto->nid_Equipo_Visitante;
      }
    }
    
    //Resultado reportado �nicamente por el equipo visitante
    if(is_object($reporte_equipo_visitante)) {
      if ($reporte_equipo_visitante->Resultado == VICTORIA) {
        //Resultado coherente indicando que gan� el equipo local
        $nid_Ganador = $reto->nid_Equipo_Visitante;
      }
      elseif ($reporte_equipo_visitante->Resultado == DERROTA) {
        //Resultado coherente indicando que gan� el equipo visitante
        $nid_Ganador = $reto->nid_Equipo_Local;
      }
    }
      
    //Si no hay un ganador definido, entonces el desaf�o debe ir al estado de conflicto
    if ($nid_Ganador == 0) {
      _match_update_match($reto->nid_Reto, CONFLICTO);
    }
    else {
      _match_end_match($reto->nid_Reto, $nid_Ganador);
    }
  }
}